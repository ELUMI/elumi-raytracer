#include "io/ExporterImpl/PNGExporter.h"
#include "io/ImporterImpl/OBJImporter.h"
#include "raytracer/Renderer.h"

#include <iostream>
#include <stdlib.h>
//#include <GL/glfw.h>
#include <string>

using namespace std;
using namespace raytracer;

int main(int argc, char* argv[]) {

  //  int running = GL_TRUE;
  //
  //  //Initialize GLFW
  //  if(!glfwInit()) {
  //    exit(EXIT_FAILURE);
  //  }

  if (argc < 3) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
    std::cout << "Usage is <flags> <infile> <outfile>\n"; // Inform the user of how to use the program
    exit(0);
  } else { // if we got enough parameters...

    char* inputFileName, *outputFileName, *settingsFile;
    inputFileName  = argv[1];
    outputFileName = argv[2];

    raytracer::Settings settings;
    settings.width = 300;
    settings.height = 300;
    settings.backgroundColor[0] = 0;
    settings.backgroundColor[1] = 50;
    settings.backgroundColor[2] = 50;
    settings.backgroundColor[3] = 255;

    /* IMPORTER
     ***************** */

//    raytracer::IImporter* importer = new raytracer::OBJImporter();
//    importer->loadFile(inputFileName);
//    std::vector<raytracer::Triangle*> triangles = importer->getTriangleList();

    /*for(int i=0;i<triangles.size();++i)
    {
      vec3* vec0 = triangles[i]->getVertices()[0];
      vec3* vec1 = triangles[i]->getVertices()[1];
      vec3* vec2 = triangles[i]->getVertices()[2];

      std::cout << vec0->x << ", " << vec0->y << ", " << vec0->z << std::endl;
      std::cout << vec1->x << ", " << vec1->y << ", " << vec1->z << std::endl;
      std::cout << vec2->x << ", " << vec2->y << ", " << vec2->z << std::endl << std::endl;
    }*/

    Material mat;
    mat.setColor(vec4(255,0,0,255));

    std::vector<vec3*> verts, norms, texs;

    vec3 v1 = vec3(0.0f, 0.0f, 0.0f);
    vec3 v2 = vec3(1.0f, 0.0f, 0.0f);
    vec3 v3 = vec3(0.0f, 1.0f, 0.0f);

    verts.push_back( &v1 );
    verts.push_back( &v2 );
    verts.push_back( &v3 );


    vec3 n1 = vec3(0.0f, 0.0f, 1.0f);
    vec3 n2 = vec3(0.0f, 0.0f, 1.0f);
    vec3 n3 = vec3(0.0f, 0.0f, 1.0f);

    norms.push_back( &n1 );
    norms.push_back( &n2 );
    norms.push_back( &n3 );


    Triangle tri;
    tri.set(verts,norms,texs,&mat);



    std::vector<raytracer::Triangle*> triangles2;
    triangles2.push_back(&tri);
    std::cout << std::endl << "eAAAAAAAAAAAAAAAAAAAA" << std::endl;


    /* RENDERER
     ***************** */

    raytracer::Camera camera;
    camera.setPosition(vec3(0.0f, 0.0f, -10.0f));
//    vec3 pos = vec3(0,0,0);
//    vec3 dir = vec3(0,0,1);
//    vec3 up = vec3(0,1,0);
//
//    camera.set()
    raytracer::Renderer myRenderer(&settings);
    //myRenderer.loadSettings(settings);
    myRenderer.loadCamera(camera);
    if (!triangles2.empty())
      myRenderer.loadTriangles(triangles2);


    std::cout << std::endl << "BBBBBBBBB" << std::endl;

    myRenderer.render();

    uint8_t* buffer = myRenderer.getFloatArray();

    std::cout << std::endl << "CCCCCCCCCCCCCCCCc" << std::endl;

    // testbuffer
//    uint8_t* buffer = (uint8_t *) calloc (sizeof (uint8_t), settings.width * settings.height * 4);
//    for(int i=0;i<settings.height*settings.width*4;i+=4)
//    {
//      if (i<10000){
//        buffer[i] = 255;
//        buffer[i+1] = 0;
//        buffer[i+2] = 0;
//        buffer[i+3] = settings.backgroundColor[3];
//      } else if (i>=10000 && i<30000) {
//
//        buffer[i] = 0;
//        buffer[i+1] = 0;
//        buffer[i+2] = 255;
//        buffer[i+3] = settings.backgroundColor[3];
//      } else {
//        buffer[i] = settings.backgroundColor[0];
//        buffer[i+1] = settings.backgroundColor[1];
//        buffer[i+2] = settings.backgroundColor[2];
//        buffer[i+3] = settings.backgroundColor[3];
//      }
//
//    }

    /* EXPORTER
     ***************** */



    raytracer::IExporter* exporter = new raytracer::PNGExporter;
    exporter->exportImage(outputFileName,settings.width,settings.height,buffer);

    //    //Open an OpenGl window
    //      if(!glfwOpenWindow(300,300,0,0,0,0,0,0,GLFW_WINDOW)) {
    //        glfwTerminate();
    //        exit(EXIT_FAILURE);
    //      }
    //
    //      //Main loop
    //      while(running) {
    //        //OpenGl rendering goes here...d
    //        glClear(GL_COLOR_BUFFER_BIT);
    //
    //        glDrawPixels(300,300,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8_REV,buffer);
    //
    //        //Swap front and back rendering buffers
    //        glfwSwapBuffers();
    //
    //        //Check if ESC key was pressed or window was closed
    //        running = !glfwGetKey(GLFW_KEY_ESC) &&
    //            glfwGetWindowParam(GLFW_OPENED);
    //      }
    //
    //      //Close window and terminate GLFW
    //      glfwTerminate();

    //delete importer;
    delete exporter;
    std::cout << std::endl << "end of PROGRAM" << std::endl;
  }

  exit(EXIT_SUCCESS);
}
