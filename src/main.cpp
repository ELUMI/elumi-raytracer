#include "io/ExporterImpl/PNGExporter.h"
#include "raytracer/Renderer.h"

#include <iostream>
#include <stdlib.h>
#include <GL/glfw.h>
#include <string>

#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367

using namespace std;

int main(int argc, char* argv[]) {

  int running = GL_TRUE;

  //Initialize GLFW
  if(!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  if (argc < 3) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
    std::cout << "Usage is <flags> <infile> <outfile>\n"; // Inform the user of how to use the program
    exit(0);
  } else { // if we got enough parameters...

    char* inputFileName, *outputFileName, *settingsFile;
    int width = 300;
    int height = 300;

    inputFileName  = argv[1];
    outputFileName = argv[2];

    raytracer::Settings settings;
    settings.width = 300;
    settings.height = 300;
    uint8_t bgc[4] = {0,50,100,255};
    settings.backgroundColor = bgc;


    /* IMPORTER
     ***************** */

    // IMPORTERA HÄR


    /* RENDERER
     ***************** */



    // testbuffer
    uint8_t* buffer = (uint8_t *) calloc (sizeof (uint8_t), width * height * 4);
    for(int i=0;i<height*width*4;i+=4)
    {
      if(i<10000){
      buffer[i] = 255;
      buffer[i+1] = 0;
      buffer[i+2] = 0;
      buffer[i+3] = settings.backgroundColor[3];
      }
      else
      {
        buffer[i] = settings.backgroundColor[0];
              buffer[i+1] = settings.backgroundColor[1];
              buffer[i+2] = settings.backgroundColor[2];
              buffer[i+3] = settings.backgroundColor[3];
      }

      }

    /* EXPORTER
     ***************** */


    raytracer::PNGExporter exp;
    exp.exportImage(outputFileName,settings.width,settings.height,buffer);

    //Open an OpenGl window
      if(!glfwOpenWindow(300,300,0,0,0,0,0,0,GLFW_WINDOW)) {
        glfwTerminate();
        exit(EXIT_FAILURE);
      }

      //Main loop
      while(running) {
        //OpenGl rendering goes here...d

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawPixels(300,300,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8_REV,buffer);

        //Swap front and back rendering buffers
        glfwSwapBuffers();

        //Check if ESC key was pressed or window was closed
        running = !glfwGetKey(GLFW_KEY_ESC) &&
            glfwGetWindowParam(GLFW_OPENED);
      }

      //Close window and terminate GLFW
      glfwTerminate();

  }

  exit(EXIT_SUCCESS);
}
