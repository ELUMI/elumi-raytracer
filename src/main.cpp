#include <iostream>
#include <stdlib.h>
#include <string>
#include "io/ExporterImpl/PNGExporter.h"

using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 3) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
        std::cout << "Usage is <flags> <infile> <outfile>\n"; // Inform the user of how to use the program
        exit(0);
    } else { // if we got enough parameters...
        char* inputFileName, *outputFileName, *settingsFile;
        int width = 200;
        int height = 200;
        /*std::cout << argv[0] << ": ";
        for (int i = 1; i < argc; i++) { 
            if (i + 1 != argc-2) // Check that we haven't finished parsing already
                if (argv[i] == "-settings") {
                    // We know the next argument *should* be the filename:
                    settingsFile = argv[i + 1];
                } else if (argv[i] == "-w") {
                    width = atoi(argv[i + 1]);
                } else if (argv[i] == "-h") {
                    height = atoi(argv[i + 1]);
                } else {
                    std::cout << "Not enough or invalid arguments, please try again.\n";
                    exit(0);
            }
            std::cout << argv[i] << " ";
        }
        */
        inputFileName  = argv[1];
        outputFileName = argv[2];
        
        // testbuffer
        uint8_t* buffer = (uint8_t *) calloc (sizeof (uint8_t), width * height * 4);
        for(int i=0;i<height*width*4;i+=4)
        {
            buffer[i] = 255;
            buffer[i+1] = 255;
            buffer[i+2] = 0;
            buffer[i+3] = 255;
         }
        
        
       	raytracer::PNGExporter exp;
    	exp.exportImage(outputFileName,width,height,buffer);

    }

  return 0;
}

