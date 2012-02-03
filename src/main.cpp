#include "io/ExporterImpl/PNGExporter.h"
#include "io/ImporterImpl/OBJImporter.h"
#include "raytracer/Renderer.h"
#include "raytracer/scene/ILight.h"
#include "raytracer/scene/LightImpl/OmniLight.h"

#include <iostream>
#include <stdlib.h>
#include <GL/glfw.h>
#include <string>
#include <omp.h>

#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367

using namespace std;
using namespace raytracer;

void GLFWCALL OnWindowSize(int width,int height);
raytracer::Settings settings;
uint8_t* buffer;

int main(int argc, char* argv[]) {

  int running = GL_TRUE;

  //Initialize GLFW
  if(!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  if (argc < 3) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
    std::cout << "Usage is <flags> <infile> <outfile>\n"; // Inform the user of how to use the program
    exit(0);
  }
  // if we got enough parameters...
  else {
    char* inputFileName, *outputFileName, *settingsFile;
    inputFileName  = argv[1];
    outputFileName = argv[2];

    settings.width = 320;
    settings.height = 240;
    settings.backgroundColor[0] = 0;
    settings.backgroundColor[1] = 50.0f/255.0f;
    settings.backgroundColor[2] = 50.0f/255.0f;
    settings.backgroundColor[3] = 1;

    /* IMPORTER
     ***************** */

    raytracer::IImporter* importer = new raytracer::OBJImporter();
    importer->loadFile(inputFileName);
    std::vector<raytracer::Triangle*> triangles = importer->getTriangleList();

    /* RENDERER
     ***************** */
    raytracer::Camera camera;
    camera.setPosition(vec3(0.0f, 0.0f, 7.0f));
    camera.setDirection(vec3(0.0f,0.0f,-1.0f));
    camera.setUpVector(vec3(0.0f,1.0f,0.0f));

    raytracer::Renderer myRenderer(&settings);
    myRenderer.loadCamera(camera);
    if (!triangles.empty())
      myRenderer.loadTriangles(triangles);

    ILight* lights = new OmniLight(vec3(0,0,5));

    lights->setIntensity(40);
    lights->setDistanceFalloff(QUADRATIC);

    myRenderer.loadLights(lights,1,false);


    buffer = myRenderer.getFloatArray();
    #pragma omp parallel
    {
      #pragma omp sections
      {
        // THREAD 1
        {
          myRenderer.render();
        }

        // THREAD 2
        #pragma omp section
        {
          /* WINDOW
           ***************** */

          //Open an OpenGl window
          if(!glfwOpenWindow(settings.width,settings.height,0,0,0,0,0,0,GLFW_WINDOW)) {
            glfwTerminate();
            exit(EXIT_FAILURE);
          }

          glfwEnable(GLFW_AUTO_POLL_EVENTS);
          glfwSetWindowSizeCallback(OnWindowSize); // TODO: In settings

          while(running) {
            //OpenGl rendering goes here...d
            glClearColor(settings.backgroundColor.x,settings.backgroundColor.y,settings.backgroundColor.z
                ,settings.backgroundColor.a);
            glClear(GL_COLOR_BUFFER_BIT);

            glDrawPixels(settings.width,settings.height,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8_REV,buffer);

            //Swap front and back rendering buffers
            glfwSwapBuffers();

            //Check if ESC key was pressed or window was closed
            running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
          }
        }
      }
    }

    //Close window and terminate GLFW
    glfwTerminate();

    /* EXPORTER
     ***************** */
    raytracer::IExporter* exporter = new raytracer::PNGExporter;
    exporter->exportImage(outputFileName,settings.width,settings.height,buffer);
    delete importer;
    delete exporter;
    std::cout << std::endl << "end of PROGRAM" << std::endl;
  }

  exit(EXIT_SUCCESS);
}
void GLFWCALL OnWindowSize(int width,int height){
  float _width = (float)width/settings.width;
  float _height = (float)height/settings.height;
  glPixelZoom(_width,_height);
}
