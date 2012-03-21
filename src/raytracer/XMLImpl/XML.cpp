/*
 * XML.cpp
 *
 *  Created on: Mar 19, 2012
 *      Author: david
 */

#include "XML.h"

using namespace pugi;
using namespace glm;

namespace raytracer {

IImporter* importer;

XML::XML() {
  // TODO Auto-generated constructor stub
  importer = new OBJImporter();
}

IXML::~IXML() {}

Scene XML::importScene(const char* fileName) {

  Settings settings;

  Scene scene = Scene(&settings);

  xml_document doc;
  pugi::xml_parse_result result = doc.load_file(fileName);

  //Load objects
  for (pugi::xml_node obj = doc.child("Object"); obj; obj = obj.next_sibling("Object"))
  {
    const char* fileName = obj.attribute("fileName").value();
    importer->loadFile(fileName);
    std::vector<raytracer::Triangle*> triangles = importer->getTriangleList();
    std::vector<raytracer::Material*> materials = importer->getMaterialList();
    std::vector<raytracer::Texture*> textures   = importer->getTextures();

    if (!triangles.empty()) {
      scene.loadMaterials(materials,true); //load materials BEFORE triangles!
      scene.loadTriangles(triangles,true);
      scene.loadTextures(textures,true);
    }
  }
  //Load lights
  for (pugi::xml_node light = doc.child("Light"); light; light = light.next_sibling("Light"))
  {

    ILight* newLight = NULL;
    string type = light.attribute("type").value();

    if(type.compare("PointLight") == 0) {
      newLight = new OmniLight();
    }

    if(newLight != NULL) {

      xml_node position = light.child("Position");
      xml_node color = light.child("Color");

      vec3 pos = vec3(position.attribute("x").as_float(), position.attribute("y").as_float(),
          position.attribute("z").as_float());

      vec3 col = vec3(color.attribute("r").as_float(), color.attribute("g").as_float(),
          color.attribute("b").as_float());

      newLight->setIntensity(light.attribute("intensity").as_float());
      newLight->setColor(col);
      newLight->setPosition(pos);

      //Falloff
      string falloff = light.attribute("falloff").value();

      if(falloff.compare("Quadratic") == 0) {
        newLight->setDistanceFalloff(QUADRATIC);
      } else if(falloff.compare("Linear") == 0) {
        newLight->setDistanceFalloff(LINEAR);
      }

      scene.loadLights(newLight,1,true);
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

    scene.loadCamera(cam);
  }

  return scene;
}

void XML::exportScene(Scene scene, const char* fileName) {

}

}


