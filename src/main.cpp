#include <GL/glew.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/algorithm/string.hpp>

#include "io/ExporterImpl/PNGExporter.h"
#include "raytracer/Renderer.h"
#include "raytracer/GLData/glutil.h"
#include "raytracer/utilities/Random.h"
#include "raytracer/TracerImpl/PhotonMapper.h"

#include "raytracer/IXML.h"
#include "raytracer/XMLImpl/XML.h"

#include "raytracer/common.hpp"
#include "raytracer/GLData/LineArrayDataStruct.hpp"

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

Settings* settings;
float* buffer;

GLuint shader_program;
raytracer::Camera camera;
vec2 mousePrev;
double prevTime;
int renderMode = 2;
raytracer::Renderer* myRenderer;
bool auto_render = false;

enum DebugVariable { TEST, KEY, WHITE, RADIUS };
DebugVariable var = TEST;

void timedCallback();
void mouse(int button, int action);
void mouseMove(int x, int y);
void mouseMove(int x, int y);
void windowSize(int width, int height);
void initGL();
void getArguments(int argc, char *argv[]);
void drawDrawables(IDraw *drawables[], size_t n);
void drawPoints();
void drawPointsPhoton();
void writePhotonMap();
void readPhotonMap();

int opengl_version = 2;
bool headless=false;
unsigned int win_width, win_height;
string inputFileName, outputFileName;

int main(int argc, char* argv[]) {
  init_generators();
  int running = GL_TRUE;
  getArguments(argc, argv);

  if(!opengl_version)
    headless = true;

  // INIT OPEN GL
  if (!headless) {
    win_width = 50;
    win_height = 50;

    glfwInit();
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
  } else if(!opengl_version) {
      cout << "Not using OpenGL" << endl;
  } else {
    glfwInit();
    //a window is almost necessary for an opengl context
    if (!glfwOpenWindow(1, 1, 0, 0, 0, 0, 0, 0,
        GLFW_WINDOW)) {
      cerr << "Failed to open window";

      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    initGL();
    CHECK_GL_ERROR();
  }
  cout << "OpenGL version: " << opengl_version << "\n";



  // CREATE RENDERER AND LOAD SCENE DATA
  myRenderer = new Renderer(opengl_version);
  myRenderer->loadSceneFromXML(inputFileName.c_str());
  Scene* myScene = myRenderer->getScene();
  settings = myScene->getSettings();
  camera = myScene->getCamera();


//
//  cout << myScene->getEnvironmentMap()->getColor(Ray::generateRay(vec3(0,0,0), vec3(1,0,0))).x;
//  exit(1);

  // RESIZE
  if (!headless) {
    glfwSetWindowSize(settings->width, settings->height);
  }


  // START BUFFER
  buffer = myRenderer->getColorBuffer();
  for (int i = 0; i < settings->width * settings->height-3; i += 3) {
    buffer[i * 4 + 0] = 0;
    buffer[i * 4 + 1] = 0;
    buffer[i * 4 + 2] = 0;
    buffer[i * 4 + 3] = 0;
    buffer[i * 4 + 4] = 0;
    buffer[i * 4 + 5] = 0;
    buffer[i * 4 + 6] = 0;
    buffer[i * 4 + 7] = 0;
    buffer[i * 4 + 8] = 0;
    buffer[i * 4 + 9] = 0;
    buffer[i * 4 + 10] = 0;
    buffer[i * 4 + 11] = 0;
  }

  if (headless) {
    myRenderer->render();
  } else {
    myRenderer->asyncRender();

    /* WINDOW
     ***************** */

    glfwEnable(GLFW_AUTO_POLL_EVENTS);
    glfwSetWindowSizeCallback(windowSize); // TODO: In settings

    IDraw* data_struct_drawable = NULL;
    if(settings->wireframe==1){
      data_struct_drawable= new LineArrayDataStruct(myRenderer->getScene()->getAccDataStruct()->getAABBList());
    }

    while (running) {
      if(auto_render) {
        while(myRenderer->renderComplete())
          timedCallback();

        if(myRenderer->renderComplete() == 0.0f) {
          myRenderer->loadCamera(camera);
          myRenderer->asyncRender();
        }
      }

      //OpenGl rendering goes here...d
      glViewport(0, 0, win_width, win_height);

      glClearColor(settings->background_color.x, settings->background_color.y,
          settings->background_color.z, settings->background_color.a);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glEnable(GL_DEPTH_TEST);
      glDisable(GL_CULL_FACE);

      int light_size = myRenderer->getScene()->getLightVector()->size();

      IDraw* drawables[2+light_size]; //enough size
      int n_drawables = 0;
      drawables[n_drawables++] = myRenderer->getScene()->getDrawable();
      for(int i=0; i<light_size; ++i)
        drawables[n_drawables++] = myScene->getLightVector()->at(i);
      if(settings->wireframe==1)
        drawables[n_drawables++] = data_struct_drawable;
      switch (renderMode) {
      case 1:
        drawDrawables(drawables, n_drawables);
        break;
      case 2:
        glRasterPos2f(-1,-1);
        glPixelZoom((float) win_width / settings->width, (float) win_height / settings->height);
        glDrawPixels(settings->width, settings->height, GL_RGBA, GL_FLOAT, buffer);
        break;
      case 3:
        drawDrawables(drawables, n_drawables);
        drawPoints();
        break;
      case 4:
        drawPoints();
        break;
      case 5:
        drawPointsPhoton();
        break;
      case 6:
        drawPoints();
        drawPointsPhoton();
        break;
      case 7:
        drawDrawables(drawables, n_drawables);
        drawPoints();
        drawPointsPhoton();
        break;
      }

      CHECK_GL_ERROR();

      //Swap front and back rendering buffers
      glfwSwapBuffers();

      timedCallback();
      glfwSleep(1.0f/60.0f);

      //Check if ESC key was pressed or window was closed
      running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }
    myRenderer->stopRendering();

    //Close window and terminate GLFW
    glfwTerminate();
  }

  /* EXPORTER
   ***************** */

  if (myRenderer->renderComplete() == 0.0f) {
    raytracer::IExporter* exporter = new raytracer::PNGExporter;
    exporter->exportImage(outputFileName.c_str(), settings->width, settings->height, buffer);
    delete exporter;
  }

  delete myRenderer;
  std::cout << std::endl << "end of PROGRAM" << std::endl;

  delete_generators();
  exit(EXIT_SUCCESS);
}


void writePhotonMap() {
  PhotonMapper *tracer = dynamic_cast<PhotonMapper*>(myRenderer->getTracer());
  if(tracer == 0) return; //failed to cast or no tracer
  tracer->photonmap->write("photonmap");
}

void readPhotonMap() {
  PhotonMapper *tracer = dynamic_cast<PhotonMapper*>(myRenderer->getTracer());
  if(tracer == 0) return; //failed to cast or no tracer
  tracer->photonmap->read("photonmap");
}


void getArguments(int argc, char *argv[]) {
  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "produce help message")
      ("gl-version,v",po::value<int>(),"Open GL version")
      ("input-file,i", po::value<string>(), "Input file")
      ("output-file,o", po::value<string>(), "Output file")
      ("settings-file,s",po::value<string>(), "Settings file")
      ("headless,l", "run headless");
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
  if (vm.count("help")) {
    cout << desc << "\n";
    exit(1);
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
  if (vm.count("gl-version")) {
    opengl_version = vm["gl-version"].as<int> ();
  } else {
    cout << "Using default OpenGL version: " << opengl_version << "\n";
  }

  if (vm.count("headless")) {
    headless = true;
    cout << "Running headless\n";
  }
}

void initGL() {
  if(opengl_version == 2) {
    return;
  }
  glewInit();
  //startupGLDiagnostics();
  CHECK_GL_ERROR();

  // Workaround for AMD, which hopefully will not be neccessary in the near future...
  if (!glBindFragDataLocation) {
    glBindFragDataLocation = glBindFragDataLocationEXT;
  }
  //
  //                      Create Shaders
  //************************************
  // The loadShaderProgram and linkShaderProgam functions are defined in glutil.cpp and
  // do exactly what we did in lab1 but are hidden for convenience
  shader_program = loadShaderProgram("data/gl_shaders/simple.vert", "data/gl_shaders/simple.frag");
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

  glUseProgram(0);
  CHECK_GL_ERROR();
}


void drawDrawables(IDraw **drawables, size_t n) {
  mat4 view = camera.getViewMatrix();
  if (settings->opengl_version >= 3) {
    glUseProgram(shader_program);
    int loc = glGetUniformLocation(shader_program, "modelViewProjectionMatrix");
    for (size_t i = 0; i < n; ++i) {
      drawables[i]->drawWithView(view, loc);
    }
    glUseProgram(0);
  } else if (settings->opengl_version < 3) {
    for (size_t i = 0; i < n; ++i) {
      drawables[i]->drawWithGLView(view);
    }
  }
}

void drawPoints()
{
  mat4 view = camera.getViewMatrix();
  BaseTracer *tracer = dynamic_cast<BaseTracer*>(myRenderer->getTracer());
  if(tracer == 0) return; //failed to cast or no tracer
  vec3 *posbuff = tracer->posbuff;

  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixf(value_ptr(view));
  glColor3f(0, 1, 0);

  glBegin(GL_POINTS);
  for(int x = 0;x < settings->width;++x){
    for(int y = 0;y < settings->height;++y){
      vec3 v = posbuff[x * settings->height + y];
      float *c = buffer + (x * settings->height + y) * 4;
      glColor4f(c[0], c[1], c[2], c[4]);
      glVertex3f(v.x, v.y, v.z);
    }
  }
  glEnd();

  glPopMatrix();
  glEnable(GL_DEPTH_TEST);
}

void drawPointsPhoton()
{
  mat4 view = camera.getViewMatrix();
  PhotonMapper *tracer = dynamic_cast<PhotonMapper*>(myRenderer->getTracer());
  if(tracer == 0) return; //failed to cast or no tracer
  IPhotonMap *photonmap = tracer->photonmap;

  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixf(value_ptr(view));
  glColor3f(0, 1, 0);

  photonmap->draw();

  glPopMatrix();
  glEnable(GL_DEPTH_TEST);
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

void adjustValue(double speed) {
  switch (var) {
  case TEST:
    myRenderer->stopRendering();
    settings->test += speed * 0.1;
    myRenderer->asyncRender();
    cout << "test: " << settings->test << "\n";
    break;
  case KEY:
    settings->key += speed * 0.01;
    myRenderer->tonemapImage(true);
    cout << "key: " << settings->key << "\n";
    break;
  case WHITE:
    settings->white += speed * 0.1;
    myRenderer->tonemapImage(true);
    cout << "white: " << settings->white << "\n";
    break;
  case RADIUS:
    myRenderer->stopRendering();
    settings->gather_radius += speed;
    myRenderer->asyncRender();
    cout << "photonmap gather radius: " << settings->gather_radius << "\n";
    break;
  }
}

void timedCallback() {
  double diffTime = glfwGetTime() - prevTime;
  prevTime += diffTime;

  double speed = diffTime * 10.0;

  if (glfwGetKey(GLFW_KEY_LCTRL)) {
    speed /= 100;
  }
  if (glfwGetKey(GLFW_KEY_LSHIFT)) {
    speed *= 100;
  }
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
  if (glfwGetKey('X')) {
    camera.translate(vec3(0, 0, speed));
  }
  if (glfwGetKey('Z')) {
    camera.translate(vec3(0, 0, -speed));
  }
  if (glfwGetKey(GLFW_KEY_UP)) {
    camera.rotate(vec2(0,-speed));
  }
  if (glfwGetKey(GLFW_KEY_DOWN)) {
    camera.rotate(vec2(0,speed));
  }
  if (glfwGetKey(GLFW_KEY_LEFT)) {
    camera.rotate(vec2(-speed,0));
  }
  if (glfwGetKey(GLFW_KEY_RIGHT)) {
    camera.rotate(vec2(speed,0));
  }
  if (glfwGetKey(GLFW_KEY_TAB)) {
    auto_render = !auto_render;
    cout << "Autorender: " << auto_render << "\n";
    glfwSleep(0.5);
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
  if (glfwGetKey('7')) {
    renderMode = 7;
  }
  if (glfwGetKey('8')) {
    renderMode = 8;
  }
  if (glfwGetKey('9')) {
    renderMode = 9;
  }
  if (glfwGetKey('E')) {
    myRenderer->stopRendering();
    myRenderer->loadCamera(camera);
    myRenderer->asyncRender();
  }
  if (glfwGetKey('R')) {
    myRenderer->stopRendering();
    myRenderer->loadCamera(camera);
    myRenderer->asyncRender();
    renderMode = 2;
  }
  if (glfwGetKey('T')) {
    myRenderer->stopRendering();
  }
  if (glfwGetKey('Y')) {
    myRenderer->asyncRender();
  }
  if (glfwGetKey('U')) {
    myRenderer->tonemapImage(true);
  }
  if (glfwGetKey('I')) {
    myRenderer->tonemapImage(false);
  }
  if (glfwGetKey('F')) {
    myRenderer->stopRendering();
    settings->use_first_bounce = true;
    myRenderer->asyncRender();
  }
  if (glfwGetKey('G')) {
    myRenderer->stopRendering();
    settings->use_first_bounce = false;
    myRenderer->asyncRender();
  }
  //////////// DEBUG VARIABLES /////////////
  if (glfwGetKey(GLFW_KEY_F1)) {
    var = TEST;
  }
  if (glfwGetKey(GLFW_KEY_F2)) {
    var = KEY;
  }
  if (glfwGetKey(GLFW_KEY_F3)) {
    var = WHITE;
  }
  if (glfwGetKey(GLFW_KEY_F4)) {
    var = RADIUS;
  }
  if (glfwGetKey(GLFW_KEY_KP_ADD)) {
    adjustValue(speed);
  }
  if (glfwGetKey(GLFW_KEY_KP_SUBTRACT)) {
    adjustValue(-speed);
  }
  if (glfwGetKey(GLFW_KEY_KP_MULTIPLY)) {
    myRenderer->stopRendering();
    settings->debug_mode += 1;
    myRenderer->asyncRender();
    cout << settings->debug_mode << "\n";
    glfwSleep(0.5);
  }
  if (glfwGetKey(GLFW_KEY_KP_DIVIDE)) {
    myRenderer->stopRendering();
    settings->debug_mode -= 1;
    myRenderer->asyncRender();
    cout << settings->debug_mode << "\n";
    glfwSleep(0.5);
  }
  if (glfwGetKey('O')) {
    myRenderer->stopRendering();
    readPhotonMap();
    myRenderer->asyncRender();
  }
  if (glfwGetKey('P')) {
    writePhotonMap();
  }
  if (glfwGetKey('Q')) {
    cout << "Percent left: " << myRenderer->renderComplete() << endl;
  }

}

