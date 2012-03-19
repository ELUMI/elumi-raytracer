#include <GL/glew.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/algorithm/string.hpp>

#include "io/ExporterImpl/PNGExporter.h"
#include "io/ImporterImpl/OBJImporter.h"
#include "raytracer/Renderer.h"
#include "raytracer/scene/ILight.h"
#include "raytracer/scene/LightImpl/OmniLight.h"
#include "raytracer/utilities/glutil.h"

#include "pugixml.hpp"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <GL/glfw.h>
#include <string>
#include <sstream>
#include <vector>
#include <omp.h>
#include <glm/gtc/type_ptr.hpp> //value_ptr
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367

using namespace std;
using namespace raytracer;

raytracer::Settings settings;
float* buffer;

GLuint shader_program;
raytracer::Camera camera;
vec2 mousePrev;
double prevTime;
int renderMode = 2;
raytracer::Renderer* myRenderer;

void timedCallback();
void mouse(int button, int action);
void mouseMove(int x, int y);
void mouseMove(int x, int y);
void windowSize(int width, int height);
void initGL();
void getSettings(int argc, char *argv[]);

unsigned int win_width, win_height;
string inputFileName, outputFileName;

int main(int argc, char* argv[]) {
  int running = GL_TRUE;
  getSettings(argc, argv);
  cout << "OpenGL version: " << settings.opengl_version;

  if (settings.opengl_version) {
    win_width = settings.width*4;
    win_height = settings.height*4;

    glfwInit();
    //Open an OpenGl window
    if (!glfwOpenWindow(win_width, win_height, 0, 0, 0, 0, 0, 0,
        GLFW_WINDOW)) {
      cerr << "Failed to open window";
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    initGL();
    CHECK_GL_ERROR();

    glfwSetMouseButtonCallback(mouse);
    glfwSetMousePosCallback(mouseMove);
  } else {
    cout << "Not using OpenGL" << endl;
  }

  /* IMPORTER
   ***************** */

  raytracer::IImporter* importer = new raytracer::OBJImporter();
  importer->loadFile(inputFileName.c_str());
  std::vector<raytracer::Triangle*> triangles = importer->getTriangleList();
  std::vector<raytracer::Material*> materials = importer->getMaterialList();
  std::vector<raytracer::Texture*> textures   = importer->getTextures();


  /* RENDERER
   ***************** */

  camera.set(vec3(0,0,0), vec3(0,0,0), vec3(0,0,0), 0.7845f, settings.width/settings.height);
  camera.setPosition(vec3(0,10.0f,5.5f));
  camera.setDirection(normalize(vec3(0.0f, -1.4f, -1.0f)));
  camera.setUpVector(vec3(0.0f, 1.0f, 0.0f));


  OmniLight* lights = new OmniLight(vec3(5, 7, 5.5f));
  lights->setIntensity(300);
  lights->setColor(vec3(1,1,1));
  lights->setDistanceFalloff(QUADRATIC);

  /*OmniLight* light2 = new OmniLight(vec3(3, 2, -5));
  light2->setIntensity(15);
  light2->setColor(vec3(1, 1, 1));
  light2->setDistanceFalloff(QUADRATIC);

  OmniLight* light3 = new OmniLight(vec3(0, -5, 0));
  light3->setIntensity(15);
  light3->setColor(vec3(1, 1, 1));
  light3->setDistanceFalloff(QUADRATIC);

  OmniLight* light4 = new OmniLight(vec3(0, 5, 0));
  light4->setIntensity(15);
  light4->setColor(vec3(1, 1, 1));
  light4->setDistanceFalloff(QUADRATIC);*/

  myRenderer = new Renderer(&settings);
  myRenderer->loadCamera(camera);
  if (!triangles.empty()) {
    myRenderer->getScene().loadMaterials(materials); //load materials BEFORE triangles!
    myRenderer->loadTriangles(triangles);
    myRenderer->getScene().loadTextures(textures);
  }

  myRenderer->loadLights(lights, 1, false);
  //myRenderer->loadLights(light2, 1, false);
//  myRenderer->loadLights(light3, 1, false);
//  myRenderer->loadLights(light4, 1, false);


  buffer = myRenderer->getColorBuffer();
  for (int i = 0; i < settings.width * settings.height-3; i += 3) {
    buffer[i * 4 + 0] = 1;
    buffer[i * 4 + 1] = 0;
    buffer[i * 4 + 2] = 1;
    buffer[i * 4 + 3] = 1;
    buffer[i * 4 + 4] = 0;
    buffer[i * 4 + 5] = 0;
    buffer[i * 4 + 6] = 0;
    buffer[i * 4 + 7] = 1;
    buffer[i * 4 + 8] = 0;
    buffer[i * 4 + 9] = 0;
    buffer[i * 4 + 10] = 0;
    buffer[i * 4 + 11] = 1;
  }

  if (!settings.opengl_version) {
    myRenderer->render();
  } else {
    myRenderer->asyncRender();

    /* WINDOW
     ***************** */

    glfwEnable(GLFW_AUTO_POLL_EVENTS);
    glfwSetWindowSizeCallback(windowSize); // TODO: In settings

    while (running) {
      //OpenGl rendering goes here...d
      glViewport(0, 0, win_width, win_height);

      glClearColor(settings.background_color.x, settings.background_color.y,
          settings.background_color.z, settings.background_color.a);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glEnable(GL_DEPTH_TEST);
      glDisable(GL_CULL_FACE);


      switch (renderMode) {
      case 1: {
        mat4 view = camera.getViewMatrix();
        IDraw* drawables[] = { myRenderer->getScene().getDrawable(), lights };
        if(settings.opengl_version >= 3) {
          glUseProgram(shader_program);
          int loc = glGetUniformLocation(shader_program, "modelViewProjectionMatrix");
          for(int i=0; i<sizeof(drawables)/sizeof(IDraw*); ++i) {
            drawables[i]->drawWithView(view,loc);
          }
          glUseProgram(0);
        } else if (settings.opengl_version < 3) {
          myRenderer->getScene().getDrawable()->drawWithGLView(view);
          for(int i=0; i<sizeof(drawables)/sizeof(IDraw*); ++i) {
            drawables[i]->drawWithGLView(view);
          }
        }
        break;
      }
      case 2:
        glRasterPos2f(-1,-1);
        glPixelZoom((float) win_width / settings.width, (float) win_height / settings.height);
        glDrawPixels(settings.width, settings.height, GL_RGBA,
            GL_FLOAT, buffer);
        break;
      }

      CHECK_GL_ERROR();

      //Swap front and back rendering buffers
      glfwSwapBuffers();

      timedCallback();
      glfwSleep(0.01);

      //Check if ESC key was pressed or window was closed
      running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }

    //Close window and terminate GLFW
    glfwTerminate();
  }
  /* EXPORTER
   ***************** */
  if (myRenderer->renderComplete() == 0) {
    raytracer::IExporter* exporter = new raytracer::PNGExporter;
    exporter->exportImage(outputFileName.c_str(), settings.width, settings.height, buffer);
    delete exporter;
  }
  delete importer;
  delete myRenderer;
  std::cout << std::endl << "end of PROGRAM" << std::endl;

  exit(EXIT_SUCCESS);
}

void getSettings(int argc, char *argv[]) {
  // Initial values.
  settings.width = 150;
  settings.height = 150;
  settings.background_color[0] = 0;
  settings.background_color[1] = 50.0f / 255.0f;
  settings.background_color[2] = 50.0f / 255.0f;
  settings.background_color[3] = 0;

  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "produce help message")("no_opengl",
      "Do not use OpenGL")("input-file,i", po::value<string>(), "Input file")(
      "output-file,o", po::value<string>(), "Output file")("settings-file,s",
      po::value<string>(), "Settings file");
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
  if (vm.count("help")) {
    cout << desc << "\n";
    exit(1);
  }
  if (vm.count("settings-file")) {
    string line;
    ifstream settings_stream(vm["settings-file"].as<string> ().c_str());
    if (settings_stream.is_open()) {
      while (settings_stream.good()) {
        getline(settings_stream, line);
        vector<string> strs;
        boost::split(strs, line, boost::is_any_of(":")); // Line may show error in eclipse but it should compile anyhow.
        string option = boost::trim_copy(strs[0]);
        string value = boost::trim_copy(strs[1]);
        cout << "Using setting: " << option
             << "\t\twith value: " << value << endl;
        stringstream ssvalue(value);
        if (option == "width") {
          ssvalue >> settings.width;
        } else if (option == "height") {
          ssvalue >> settings.height;
        } else if (option == "opengl_version") {
          ssvalue >> settings.opengl_version;
        } else if (option == "background_color") {
          //TODO Handle backgroudn color
        } else if (option == "use_first_bounce") {
          ssvalue >> settings.use_first_bounce;
        } else {
          cout << "Unknown option: " << option << endl;
        }
      }
    }
  }

  if (vm.count("input-file")) {
    inputFileName = vm["input-file"].as<string> ();
  } else {
    cout << "No input file! Use raytracer --help to see command options.";
    exit(1);
  }
  if (vm.count("output-file")) {
    outputFileName = vm["output-file"].as<string> ();
  } else {
    cout << "Saving file to default destination (out.png)." << endl;
    outputFileName = "out.png";
  }
  if (vm.count("no_opengl")) {
    cout << "Not using OpenGL" << endl;
    settings.opengl_version = 0;
  } else {
    cout << "Using OpenGL.\n";
  }
}

void initGL() {
  if(settings.opengl_version == 2) {
    return;
  }
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
  shader_program = loadShaderProgram("simple.vert", "simple.frag");
  glBindAttribLocation(shader_program, 0, "position");
  glBindAttribLocation(shader_program, 1, "color");
  glBindAttribLocation(shader_program, 2, "normal");
  glBindAttribLocation(shader_program, 3, "texCoordIn");
  glBindAttribLocation(shader_program, 4, "material");

  glBindFragDataLocation(shader_program, 0, "fragmentColor");
  linkShaderProgram(shader_program);

  //************************************
  //        Set uniforms
  //************************************

  glUseProgram(shader_program);
  CHECK_GL_ERROR();

  // Get the location in the shader for uniform tex0
  int texLoc = glGetUniformLocation(shader_program, "colortexture");
  // Set colortexture to 0, to associate it with texture unit 0
  glUniform1i(texLoc, 0);
}

void windowSize(int width, int height) {
  win_width=width;
  win_height=height;
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
  if (glfwGetKey('R')) {
    myRenderer->loadCamera(camera);
    myRenderer->asyncRender();
    renderMode = 2;
  }
  if (glfwGetKey('T')) {
    myRenderer->stopRendering();
  }
  if (glfwGetKey('F') && settings.opengl_version >= 3) {
    myRenderer->loadCamera(camera);
    myRenderer->stopRendering();
    settings.use_first_bounce = true;
    myRenderer->asyncRender();
    renderMode = 2;
  }
  if (glfwGetKey('G')) {
    myRenderer->loadCamera(camera);
    myRenderer->stopRendering();
    settings.use_first_bounce = false;
    myRenderer->asyncRender();
    renderMode = 2;
  }
  if (glfwGetKey(GLFW_KEY_KP_ADD)) {
    myRenderer->loadCamera(camera);
    myRenderer->stopRendering();
    settings.test += speed/100;
    myRenderer->asyncRender();
    renderMode = 2;
    cout << settings.test << "\n";
  }
  if (glfwGetKey(GLFW_KEY_KP_SUBTRACT)) {
    myRenderer->loadCamera(camera);
    myRenderer->stopRendering();
    settings.test -= speed/100;
    myRenderer->asyncRender();
    renderMode = 2;
    cout << settings.test << "\n";
  }
  if (glfwGetKey(GLFW_KEY_KP_MULTIPLY)) {
    myRenderer->loadCamera(camera);
    myRenderer->stopRendering();
    settings.debug_mode += 1;
    myRenderer->asyncRender();
    renderMode = 2;
    cout << settings.debug_mode << "\n";
    glfwSleep(0.5);
  }
  if (glfwGetKey(GLFW_KEY_KP_DIVIDE)) {
    myRenderer->loadCamera(camera);
    myRenderer->stopRendering();
    settings.debug_mode -= 1;
    myRenderer->asyncRender();
    renderMode = 2;
    cout << settings.debug_mode << "\n";
    glfwSleep(0.5);
  }


}
