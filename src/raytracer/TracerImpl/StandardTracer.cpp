/*
 * StandardTracer.cpp
 *
 *  Created on: Feb 14, 2012
 *      Author: irri
 */


#include "StandardTracer.h"

namespace raytracer {

StandardTracer::StandardTracer(Scene* scene, Settings* settings)
: BaseTracer(scene, settings) {

}

StandardTracer::~StandardTracer() {

}

void StandardTracer::traceImage(float* color_buffer) {
  max_recursion_depth = 3;
  BaseTracer::traceImage(color_buffer);
}

vec4 StandardTracer::trace(Ray ray, IAccDataStruct::IntersectionData idata) {
  return shade(ray, idata, 0);
}

vec4 StandardTracer::tracePrim(Ray ray,
    const unsigned int depth) {
  IAccDataStruct::IntersectionData idata = datastruct->findClosestIntersection(ray);
  return shade(ray, idata, depth);
}

vec4 StandardTracer::shade(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata,
    const unsigned int depth) {

  if (idata.material == IAccDataStruct::IntersectionData::NOT_FOUND) {
    // No intersection
    return settings->background_color;
  }
  assert(idata.material < scene->getMaterialVector().size());

  // Intersection!

  vec3 color      = vec3(0,0,0);
  vec3 ambient    = vec3(0,0,0);
  vec3 diffuse    = vec3(0,0,0);
  vec3 specular   = vec3(0,0,0);
  vec4 refl_color = vec4(0,0,0,0);
  vec4 refr_color = vec4(0,0,0,0);
  Material* material = scene->getMaterialVector()[idata.material];

  vec3 texture_color = vec3(0,0,0);

  vec3 bump_normal = vec3(0,0,0);
  vec3 normal = idata.normal;
  vec2 tex_coords = vec2(0,0);

  if(material->getDiffuseMap() != -1) {

    Texture* texture = scene->getTextureAt(material->getDiffuseMap());
    int mipmap_levels = texture->getMipmapLevels();

    if(true) {
      //tex_coords  = texture->getUVCoordinates(idata.interPoint,idata.e1,idata.e2);
      tex_coords = texture->getUVCoordinates(idata.interPoint,YAXIS);
    } else { //We have texture coordinates
      tex_coords = idata.texcoord;
    }

    if(false) {

      float d = 0;

      if(d > 0) {
        Texture* mmp_bottom = scene->getTextureAt(material->getDiffuseMap()+(int)d);
        Texture* mmp_top = scene->getTextureAt(material->getDiffuseMap()+(int)d+1);

        texture_color = ( (1.0f-d-floor(d))*mmp_bottom->getColorAt(tex_coords) + d-floor(d)*mmp_top->getInterpolatedColor(tex_coords) );
      } else {
        texture_color = texture->getColorAt(tex_coords);
      }

      //float dv = 1.0f/(float)mipmap_levels;

    } else {
      texture_color = texture->getColorAt(tex_coords);
    }

  }
  //Bump mapping
  if(material->getBumpMap() != -1) {
    Texture* bumpmap = scene->getTextureAt(material->getBumpMap());
    vec2 coords  = bumpmap->getUVCoordinates(idata.interPoint,idata.e1,idata.e2);
    bump_normal = bumpmap->getColorAt(coords);
    bump_normal = normalize(faceforward(bump_normal,incoming_ray.getDirection(),normal));

    normal = normalize(normal+bump_normal);
  }

  // For each light source in the scene
  for(unsigned int i=0; i<lights->size(); ++i) {
    ILight* light  = lights->at(i);
    Ray light_ray = Ray::generateRay(light->getPosition(), idata.interPoint);
    IAccDataStruct::IntersectionData light_idata = datastruct->findClosestIntersection(light_ray);

    float distance_to_light = length(idata.interPoint - light->getPosition()); // 1
    float distance_between_light_and_first_hit = length(light_idata.interPoint - light->getPosition()); // 2

    if (//light_idata.material != IAccDataStruct::IntersectionData::NOT_FOUND
         (distance_between_light_and_first_hit + 0.0001f) < distance_to_light) {
      // IN SHADOW!
      //diffuse = vec3(1,0,0);
    } else {

      // Falloff intensity
      float intensity = light->getIntensity(distance_to_light);

      //Diffuse
      diffuse += intensity
          * clamp(glm::dot(-light_ray.getDirection(), normal))
          * light->getColor();

      if(material->getDiffuseMap() == -1) {
        diffuse *= material->getDiffuse();
      } else {
        diffuse *= texture_color;
      }

      //Specular
      vec3 h = normalize(-incoming_ray.getDirection() - light_ray.getDirection());
      specular += intensity
          * glm::pow( clamp( glm::dot(normal, h) ), material->getShininess() )
      * material->getSpecular()
      * light->getColor();
    }


    // For each light, add ambient component
    ambient += light->getColor();
  }

  // Multiply the accumelated ambient light colors with ambient material color
  ambient *= material->getAmbient();

  if (depth < max_recursion_depth) {

	// är -1 på väg in
	// är 1 på väg ut
    float refraction_sign = glm::sign(glm::dot(normal, incoming_ray.getDirection()));
   
    // TODO 
    // reflective bounce innuti också, multiplicera normal med refr_sign


    // REFLECTION RAY
    if(material->getReflection() > 0.0f /*&& refraction_sign<0.0f*/) {
      vec3 refl_normal = -normal*refraction_sign;
      Ray refl_ray = Ray(idata.interPoint, glm::reflect(incoming_ray.getDirection(), refl_normal ));
      refl_ray = Ray( refl_ray.getPosition() + refl_normal*0.001f , refl_ray.getDirection() );
      refl_color = tracePrim(refl_ray, depth+1) * material->getReflection() ;//* vec4((vec3(1,1,1)-material->getDiffuse()),1);
    }

    // REFRACTION RAY
    if(material->getOpacity() < 1.0f) {

      float eta = material->getIndexOfRefraction();
      if(refraction_sign > 0.0f)
        eta = 1/eta;

      vec3 refr_normal = -normal*refraction_sign;
      vec3 offset = glm::normalize(normal) * refraction_sign * 0.001f;
      vec3 refr_dir = glm::refract(incoming_ray.getDirection(), refr_normal, eta);
      Ray refr_ray = Ray(idata.interPoint + offset, glm::normalize(refr_dir));
      refr_color = tracePrim(refr_ray, depth+1) * (1-material->getOpacity());

    }
  }

  // Summarize all color components
  color += (ambient + diffuse + specular) * (1-material->getReflection()) * material->getOpacity();
  vec4 out_color = vec4(color, material->getOpacity()) + refl_color + refr_color;

  return out_color;
}


}
