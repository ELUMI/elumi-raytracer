/*
 * XML.cpp
 *
 *  Created on: Mar 19, 2012
 *      Author: david
 */

#include "XML.h"

using namespace pugi;
using namespace glm;

#include "../scene/Camera.h"
#include "../Settings.h"
#include "../scene/ILight.h"
#include "../scene/LightImpl/OmniLight.h"
#include "../scene/LightImpl/AreaLight.h"
#include <exception>
using namespace std;

namespace raytracer {

class file_not_found: public exception
{
  virtual const char* what() const throw(){
    return "File not found exception";
  }
} fnf_exception;

IImporter* importer;

XML::XML(int open_gl_version) {
  this->open_gl_version = open_gl_version;
  importer = new OBJImporter();
}

XML::~XML() {
  delete importer;
}

Scene* XML::importScene(const char* fileName) {

  xml_document doc;
  pugi::xml_parse_result result = doc.load_file(fileName);

  if(!result) {
    throw fnf_exception;
  }

  Settings* settings = new Settings(); // TODO LÄS IN FRÅN FIL
  settings->opengl_version = open_gl_version;

  //Load settings. If any...
  xml_node xml_settings = doc.child("Settings");

  if(xml_settings) {
    const char* xml_file = xml_settings.attribute("fileName").value();

    xml_document settings_doc;
    result = settings_doc.load_file(xml_file);

    if(!result) {
      throw fnf_exception;
    } else {
      xml_node screen = settings_doc.child("Screen");
      xml_node tracer = settings_doc.child("Tracer");
      xml_node recursion = settings_doc.child("Recursion");

      if(screen) {
        settings->width = screen.attribute("width").as_int();
        settings->height = screen.attribute("height").as_int();
      }
      if(tracer) {
        settings->tracer = tracer.attribute("version").as_int();
      }
      if(recursion) {
        settings->max_recursion_depth = recursion.attribute("maxDepth").as_int();
        settings->recursion_attenuation_threshold = recursion.attribute("attenuationThreshold").as_float();
      }
    }
  }

  Scene* scene = new Scene(settings);

  //Load objects
  for (pugi::xml_node obj = doc.child("Object"); obj; obj = obj.next_sibling("Object"))
  {
    const char* fileName = obj.attribute("fileName").value();
    importer->loadFile(fileName);
    std::vector<raytracer::Triangle*> triangles = importer->getTriangleList();
    std::vector<raytracer::Material*> materials = importer->getMaterialList();
    std::vector<raytracer::Texture*> textures   = importer->getTextures();

    if (!triangles.empty()) {
      scene->loadMaterials(materials); //load materials BEFORE triangles!
      scene->loadTriangles(triangles,importer->getAABB());
      scene->loadTextures(textures);
    }
  }
  //Load lights
  for (pugi::xml_node light = doc.child("Light"); light; light = light.next_sibling("Light"))
  {

    ILight* newLight = NULL;
    string type = light.attribute("type").value();
    ILight::FalloffType ftype = ILight::NONE;

    //Falloff
    string falloff = light.attribute("falloff").value();

    if(falloff.compare("Quadratic") == 0) {
      ftype = ILight::QUADRATIC;
    } else if(falloff.compare("Linear") == 0) {
      ftype = ILight::LINEAR;
    } else if(falloff.compare("None") == 0) {
      ftype = ILight::NONE;
    }

    //Color and position
    xml_node position = light.child("Position");
    xml_node color = light.child("Color");

    vec3 pos = vec3();
    vec3 col = vec3();

    if(position)
      pos = vec3(position.attribute("x").as_float(), position.attribute("y").as_float(),
        position.attribute("z").as_float());
    if(color)
      col = vec3(color.attribute("r").as_float(), color.attribute("g").as_float(),
        color.attribute("b").as_float());


    if(type.compare("Point") == 0) {
      newLight = new OmniLight();

    } else if(type.compare("Area") == 0) {

      xml_node xml_axis1 = light.child("Axis1");
      xml_node xml_axis2 = light.child("Axis2");

      unsigned int samples1 = xml_axis1.attribute("samples").as_int();
      unsigned int samples2 = xml_axis2.attribute("samples").as_int();

      vec3 axis1 = vec3(xml_axis1.attribute("x").as_float(), xml_axis1.attribute("y").as_float(),
          xml_axis1.attribute("z").as_float());

      vec3 axis2 = vec3(xml_axis2.attribute("x").as_float(), xml_axis2.attribute("y").as_float(),
          xml_axis2.attribute("z").as_float());

      newLight = new AreaLight(pos,axis1,axis2,samples1,samples2);
    }

    if(newLight != NULL) {
      newLight->setIntensity(light.attribute("intensity").as_float());
      newLight->setColor(col);
      newLight->setPosition(pos);
      newLight->setDistanceFalloff(ftype);

      ILight* array[] = {newLight};

      scene->loadLights(array,1);
    }
  }
  //Load camera
  xml_node camera = doc.child("Camera");
  if(camera) {
    xml_node position = camera.child("Position");
    xml_node direction = camera.child("Direction");
    xml_node normal = camera.child("Normal");
    vec3 pos = vec3(position.attribute("x").as_float(), position.attribute("y").as_float(),
        position.attribute("z").as_float());

    vec3 dir = vec3(direction.attribute("x").as_float(), direction.attribute("y").as_float(),
        direction.attribute("z").as_float());
    vec3 norm = vec3(normal.attribute("x").as_float(), normal.attribute("y").as_float(),
        normal.attribute("z").as_float());

    Camera cam = Camera();
    cam.setDirection(dir);
    cam.setPosition(pos);
    cam.setUpVector(norm);

    scene->loadCamera(cam);
  }

  return scene;
}

void XML::exportScene(Scene scene, const char* fileName) {

}

}
