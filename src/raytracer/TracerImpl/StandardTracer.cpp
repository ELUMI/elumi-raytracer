/*
 * StandardTracer.cpp
 *
 *  Created on: Feb 14, 2012
 *      Author: irri
 */

#include <iostream>

#include "StandardTracer.h"
#include "../utilities/Random.h"
#include "../EnvironmentMapImpl/CubeMap.h"

namespace raytracer {

// TODO report, change threshold comparison images
StandardTracer::StandardTracer(Scene* scene) :
  BaseTracer(scene), MAX_RECURSION_DEPTH(settings->max_recursion_depth),
      ATTENUATION_THRESHOLD(settings->recursion_attenuation_threshold) {
  using_environment_map = false;
}

StandardTracer::~StandardTracer() {

}

void StandardTracer::initTracing() {
  BaseTracer::initTracing();
}

void StandardTracer::first_bounce(){
  BaseTracer::first_bounce();
}

void StandardTracer::traceImage(float* color_buffer) {
  if (scene->getEnvironmentMap() != NULL)
    using_environment_map = true;

  BaseTracer::traceImage(color_buffer);
}

vec4 StandardTracer::trace(Ray ray, IAccDataStruct::IntersectionData idata,
    int thread_id) {
  return shade(ray, idata, 1.0f, 0, thread_id);
}

// Recursive trace method (trace')
vec4 StandardTracer::tracePrim(Ray ray, float attenuation,
    unsigned short depth, int thread_id) {
  IAccDataStruct::IntersectionData idata = datastruct->findClosestIntersection(
      ray);
  return shade(ray, idata, attenuation, depth, thread_id);
}

inline vec3 StandardTracer::getPerturbedNormal(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata, Material* material,vec2 tex_coords){
  vec3 normal = idata.normal;

  if(material->getNormalMap() != -1) {
    Texture* normal_map = scene->getTextureAt(material->getNormalMap());
    vec3 perturbed_normal = normal_map->getColorAt(tex_coords,material->getScale(),material->getCorresponder());
    return normalize(faceforward(perturbed_normal,incoming_ray.getDirection(),normal));
  }

  return vec3();
}

vec3 StandardTracer::brdf(vec3 incoming_direction,
    vec3 outgoing_direction, vec3 normal,
    Material * material, vec3 texture_color, vec2 tex_coords)
{
  //diffuse
  vec3 color = clamp(glm::dot(-incoming_direction, normal)) * texture_color;

  //specular
  vec3 h = normalize(-outgoing_direction - incoming_direction);
  vec3 specular =
      glm::pow(clamp(glm::dot(normal, h)), material->getShininess())
          * material->getSpecular();

  if(material->getSpecularMap() != -1) {
    Texture *spec = scene->getTextureAt(material->getSpecularMap());
    specular *= length(spec->getColorAt(tex_coords,material->getScale(),material->getCorresponder()));
  }

  return color + specular;
}

vec2 StandardTracer::getTextureCoordinates(Material* material,
    IAccDataStruct::IntersectionData idata, vec3 displacement) {

  vec2 texture_coordinates = vec2();
  vec3 p = idata.interPoint + displacement;

  if(material->getProjector() != UV){
    texture_coordinates = Texture::getUVCoordinates(p, idata.normal, material->getScale(),
        material->getProjector(),material->getAxis(), material->getUsePosition());
  }else{
    //We have texture coordinates
    texture_coordinates = idata.texcoord;
  }

  return texture_coordinates;
}

vec3 StandardTracer::getParallax(Material* material,IAccDataStruct::IntersectionData idata,vec2 tex_coords) {
  if(material->getBumpMap() != -1) {
    Texture* bump_map = scene->getTextureAt(material->getBumpMap());
    float height = glm::length(bump_map->getColorAt(tex_coords,material->getScale(),material->getCorresponder()));
    return height*normalize(scene->getCamera().getPosition()-idata.interPoint)/15.0f;
  }
  return vec3(0,0,0);
}

vec3 StandardTracer::getTextureColor(Material* material,
    IAccDataStruct::IntersectionData idata,vec2 tex_coords)
{
  if(material->getDiffuseMap() == -1){
    return material->getDiffuse();
  } else {
    Texture* texture = scene->getTextureAt(material->getDiffuseMap());
    return texture->getColorAt(tex_coords,material->getScale(),material->getCorresponder());
  }
  return vec3();
}

inline vec3 StandardTracer::reflection_refraction(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata,
    float attenuation, unsigned short depth,
    Material *material, vec3 normal, vec3 color, vec2 tex_coords, int thread_id) {

  if (attenuation < ATTENUATION_THRESHOLD || depth > MAX_RECURSION_DEPTH) {
    return color;
  }

  const float offset_size = 0.01f;
  float reflectance = material->getReflection();
  float transmittance = (1 - material->getOpacity());

  //Transparency map
  if(material->getTransparencyMap() != -1) {
    Texture* transparencymap = scene->getTextureAt(material->getTransparencyMap());
    transmittance = glm::length(transparencymap->getColorAt(tex_coords,material->getScale(),material->getCorresponder()));
  }

  // REFRACTION_SIGN: //missledande namn, tecknet är vilket håll offsetten typ förskjuts
  // Ray enters mesh => -1
  // Ray leaves mesh => 1
  const float refraction_sign = glm::sign(
      glm::dot(normal, incoming_ray.getDirection()));
  const vec3 refr_normal = -normal * refraction_sign;

  if(material->getReflectionMap() != -1) {
    Texture* reflectionmap = scene->getTextureAt(material->getReflectionMap());
    reflectance = glm::length(reflectionmap->getColorAt(tex_coords,material->getScale(),material->getCorresponder()));
  }

  //Fresnel reflectance
  if (material->getFresnelIndex() != 0.0f) {
  //if (settings->use_fresnel) {
    //Schlick's approx.
//    float fresnel_refl = reflectance + (1 - reflectance) * glm::pow(
//    clamp(1.0f - abs(glm::dot(-incoming_ray.getDirection(), normal))), 5.0f);
//
//    cout << fresnel_refl << "\n";


    float c = abs(glm::dot(incoming_ray.getDirection(), normal));
    float g = glm::sqrt( material->getFresnelIndex()*material->getFresnelIndex() + c*c -1 );
    float gMc = g-c;
    float gPc = g+c;
    float fresnel_refl = 0.5 * ((gMc*gMc) / (gPc*gPc)) *
                         (  1.0f +
                             ( (c*(gPc)-1)*(c*(gPc)-1) )
                           / ( (c*(gMc)+1)*(c*(gMc)+1) )
                         );
    reflectance = fresnel_refl;
  }

  /**** REFRACTION RAY ****/
  if (transmittance > 0.0f /*&& reflectance < 1.0f*/) {

    float refract_spread = material->getRefractionSpread();
    int refract_samples = material->getRefractionSamples();
    float eta = material->getIndexOfRefraction();

    if (refraction_sign == -1.0f)
      eta = 1 / eta;

    vec3 offset = -refr_normal * offset_size;
    vec3 refr_dir = glm::refract(incoming_ray.getDirection(), refr_normal, eta);

    if (refr_dir == vec3(0, 0, 0)) {
      // Refraction direction imaginary => Total internal reflection
      refr_dir = glm::reflect(incoming_ray.getDirection(), refr_normal);
    }

    Ray refr_ray = Ray(idata.interPoint + offset, refr_dir);
    vec4 refr_color;

    if (refract_spread > 0.0f && refract_samples > 0) { // Glossy refractions
      for (int i = 0; i < refract_samples; ++i) {

        vec3 sample_dir = glm::normalize(
            gen_random_cone(refr_dir, refract_spread, thread_id));

        Ray sample_ray = Ray(idata.interPoint + offset, sample_dir);

        refr_color += tracePrim(sample_ray,
            attenuation * (transmittance) / refract_samples, depth + 1,
            thread_id) * (transmittance) / refract_samples;

      }
    } else { // Non glossy
      refr_color = tracePrim(refr_ray, attenuation * (transmittance),
          depth + 1, thread_id) * (transmittance);
    }
    // SVART BEROR PÅ SELF SHADOWING
    //mix with output
    color = color * (1 - transmittance) + vec3(refr_color);
  } // end refraction

  /**** REFLECTION RAY ****/
  if (reflectance > 0.0f) {
    vec3 offset = refr_normal * offset_size;
    vec3 refl_dir = glm::reflect(incoming_ray.getDirection(), refr_normal);
    Ray refl_ray = Ray(idata.interPoint + offset, glm::normalize(refl_dir));

    vec4 refl_color;

    float reflect_spread = material->getReflectionSpread();
    int reflect_samples = material->getReflectionSamples();

    if (reflect_spread > 0.0f && reflect_samples > 0) { // Glossy reflections
      for (int i = 0; i < reflect_samples; ++i) {

        vec3 sample_dir = glm::normalize(
            gen_random_cone(refl_dir, reflect_spread, thread_id));

        Ray sample_ray = Ray(idata.interPoint + offset, sample_dir);

        refl_color += tracePrim(sample_ray,
            attenuation * reflectance / reflect_samples, depth + 1, thread_id)
            * reflectance / reflect_samples;

      }
    } else { // Non glossy
      refl_color = tracePrim(refl_ray, attenuation * reflectance, depth + 1,
          thread_id) * reflectance;
    }
    // SVART BEROR PÅ ATT DEN STUDSAR MOT SIG SJÄLV OCH ALLTID BLIR REFLECTIVE TILLS MAXDJUP
    //mix with output
    color = color * (1 - reflectance) + vec3(refl_color);
  }



  return color;
}

vec3 StandardTracer::getAmbient(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata, int thread_id, unsigned short depth) {
  Material *material = scene->getMaterialVector()[idata.material];
  vec3 color = vec3(0);
  for (unsigned int i = 0; i < lights->size(); ++i) {
    ILight *light = lights->at(i);
    if (light->getFalloffType() == ILight::NONE) {
      //do nothing, handled in getAmbient()
      color += light->getColor() * material->getAmbient();
    }
  }

  return color;
}

float StandardTracer::getIndividualLight(vec3 pos, ILight* light, int thread_id) {
  return light->calcLight(datastruct, pos, thread_id);
}

vec3 StandardTracer::getLighting(
    Ray incoming_ray
    , IAccDataStruct::IntersectionData idata
    , vec3 normal
    , Material *material
    , vec2 tex_coords
    , int thread_id
    , vec3 parallax
)
{
  vec3 color = vec3(0);

  vec3 perturbed_normal = getPerturbedNormal(incoming_ray,idata,material,tex_coords);
  //vec3 perturbed_normal = vec3(0,0,0);
  vec3 texture_color = getTextureColor(material, idata,tex_coords);

  /**** For each light source in the scene ****/
  for(unsigned int i = 0;i < lights->size();++i){
    ILight *light = lights->at(i);
    if(light->getFalloffType() == ILight::NONE){
      //do nothing, handled in getAmbient()
    }else{
      /**** NON AMBIENT LIGHT, CALCULATE SHADOW RAYS ***/
      float in_light = getIndividualLight(idata.interPoint, light, thread_id);
      if(in_light > 0.0f){

        float s = 1.0f;

        if(material->getBumpMap() != -1 && material->getUseRelief()) {

          vec3 view = normalize(light->getPosition()-idata.interPoint);

          if(dot(view,idata.normal) > 0) {
            vec2 t1 = getTextureCoordinates(material, idata,parallax);
            Texture* bump_map = scene->getTextureAt(material->getBumpMap());
            float h0 = glm::length(bump_map->getColorAt(t1,material->getScale(),material->getCorresponder()));

            float bias = 1.5f;
            for(float i = 1; i <= 10; i++) {
              vec3 p = view + (view * i * 0.1f);
              vec2 t2 = getTextureCoordinates(material,idata,p);
              p = -faceforward(p,vec3(0,1,0),idata.normal);
              t2.x += p.x;
              t2.y += p.z;

              float h = glm::length(bump_map->getColorAt(t2,material->getScale(),material->getCorresponder()));

              if (p.y*bias + h0 < h) {
                s -= (glm::pow(0.2f,i/2.0f));
              }
            }
            s = glm::clamp(s,0.0f,1.0f);
          }
        }


        // NOT ENTIRELY IN SHADOW! SHADE!
        Ray light_ray = Ray::generateRay(light->getPosition(), idata.interPoint);
        color += s*light->getColor() * in_light * brdf(light_ray.getDirection(), incoming_ray.getDirection(), normalize(normal+perturbed_normal), material, texture_color);
      }
    }
  }
  return color;
}

vec4 StandardTracer::shade(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata, float attenuation,
    unsigned short depth, int thread_id) {
  if (idata.missed()) {
    // No intersection
    if (using_environment_map)
      return vec4(scene->getEnvironmentMap()->getColor(incoming_ray), 1.0f);
    return settings->background_color;
  }
  assert(idata.material < scene->getMaterialVector().size());
  // Intersection!
  Material *material = scene->getMaterialVector()[idata.material];
  vec3 normal = idata.normal;
  vec3 color = material->getEmissive();

  vec2 tex_coords = getTextureCoordinates(material,idata,vec3(0,0,0));
  vec3 parallax = getParallax(material,idata,tex_coords);
  tex_coords = getTextureCoordinates(material,idata,parallax);

  color += getAmbient(incoming_ray, idata, thread_id, depth);
  color += getLighting(incoming_ray, idata, normal, material, tex_coords, thread_id, parallax);
  color = reflection_refraction(incoming_ray, idata, attenuation, depth, material, normal, color, tex_coords,thread_id);

  return vec4(color, 1.0f);
}

}
