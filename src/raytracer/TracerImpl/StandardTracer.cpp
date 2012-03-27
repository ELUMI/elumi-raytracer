/*
 * StandardTracer.cpp
 *
 *  Created on: Feb 14, 2012
 *      Author: irri
 */




#include "StandardTracer.h"

namespace raytracer {

// TODO rapport ändra threshold jämförelsebilder
StandardTracer::StandardTracer(Scene* scene)
: BaseTracer(scene)
, MAX_RECURSION_DEPTH(settings->max_recursion_depth)
, ATTENUATION_THRESHOLD(settings->recursion_attenuation_threshold) {

}

StandardTracer::~StandardTracer() {

}

void StandardTracer::initTracing(){
  BaseTracer::initTracing();
}

void StandardTracer::traceImage(float* color_buffer) {
  BaseTracer::traceImage(color_buffer);
}

vec4 StandardTracer::trace(Ray ray, IAccDataStruct::IntersectionData idata) {
  return shade(ray, idata, 1.0f, 0);
}

vec4 StandardTracer::tracePrim(Ray ray, float attenuation, unsigned short depth) {
  IAccDataStruct::IntersectionData idata = datastruct->findClosestIntersection(ray);
  return shade(ray, idata, attenuation, depth);
}

inline vec3 StandardTracer::bumpMap(Material* material,
    IAccDataStruct::IntersectionData& idata, Ray incoming_ray){
  if(material->getBumpMap() == -1){
    return idata.normal;
  }

  /**** Bump mapping ****/
  vec3 bump_normal = vec3(0, 0, 0);
  Texture *bumpmap = scene->getTextureAt(material->getBumpMap());
  vec2 coords = bumpmap->getUVCoordinates(idata.interPoint, idata.e1, idata.e2);
  bump_normal = bumpmap->getColorAt(coords);
  bump_normal = normalize(faceforward(bump_normal, incoming_ray.getDirection(), idata.normal));
  return normalize(idata.normal + bump_normal);
}

inline vec3 StandardTracer::brdf(vec3 incoming_direction, vec3 outgoing_direction, vec3 normal, Material * material, vec3 texture_color)
{
  //diffuse
  vec3 color = clamp(glm::dot(-incoming_direction, normal)) * texture_color;

  //specular
  vec3 h = normalize(-outgoing_direction - incoming_direction);
  color += glm::pow(clamp(glm::dot(normal, h)), material->getShininess()) * material->getSpecular();

  //maybe some fresnel
  // Fresnel reflectance (with Schlick's approx.)
  //      float fresnel_refl = reflectance
  //          + (1 - reflectance)
  //          * glm::pow( clamp(1.0f + glm::dot(incoming_ray.getDirection(), normal) ), 5.0f);
  //      reflectance = fresnel_refl;

  return color;
}

vec3 StandardTracer::getTextureColor(Material* material, IAccDataStruct::IntersectionData & idata)
{
  if(material->getDiffuseMap() == -1){
    return material->getDiffuse();
  }

  vec3 texture_color = vec3(0);
  vec2 tex_coords = vec2(0, 0);
  Texture *texture = scene->getTextureAt(material->getDiffuseMap());
  int mipmap_levels = texture->getMipmapLevels();
  if(true){
    //tex_coords  = texture->getUVCoordinates(idata.interPoint,idata.e1,idata.e2);
    tex_coords = texture->getUVCoordinates(idata.interPoint, YAXIS);
  }else{
    //We have texture coordinates
    tex_coords = idata.texcoord;
  }
  if(false){
    float d = 0;
    if(d > 0){
      Texture *mmp_bottom = scene->getTextureAt(material->getDiffuseMap() + (int)((((d)))));
      Texture *mmp_top = scene->getTextureAt(material->getDiffuseMap() + (int)((((d)))) + 1);
      return ((1.0f - d - floor(d)) * mmp_bottom->getColorAt(tex_coords) + d - floor(d) * mmp_top->getInterpolatedColor(tex_coords));
    }else{
      return texture->getColorAt(tex_coords);
    }
    //float dv = 1.0f/(float)mipmap_levels;
  }
  else{
    return texture->getColorAt(tex_coords);
  }
  return texture_color;
}

inline vec3 StandardTracer::reflection_refraction(float attenuation,
    unsigned short depth,Material *material, vec3 normal, Ray incoming_ray,
    IAccDataStruct::IntersectionData idata, vec3 color) {
  if(attenuation < ATTENUATION_THRESHOLD || depth > MAX_RECURSION_DEPTH){
    return color;
  }
  float reflectance = material->getReflection();
  float transmittance = (1 - material->getOpacity());
  // REFRACTION_SIGN:
  // Ray enters mesh => -1
  // Ray leaves mesh => 1
  float refraction_sign = glm::sign(glm::dot(normal, incoming_ray.getDirection()));
  vec3 refr_normal = -normal * refraction_sign;
  /**** REFLECTION RAY ****/
  if(reflectance > 0.0f){
    vec3 offset = refr_normal * 0.01f;
    vec3 refl_dir = glm::reflect(incoming_ray.getDirection(), refr_normal);
    Ray refl_ray = Ray(idata.interPoint + offset, glm::normalize(refl_dir));
    vec3 refl_color = vec3(tracePrim(refl_ray, attenuation * reflectance, depth + 1));
    refl_color *= reflectance;
    // SVART BEROR PÅ ATT DEN STUDSAR MOT SIG SJÄLV OCH ALLTID BLIR REFLECTIVE TILLS MAXDJUP
    //mix with output
    color = color * (1 - reflectance) + vec3(refl_color);
  }

  /**** REFRACTION RAY ****/
  if(transmittance > 0.0f && reflectance < 1.0f){
    float eta = material->getIndexOfRefraction();
    if(refraction_sign == -1.0f)
      eta = 1 / eta;

    vec3 offset = -refr_normal * 0.01f;
    vec3 refr_dir = glm::refract(incoming_ray.getDirection(), refr_normal, eta);
    if(refr_dir == vec3(0, 0, 0)){
      transmittance = 0.0f;
    }else{
      Ray refr_ray = Ray(idata.interPoint + offset, refr_dir);
      vec4 refr_color = tracePrim(refr_ray, attenuation * (transmittance), depth + 1) * (transmittance);
      // SVART BEROR PÅ SELF SHADOWING
      //mix with output
      color = color * (1 - transmittance) + vec3(refr_color);
    }
  } // end refraction

  return color;
}

vec4 StandardTracer::shade(Ray incoming_ray, IAccDataStruct::IntersectionData idata, float attenuation, unsigned short  depth)
{
  if(idata.material == IAccDataStruct::IntersectionData::NOT_FOUND){
    // No intersection
    return settings->background_color;
  }
  assert(idata.material < scene->getMaterialVector().size());
  // Intersection!
  Material *material = scene->getMaterialVector()[idata.material];
  vec3 normal = bumpMap(material, idata, incoming_ray);
  vec3 texture_color = getTextureColor(material, idata);
  vec3 color = vec3(0);
  /**** For each light source in the scene ****/
  for(unsigned int i = 0;i < lights->size();++i){
    ILight *light = lights->at(i);
    if(light->getFalloffType() == ILight::NONE){
      color += light->getColor() * material->getAmbient();
    }else{
      /**** NON AMBIENT LIGHT, CALCULATE SHADOW RAYS ***/
      float in_light = light->calcLight(datastruct, idata.interPoint);
      if(in_light > 0.0f){
        // NOT ENTIRELY IN SHADOW! SHADE!
        Ray light_ray = Ray::generateRay(light->getPosition(), idata.interPoint);
        color += light->getColor() * in_light * brdf(light_ray.getDirection(), incoming_ray.getDirection(), normal, material, texture_color);
      } // end in light
    } // end non abmient

  } // for each light

  color = reflection_refraction(attenuation, depth, material, normal, incoming_ray, idata, color);

  return vec4(color,1.0f);
}

}
