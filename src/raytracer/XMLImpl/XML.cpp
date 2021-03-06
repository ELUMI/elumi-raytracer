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
#include "../scene/LightImpl/SpotLight.h"
#include "../EnvironmentMapImpl/CubeMap.h"
#include "../EnvironmentMapImpl/SphereMap.h"

#include "../scene/VolumeImpl/UniformVolume.h"
#include "../PhaseFunctorImpl/PhaseIsotropic.h"
#include "../PhaseFunctorImpl/PhaseHG.h"
#include "../PhaseFunctorImpl/PhaseSchlick.h"

#include <exception>
#include <iostream>

using namespace std;

namespace raytracer {

/* Read in light probe */
void loadFloatArray(string filename, const int width, float*** hdr) {
  int swapflag = 0 ;
  int i,j,k ;
  FILE *fp ;
  assert(fp = fopen(filename.c_str(),"rb")) ;

  for (i = 0 ; i < width ; i++)
    for (j = 0 ; j < width ; j++)
      for (k = 0 ; k < 3 ; k++) {

        /* This is a little ugly since the input is in binary format
     and we need to do some tests on endian-ness */

        float val ;
        unsigned char * c = (unsigned char *) &val ;
        if (swapflag) { /* Endianness of computer different from input
         Read in the bytes in reversed order.
         */
          assert(fscanf(fp,"%c%c%c%c",&(c[3]),&(c[2]),&(c[1]),&(c[0]))==4);
        }
        else { /* Endianness same as input */
          assert(fscanf(fp,"%c%c%c%c",&(c[0]),&(c[1]),&(c[2]),&(c[3]))==4);
        }
        hdr[i][j][k] = val ; /* Remember that c pointed to val */

      }
  fclose(fp) ;
}

class file_not_found: public exception
{
  const virtual char* what() const throw () {
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

void XML::getSettings(const char* xml_file, Settings* settings) {
  xml_document settings_doc;
  pugi::xml_parse_result result = settings_doc.load_file(xml_file);
  if (!result) {
    throw fnf_exception;
  }

  xml_node screen = settings_doc.child("Screen");
  xml_node tracer = settings_doc.child("Tracer");
  xml_node recursion = settings_doc.child("Recursion");
  xml_node threading = settings_doc.child("Threading");
  xml_node tonemapping = settings_doc.child("Tonemapping");
  xml_node tree = settings_doc.child("Tree");
  xml_node wireframe = settings_doc.child("Wireframe");
  xml_node supersampling = settings_doc.child("Supersampling");
  xml_node photonmapper = settings_doc.child("Photonmapper");
  xml_node volume = settings_doc.child("Volume");
  if (screen) {
    settings->width = screen.attribute("width").as_int();
    settings->height = screen.attribute("height").as_int();
  }
  if (tracer) {
    settings->tracer = tracer.attribute("version").as_int();
    if (!tracer.attribute("pattern").empty())
      settings->pattern = tracer.attribute("pattern").as_int();

    if (!tracer.attribute("batches").empty())
      settings->batches = tracer.attribute("batches").as_int();

    if (!tracer.attribute("use_fresnel").empty())
      settings->use_fresnel = tracer.attribute("use_fresnel").as_int();

    if (!tracer.attribute("first_bounce").empty())
      settings->use_first_bounce = tracer.attribute("first_bounce").as_bool();
  }
  if (recursion) {
    settings->max_recursion_depth = recursion.attribute("maxDepth").as_int();
    settings->recursion_attenuation_threshold = recursion.attribute(
        "attenuationThreshold").as_float();
  }
  if (threading) {
    settings->threads = threading.attribute("threads").as_int();
  }
  if (tonemapping) {
    if (!tonemapping.attribute("on").empty())
      settings->tonemap = tonemapping.attribute("on").as_bool();

    if (!tonemapping.attribute("key").empty())
      settings->key = tonemapping.attribute("key").as_float();

    if (!tonemapping.attribute("white").empty())
      settings->white = tonemapping.attribute("white").as_float();
  }
  if (tree) {
    settings->tree = tree.attribute("version").as_int();
  }
  if (wireframe) {
    settings->wireframe = wireframe.attribute("enable").as_int();
  }
  if (supersampling) {
    settings->samples = supersampling.attribute("samples").as_int();
    settings->super_sampler_pattern =
        supersampling.attribute("pattern").as_int();
  }
  if (photonmapper) {
    if (!photonmapper.attribute("photonmap").empty())
      settings->photonmap = photonmapper.attribute("photonmap").as_int();

    if (!photonmapper.attribute("photonmap_size").empty())
      settings->photonmap_size =
          photonmapper.attribute("photonmap_size").as_int();

    if (!photonmapper.attribute("photons").empty())
      settings->photons = photonmapper.attribute("photons").as_int();

    if (!photonmapper.attribute("final_gather_samples").empty())
      settings->final_gather_samples =
          photonmapper.attribute("final_gather_samples").as_int();

    if (!photonmapper.attribute("radius").empty())
      settings->gather_radius = photonmapper.attribute("radius").as_float();

    if (!photonmapper.attribute("caustics").empty())
      settings->caustics = photonmapper.attribute("caustics").as_float();

    if (!photonmapper.attribute("scaling").empty())
      settings->photonmap_scaling =
          photonmapper.attribute("scaling").as_float();

    if (!photonmapper.attribute("kernel").empty())
      settings->photon_kernel =
          photonmapper.attribute("kernel").as_int();
  }
  if(volume) {
    if(!volume.attribute("step_size"). empty())
      settings->step_size = volume.attribute("step_size").as_float();
  }
  if (settings->opengl_version < 3) {
    settings->wireframe = 0;
  }
}

Scene* XML::importScene(const char* fileName, const char* settingsFileName) {
  ilInit();
  iluInit();
  xml_document doc;
  pugi::xml_parse_result result = doc.load_file(fileName);
  if (!result) {
    throw fnf_exception;
  }
  Settings* settings = new Settings(); // TODO LÄS IN FRÅN FIL
  settings->opengl_version = open_gl_version;
  //Load settings. If any...
  xml_node xml_settings = doc.child("Settings");
  if(settingsFileName) {
    getSettings(settingsFileName, settings);
  } else if (xml_settings) {
    const char* xml_file = xml_settings.attribute("fileName").value();

    getSettings(xml_file, settings);
  }

  Scene* scene = new Scene(settings);

  //Load objects
  for (pugi::xml_node obj = doc.child("Object"); obj; obj = obj.next_sibling("Object")) {
    const char* fileName = obj.attribute("fileName").value();
    importer->loadFile(fileName);
    std::vector<raytracer::Triangle*> triangles = importer->getTriangleList();
    std::vector<raytracer::Material*> materials = importer->getMaterialList();
    std::vector<raytracer::Texture*> textures   = importer->getTextures();

    if (!triangles.empty()) {
      size_t material_shift = scene->loadMaterials(materials); //load materials BEFORE triangles!
      scene->loadTriangles(triangles,importer->getAABB(), material_shift);
      scene->loadTextures(textures);
    }
  }
  //Load objects
  for (pugi::xml_node vol = doc.child("Volume"); vol; vol = vol.next_sibling("Volume")) {


    // Load phase function for volume
    xml_node phase_node = vol.child("PhaseFunction");

    IPhaseFunctor* functor;
    string pf_type = phase_node.attribute("type").value();
    if(pf_type.compare("isotropic") == 0) {
      functor = new PhaseIsotropic();

    } else if(pf_type.compare("hg") == 0) {
      float g = 0.0f;
      if(!phase_node.attribute("g").empty())
        g = phase_node.attribute("g").as_float();
      functor = new PhaseHG(g);

    } else if(pf_type.compare("schlick") == 0) {
      float k = 0.0f;
      if(!phase_node.attribute("g").empty()){
        k = phase_node.attribute("g").as_float();
        functor = new PhaseSchlick(PhaseSchlick::HGtoSchlick(k));
      } else if(!phase_node.attribute("k").empty()) {
        k = phase_node.attribute("k").as_float();
        functor = new PhaseSchlick(k);
      }

    } else {
      functor = new PhaseIsotropic();
    }


    // Load the volume
    string type = vol.attribute("type").value();
    if(type.compare("uniform") == 0) {

      xml_node pos_node = vol.child("Position");
      xml_node u_node = vol.child("U");
      xml_node v_node = vol.child("V");
      xml_node w_node = vol.child("W");

      vec3 pos, u, v, w;

      if(!pos_node) {
        cerr << "XMLImporter: no valid position!" << endl;
      } else if(!u_node) {
        cerr << "XMLImporter: no valid u vector!" << endl;
      } else if(!v_node) {
        cerr << "XMLImporter: no valid v vector!" << endl;
      } else if(!w_node) {
        cerr << "XMLImporter: no valid w vector!" << endl;
      }
      if(!pos_node || !u_node || !v_node || !w_node)
        break;

      pos = vec3(pos_node.attribute("x").as_float(),
          pos_node.attribute("y").as_float(),
          pos_node.attribute("z").as_float());

      u = vec3(u_node.attribute("x").as_float(),
          u_node.attribute("y").as_float(),
          u_node.attribute("z").as_float());

      v = vec3(v_node.attribute("x").as_float(),
          v_node.attribute("y").as_float(),
          v_node.attribute("z").as_float());

      w = vec3(w_node.attribute("x").as_float(),
          w_node.attribute("y").as_float(),
          w_node.attribute("z").as_float());

      OBB obb = OBB(pos, u, v, w);

      float absorption = vol.attribute("absorption").as_float();
      float scattering = vol.attribute("scattering").as_float();
      float emission = vol.attribute("emission").as_float();

      IVolume* volume = new UniformVolume(obb, absorption, scattering, emission, functor);
      scene->addVolume(volume);

    } else {
      cerr << "XMLImporter: Invalid volume type" << endl;
    }
  }
  //Load lights
  for (pugi::xml_node light = doc.child("Light"); light; light = light.next_sibling("Light"))
  {

    string type = light.attribute("type").value();
    ILight::FalloffType ftype = ILight::NONE;

    //Falloff
    string falloff = light.attribute("falloff").value();

    //visible
    bool visible = light.attribute("visible").as_int();

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


    ILight* newLight = NULL;
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
      // NEJ! reinterpret_cast<AreaLight*>(newLight)->addPlane(scene); //add arealight to datastruct
    } else if(type.compare("Spot") == 0) {
      xml_node dir_node = light.child("Direction");

      vec3 dir = vec3(dir_node.attribute("x").as_float(),
          dir_node.attribute("y").as_float(),
          dir_node.attribute("z").as_float());

      float outer;
      if(!light.attribute("outer").empty())
        outer = light.attribute("outer").as_float();
      else
        outer = -1;

      float inner;
      if(!light.attribute("inner").empty())
        inner = light.attribute("inner").as_float();
      else
        inner = outer;

      if(outer > 0)
        newLight = new SpotLight(pos, dir, inner, outer);
      else
        newLight = new SpotLight(pos, dir);
    }

    if(newLight != NULL) {
      newLight->setIntensity(light.attribute("intensity").as_float());
      newLight->setColor(col);
      newLight->setPosition(pos);
      newLight->setDistanceFalloff(ftype);

      if(type.compare("Area") == 0 && visible) { // || tracer==pathtracer
        reinterpret_cast<AreaLight*>(newLight)->addPlane(scene); //add arealight to datastruct
      }

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
    cam.set(pos, dir, norm, 0.7845f, settings->width/settings->height);

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
          cout << "XML.cpp: Environtment cube map image not loaded.\n" << endl;
        }
      }
      newEnv = new CubeMap(textures, 6);
    }
    else if(type.compare("Sphere") == 0) {
      string filename = env.child("Sphere").attribute("src").value();
      unsigned int width = env.child("Sphere").attribute("radius").as_uint();

      //      ILuint image;
      //      Texture* texture = NULL;
      //      if(filename.empty())
      //        throw fnf_exception;
      //
      //      image = ilGenImage();
      //      ilBindImage(image);
      //
      //      ilLoadImage((const ILstring)filename.c_str());
      //
      //      if(ilGetError() == IL_NO_ERROR) {
      //        ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
      //        iluRotate(180);
      //
      //        ILuint w = ilGetInteger(IL_IMAGE_WIDTH);
      //        ILuint h = ilGetInteger(IL_IMAGE_HEIGHT);
      //
      //        texture = new Texture();
      //        texture->setData(w,h,ilGetData(),Texture::TEXTURE);
      //        newEnv = new SphereMap(texture);
      //      }
      //      else
      //        exit(1);

      float*** hdrdata;
      // Allocate memory
      hdrdata = new float**[width];
      for (int i = 0; i < width; ++i) {
        hdrdata[i] = new float*[width];
        for (int j = 0; j < width; ++j)
          hdrdata[i][j] = new float[3];
      }

      loadFloatArray(filename, width, hdrdata);
      newEnv = new SphereMap(hdrdata,width);


    }


    if(newEnv != NULL)
      scene->setEnvirontmentMap(newEnv);
  }

  // Load volumes

  scene->build(); //build everything we have read in

  return scene;
}

void XML::exportScene(Scene scene, const char* fileName) {

}

}
