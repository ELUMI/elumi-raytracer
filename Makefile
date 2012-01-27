LIBS = -I/usr/X11/include/libpng15 -L/usr/X11/lib -lpng15

#raytracer : main.o PNGExporter.o Renderer.o Camera.o Material.o Scene.o
	#clang++ -I/usr/X11/include/libpng15 -L/usr/X11/lib -lpng15 src/main.cpp src/io/ExporterImpl/PNGExporter.cpp
#	clang++ -I/usr/X11/include/libpng15 -L/usr/X11/lib -lpng15 -o raytracer src/main.o src/PNGExporter.

raytracer : main.o PNGExporter.o
	clang++ $(LIBS) -o raytracer main.o PNGExporter.o

main.o : src/main.cpp src/io/ExporterImpl/PNGExporter.h
	clang++ -c src/main.cpp src/io/ExporterImpl/PNGExporter.h

PNGExporter.o : src/io/ExporterImpl/PNGExporter.cpp src/io/ExporterImpl/PNGExporter.h
	clang++ -c $(LIBS) src/io/ExporterImpl/PNGExporter.cpp src/io/ExporterImpl/PNGExporter.h

clean:
	rm *.o raytracer
