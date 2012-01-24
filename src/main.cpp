#include <iostream>
#include <stdlib.h>
#include <string>
#include "io/ExporterImpl/PNGExporter.h"
using namespace std;

int main(int argc, char* argv[]) {

	if (argc<5)
		cout << "Too few arguments!" << endl;

	int r,g,b;
	string file;


	r = atoi(argv[1]);
	g = atoi(argv[2]);
	b = atoi(argv[3]);
	file = argv[4];

	cout << "r=" << r << ", g=" << g << ", b=" << b << ", file=" << file << "\n";

    int width=200;
    int height=200;

    uint8_t* buffer = (uint8_t *) calloc (sizeof (uint8_t), width * height * 4);
    for(int i=0;i<height*width*4;i+=4)
    {
        buffer[i] = r;
        buffer[i+1] = g;
        buffer[i+2] = b;
        buffer[i+3] = 255;
    }

	raytracer::PNGExporter exp;
	exp.exportImage(file.c_str(),width,height,buffer);

  return 0;
}

