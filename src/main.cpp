#include "io/ExporterImpl/PNGExporter.h"
#include "raytracer/Renderer.h"

#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 3) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
        std::cout << "Usage is <flags> <infile> <outfile>\n"; // Inform the user of how to use the program
        exit(0);
    } else { // if we got enough parameters...
 
      char* inputFileName, *outputFileName, *settingsFile;
      int width = 200;
      int height = 200;
 
      inputFileName  = argv[1];
      outputFileName = argv[2];
      
      raytracer::Settings settings;
      settings.width = 200;
      settings.height = 200;
      uint8_t bgc[4] = {255,0,255,255};
      settings.backgroundColor = bgc;
      
      
      /* IMPORTER 
       ***************** */
       
       // IMPORTERA HÄR
       
       
      /* RENDERER
       ***************** */
       
       // RENDERA HÄR!
      
      
      // testbuffer
      uint8_t* buffer = (uint8_t *) calloc (sizeof (uint8_t), width * height * 4);
      for(int i=0;i<height*width*4;i+=4)
      {
        buffer[i] = settings.backgroundColor[0];
        buffer[i+1] = settings.backgroundColor[1];
        buffer[i+2] = settings.backgroundColor[2];
        buffer[i+3] = settings.backgroundColor[3];
       }
       
       /* EXPORTER 
       ***************** */
        
        
      raytracer::PNGExporter exp;
      exp.exportImage(outputFileName,settings.width,settings.height,buffer);

    }

  return 0;
}

