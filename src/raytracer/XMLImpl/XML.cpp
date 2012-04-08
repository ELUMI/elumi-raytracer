/*
 * XML.cpp
 *
 *  Created on: Mar 19, 2012
 *      Author: david
 */

#include "XML.h"

using namespace pugi;
using namespace glm;

#include <IL/il.h>
#include <IL/ilu.h>

#include "../scene/Camera.h"
#include "../Settings.h"
#include "../scene/ILight.h"
#include "../scene/LightImpl/BaseLight.h"
#include "../scene/LightImpl/AreaLight.h"
#include "../EnvironmentMapImpl/CubeMap.h"
#include <exception>
#include <iostream>
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

  ilInit();
  iluInit();

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
      xml_node screen       = settings_doc.child("Screen");
      xml_node tracer       = settings_doc.child("Tracer");
      xml_node recursion    = settings_doc.child("Recursion");
      xml_node threading    = settings_doc.child("Threading");
      xml_node tonemapping  = settings_doc.child("Tonemapping");
      xml_node tree         = settings_doc.child("Tree");
      xml_node wireframe    = settings_doc.child("Wireframe");
      xml_node supersampling= settings_doc.child("Supersampling");
      xml_node photonmapper = settings_doc.child("Photonmapper");

      if(screen) {
        settings->width = screen.attribute("width").as_int();
        settings->height = screen.attribute("height").as_int();
      }
      if(tracer) {
        settings->tracer = tracer.attribute("version").as_int();

        if(!tracer.attribute("pattern").empty())
          settings->pattern = tracer.attribute("pattern").as_int();
        if(!tracer.attribute("batches").empty())
          settings->batches = tracer.attribute("batches").as_int();
      }
      if(recursion) {
        settings->max_recursion_depth = recursion.attribute("maxDepth").as_int();
        settings->recursion_attenuation_threshold = recursion.attribute("attenuationThreshold").as_float();
      }
      if(threading) {
        settings->threads = threading.attribute("threads").as_int();
      }
      if(tonemapping) {
        if(!tonemapping.attribute("key").empty())
          settings->key = tonemapping.attribute("key").as_float();

        if(!tonemapping.attribute("white").empty())
          settings->white = tonemapping.attribute("white").as_float();
      }
      if(tree){
        settings->tree = tree.attribute("version").as_int();
      }
      if(wireframe){
        settings->wireframe = wireframe.attribute("enable").as_int();
      }
      if(supersampling){
        settings->samples = supersampling.attribute("samples").as_int();
        settings->super_sampler_pattern = supersampling.attribute("pattern").as_int();
      }
      if(photonmapper){
        if(!photonmapper.attribute("photons").empty())
          settings->photons = photonmapper.attribute("photons").as_int();
        if(!photonmapper.attribute("radius").empty())
          settings->gather_radius = photonmapper.attribute("radius").as_float();
        if(!photonmapper.attribute("photonmap_size").empty())
          settings->photonmap_size = photonmapper.attribute("photonmap_size").as_int();
      }

      if(settings->opengl_version<3){
        settings->wireframe = 0;
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
      newLight = new BaseLight();

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


  //Load environment map
  for (pugi::xml_node env = doc.child("Environment"); env; env = env.next_sibling("Environment"))
  {

    IEnvironmentMap* newEnv = NULL;
    string type = env.attribute("type").value();

    if(type.compare("Cube") == 0) {
      string srcs[] = {"","","","","",""};
      srcs[0] = env.child("Top").attribute("src").value();
      srcs[1] = env.child("Bottom").attribute("src").value();
      srcs[2] = env.child("Front").attribute("src").value();
      srcs[3] = env.child("Right").attribute("src").value();
      srcs[4] = env.child("Back").attribute("src").value();
      srcs[5] = env.child("Left").attribute("src").value();

      ILuint image;
      Texture** textures = new Texture*[6];
      for(int i=0; i<6; ++i) {
        if(srcs[i].empty())
          throw fnf_exception;


        image = ilGenImage();
        ilBindImage(image);
        ilLoadImage((const ILstring)srcs[i].c_str());

        if(ilGetError() == IL_NO_ERROR) {

          // Fix rotation
          if(i==0) {
            iluRotate(180);
          }
          if(i==2) {
            iluRotate(180);
          }
          if(i==3) {
            iluRotate(90);
          }
          if(i==4) {
            iluRotate(180);
            iluMirror();
          }
          if(i==5) {
            iluRotate(270);
            iluMirror();
          }

          ILuint w = ilGetInteger(IL_IMAGE_WIDTH);
          ILuint h = ilGetInteger(IL_IMAGE_HEIGHT);

          textures[i] = new Texture();
          textures[i]->setData(w,h,ilGetData(),Texture::TEXTURE);
        } else {
          cout << "XML.cpp: Environtment map image not loaded.\n" << endl;
        }
      }
      newEnv = new CubeMap(textures, 6);
    }

    if(newEnv != NULL)
      scene->setEnvirontmentMap(newEnv);
  }


  return scene;
}

void XML::exportScene(Scene scene, const char* fileName) {

}

}
