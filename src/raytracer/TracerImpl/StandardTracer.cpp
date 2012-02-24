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
  int x_coord,y_coord;

  if(material->getTexture() != -1) {

    Texture* texture = scene->getTextureAt(material->getTexture());


    vec3 p = idata.interPoint;
    vec2 texCoords = vec2(0,0);

    vec3 bump_normal = vec3(0,0,0);

    float x;
    float y;

    //Planar mapping
    if(idata.uvcoords.size() ==  0) {
      x = p.x;
      y = p.y;

      if(x > y) {
        x = p.y;
        y = p.x;
      }
      if(y > p.z) {
        y = p.z;
      }

      texCoords.x = x;
      texCoords.y = y;

    } else { //We have texture coordinates
      texCoords.x = idata.texcoord.x;
      texCoords.y = idata.texcoord.y;
    }

    texCoords.x = (int)(texCoords.x*(texture->getWidth()))%texture->getWidth();
    texCoords.y = (int)(texCoords.y*(texture->getHeight()))%texture->getHeight();

    x_coord = (int)texCoords.x;
    y_coord = (int)texCoords.y;

    texture_color = texture->getColorAt(x_coord,y_coord)/255.0f;

    //cout << "texture color x: " << texture_color.x << " y: " << texture_color.y << " z: " << texture_color.z << endl;

  }
  // For each light source in the scene
  for(int i=0; i<lights->size(); ++i) {
    ILight* light  = lights->at(i);
    Ray light_ray = Ray::generateRay( idata.interPoint, light->getPosition() ) ;
    IAccDataStruct::IntersectionData light_idata = datastruct->findClosestIntersection(light_ray);

    float distance_to_light = length(light->getPosition()-idata.interPoint);

    // If light reaches triangle:
    if (//light_idata.triangle != idata.triangle &&
        distance_to_light < length(light->getPosition()-light_idata.interPoint)) {

      // Falloff intensity
      float intensity = light->getIntensity(distance_to_light);

      if(material->getBumpMap() != -1) {
        Texture* bump_map = scene->getTextureAt(material->getBumpMap());
        bump_normal = normalize(bump_map->getColorAt(x_coord,y_coord));

        /*vec3 up_vector = vec3(0,0,1);
        vec3 par_vector = normalize(cross(idata.normal,up_vector));
        bump_normal = -dot(bump_normal,par_vector)*par_vector;*/
      }

      // Diffuse lighting
      diffuse += intensity
                 * clamp(glm::dot(light_ray.getDirection(), idata.normal))
                 * material->getDiffuse()
                 * light->getColor();

      //Texture
      texture_color = intensity * clamp(glm::dot(light_ray.getDirection(), bump_normal)) * vec3(127,127,127)/255.0f
                     * light->getColor();

      cout << "norm x:" << idata.normal.x << " y: " << idata.normal.y << " z: " << idata.normal.z << endl;
      cout << "bump x:" << bump_normal.x << " y: " << bump_normal.y << " z: " << bump_normal.z << endl;

      // Specular lighting
      vec3 h = normalize(light_ray.getDirection() + incoming_ray.getDirection());
      specular += intensity
                  * glm::pow( clamp( glm::dot(idata.normal, h) ), material->getShininess() )
                  * material->getSpecular()
                  * light->getColor();
    }

    // For each light, add ambient component
    ambient += light->getColor();
  }

  // Multiply the accumelated ambient light colors with ambient material color
  ambient *= material->getAmbient();

//  if (depth < max_recursion_depth) {
//    if(material->getReflection() > 0.0f) {
//      Ray refl_ray = Ray::reflection(incoming_ray, idata.normal, idata.interPoint);
//      refl_color = tracePrim(refl_ray, depth+1) * material->getReflection();
//    }
//
//    if(material->getTransparency() != 1.0f) {
//      Ray refr_ray = Ray::refraction(incoming_ray, idata.normal, idata.interPoint,
//                             material->getIndexOfRefraction());
//      refr_color = tracePrim(refr_ray, depth+1) * material->getReflection();
//    }
//  }

  // Summarize all color components
  color += (texture_color+specular);
  return vec4( color, material->getTransparency()) + refl_color + refr_color;
}


}
