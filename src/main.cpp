#include <GL/glew.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <IL/il.h>

#include "io/ExporterImpl/PNGExporter.h"
#include "io/ImporterImpl/OBJImporter.h"
#include "raytracer/Renderer.h"
#include "raytracer/scene/ILight.h"
#include "raytracer/scene/LightImpl/OmniLight.h"
#include "raytracer/AccDataStructImpl/VertexArrayDataStruct.h"
#include "raytracer/utilities/glutil.h"

#include <iostream>
#include <stdlib.h>
#include <GL/glfw.h>
#include <string>
#include <omp.h>
#include <glm/gtc/type_ptr.hpp> //value_ptr

#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367

using namespace std;
using namespace raytracer;

raytracer::Settings settings;
uint8_t* buffer;

GLuint shaderProgram;
raytracer::Camera camera;
vec2 mousePrev;
double prevTime;
int renderMode = 2;

void timedCallback();
void mouse(int button, int action);
void mouseMove(int x, int y);
void mouseMove(int x, int y);
void windowSize(int width, int height);
void initGL();

int main(int argc, char* argv[]) {

  int running = GL_TRUE;

  char* inputFileName, *outputFileName, *settingsFile;

  ilInit();

  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()
      ("help", "produce help message")
      ("input-file", po::value<string>(),"Input file")
      ("output-file", po::value<string>(),"Output file")
      ("no_opengl", "Do not use OpenGL");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
      cout << desc << "\n";
      return 1;
  }

  if(vm.count("input-file")) {
  } else {
  }

  if (vm.count("no_opengl")) {
    cout << "Not using OpenGL" << endl;
    settings.use_opengl = false;
  } else {
      cout << "Using OpenGL.\n";
  }
  /** END TEST**/

  //Initialize GLFW
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  if (argc < 3) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
    std::cout << "Usage is <flags> <infile> <outfile>\n"; // Inform the user of how to use the program
    exit(0);
  }
  // if we got enough parameters...
  else {

    inputFileName = argv[1];
    outputFileName = argv[2];

    settings.width = 50;
    settings.height = 50;
    settings.backgroundColor[0] = 0;
    settings.backgroundColor[1] = 50.0f / 255.0f;
    settings.backgroundColor[2] = 50.0f / 255.0f;
    settings.backgroundColor[3] = 1;

    if(settings.use_opengl) {
      //Open an OpenGl window
      if (!glfwOpenWindow(settings.width, settings.height, 0, 0, 0, 0, 0, 0,
          GLFW_WINDOW)) {
        glfwTerminate();
        exit(EXIT_FAILURE);
      }
      initGL();
      CHECK_GL_ERROR();

      glfwSetMouseButtonCallback(mouse);
      glfwSetMousePosCallback(mouseMove);
    }

    /* IMPORTER
     ***************** */

    raytracer::IImporter* importer = new raytracer::OBJImporter();
    importer->loadFile(inputFileName);
    std::vector<raytracer::Triangle*> triangles = importer->getTriangleList();

    VertexArrayDataStruct vertices;
    if(settings.use_opengl) {
      vertices.setData(triangles);
    }

    /* RENDERER
     ***************** */
    camera.setPosition(vec3(0.2f, 1.0f, 5.0f));
    camera.setDirection(vec3(0.0f, 0.0f, -1.0f));
    camera.setUpVector(vec3(0.0f, 1.0f, 0.0f));

    raytracer::Renderer myRenderer(&settings);
    myRenderer.loadCamera(camera);
    if (!triangles.empty())
      myRenderer.loadTriangles(triangles);

    ILight* lights = new OmniLight(vec3(0, 0, 5));

    lights->setIntensity(40);
    lights->setDistanceFalloff(QUADRATIC);

    myRenderer.loadLights(lights, 1, false);

    buffer = myRenderer.getFloatArray();
    for (int i = 0; i < settings.width * settings.height; i += 3) {
      buffer[i * 4 + 0] = 0;
      buffer[i * 4 + 1] = 50;
      buffer[i * 4 + 2] = 50;
      buffer[i * 4 + 3] = 255;
      buffer[i * 4 + 4] = 0;
      buffer[i * 4 + 5] = 0;
      buffer[i * 4 + 6] = 0;
      buffer[i * 4 + 7] = 255;
      buffer[i * 4 + 8] = 0;
      buffer[i * 4 + 9] = 0;
      buffer[i * 4 + 10] = 0;
      buffer[i * 4 + 11] = 255;
    }

    if(settings.use_opengl) {
      #pragma omp parallel num_threads(omp_get_num_procs()+1)
      {
        if (omp_get_thread_num() != 0)
        {
          omp_set_num_threads(omp_get_num_threads()-1);
          myRenderer.render();
        } else {
          /* WINDOW
           ***************** */
          glfwEnable(GLFW_AUTO_POLL_EVENTS);
          glfwSetWindowSizeCallback(windowSize); // TODO: In settings

          while (running) {
            //OpenGl rendering goes here...d
            glClearColor(settings.backgroundColor.x, settings.backgroundColor.y,
                settings.backgroundColor.z, settings.backgroundColor.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);

            switch (renderMode) {
            case 1: {
              glUseProgram(shaderProgram);
              int loc = glGetUniformLocation(shaderProgram,
                  "modelViewProjectionMatrix");
              mat4 view = camera.getViewMatrix();
              glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(view));

              vertices.draw();
              break;
            }
            case 2:
              glDrawPixels(settings.width, settings.height, GL_RGBA,
                  GL_UNSIGNED_INT_8_8_8_8_REV, buffer);
              break;
            }

            glUseProgram(0);
            CHECK_GL_ERROR();

            //Swap front and back rendering buffers
            glfwSwapBuffers();

            timedCallback();
            glfwSleep(0.01);

            //Check if ESC key was pressed or window was closed
            running = !glfwGetKey(GLFW_KEY_ESC)
                            && glfwGetWindowParam(GLFW_OPENED);
          }
        }
      }
      //Close window and terminate GLFW
      glfwTerminate();
    }
    else {  //Not OpenGL
      #pragma omp parallel
      {
        myRenderer.render();
      }
    }

    /* EXPORTER
     ***************** */
    raytracer::IExporter* exporter = new raytracer::PNGExporter;
    exporter->exportImage(outputFileName, settings.width, settings.height,
        buffer);
    delete importer;
    delete exporter;
    std::cout << std::endl << "end of PROGRAM" << std::endl;
  }

  exit(EXIT_SUCCESS);
}

void initGL() {
  glewInit();
  //startupGLDiagnostics();
  CHECK_GL_ERROR();

  // Workaround for AMD, which hopefully will not be neccessary in the near future...
  if (!glBindFragDataLocation) {
    glBindFragDataLocation = glBindFragDataLocationEXT;
  }

  //                      Create Shaders
  //************************************
  // The loadShaderProgram and linkShaderProgam functions are defined in glutil.cpp and
  // do exactly what we did in lab1 but are hidden for convenience
  shaderProgram = loadShaderProgram("simple.vert", "simple.frag");
  glBindAttribLocation(shaderProgram, 0, "position");
  glBindAttribLocation(shaderProgram, 1, "color");
  glBindAttribLocation(shaderProgram, 2, "texCoordIn");
  glBindFragDataLocation(shaderProgram, 0, "fragmentColor");
  linkShaderProgram(shaderProgram);

  //************************************
  //        Set uniforms
  //************************************

  glUseProgram(shaderProgram);
  CHECK_GL_ERROR();

  // Get the location in the shader for uniform tex0
  int texLoc = glGetUniformLocation(shaderProgram, "colortexture");
  // Set colortexture to 0, to associate it with texture unit 0
  glUniform1i(texLoc, 0);
}

void windowSize(int width, int height) {
  float _width = (float) width / settings.width;
  float _height = (float) height / settings.height;
  glPixelZoom(_width, _height);
  glViewport(0, 0, width, height);
}

void mouseMove(int x, int y) {
  vec2 pos = vec2(x, y);
  if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
    vec2 diff = pos - mousePrev;
    camera.rotate(0.5f * diff);
    mousePrev = pos;
  }
}
void mouse(int button, int action) {
  int x, y;
  glfwGetMousePos(&x, &y);
  vec2 pos = vec2(x, y);
  if (action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      mousePrev = pos;
    }
  }
  //mouseMove(pos.x,pos.y);
}
void timedCallback() {
  double diffTime = glfwGetTime() - prevTime;
  prevTime += diffTime;

  double speed = diffTime * 10.0;
  if (glfwGetKey('W')) {
    camera.translate(vec3(speed, 0, 0));
  }
  if (glfwGetKey('S')) {
    camera.translate(vec3(-speed, 0, 0));
  }
  if (glfwGetKey('D')) {
    camera.translate(vec3(0, speed, 0));
  }
  if (glfwGetKey('A')) {
    camera.translate(vec3(0, -speed, 0));
  }
  if (glfwGetKey(' ')) {
    camera.translate(vec3(0, 0, speed));
  }
  if (glfwGetKey('Z')) {
    camera.translate(vec3(0, 0, -speed));
  }
  if (glfwGetKey('1')) {
    renderMode = 1;
  }
  if (glfwGetKey('2')) {
    renderMode = 2;
  }
  if (glfwGetKey('3')) {
    renderMode = 3;
  }
  if (glfwGetKey('4')) {
    renderMode = 4;
  }
  if (glfwGetKey('5')) {
    renderMode = 5;
  }
  if (glfwGetKey('6')) {
    renderMode = 6;
  }
}
