# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = g++

# define any compile-time flags
CFLAGS = -Wall -g

# define any directories containing header files other than /usr/include
#
INCLUDES = -I/home/irri/code/include -I/home/irri/code/include/GL

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = -L/home/irri/code/lib/glfw -L/home/irri/code/lib/glew

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = -lpng -lIL -lILU -lboost_program_options -lboost_thread -lboost_chrono -lboost_timer -lboost_system -lglfw -lGL -lGLU -lGLEW

# define the C source files
SRCS=src/io/ExporterImpl/PNGExporter.cpp src/io/ImporterImpl/OBJImporter.cpp src/io/ImporterImpl/OBJImporterImpl/list.cpp src/io/ImporterImpl/OBJImporterImpl/string_extra.cpp src/io/ImporterImpl/OBJImporterImpl/obj_parser.cpp src/io/ImporterImpl/OBJImporterImpl/obj_loader.cpp src/main.cpp src/raytracer/IDraw.cpp src/raytracer/common.cpp src/raytracer/EnvironmentMapImpl/SphereMap.cpp src/raytracer/EnvironmentMapImpl/CubeMap.cpp src/raytracer/Renderer.cpp src/raytracer/SuperSamplePatternImpl/JitterPattern.cpp src/raytracer/SuperSamplePatternImpl/RandomPattern.cpp src/raytracer/SuperSamplePatternImpl/GridPattern.cpp src/raytracer/RenderPatternImpl/LinePattern.cpp src/raytracer/RenderPatternImpl/HilbertCurve.cpp src/raytracer/GLData/PhotonProcesser.cpp src/raytracer/GLData/glutil.cpp src/raytracer/GLData/LineArrayDataStruct.cpp src/raytracer/GLData/VertexArrayDataStruct.cpp src/raytracer/GLData/TriangleArray.cpp src/raytracer/GLData/DeferredProcesser.cpp src/raytracer/GLData/DeferredTexProcesser.cpp src/raytracer/AccDataStructImpl/HashDataStruct.cpp src/raytracer/AccDataStructImpl/CombinedDataStruct.cpp src/raytracer/AccDataStructImpl/KDTreeDataStruct.cpp src/raytracer/AccDataStructImpl/ArrayDataStruct.cpp src/raytracer/AccDataStructImpl/TestHeightMapDataStruct.cpp src/raytracer/scene/Camera.cpp src/raytracer/scene/Heightmap.cpp src/raytracer/scene/LightImpl/BaseLight.cpp src/raytracer/scene/LightImpl/AreaLight.cpp src/raytracer/scene/LightImpl/SpotLight.cpp src/raytracer/scene/Scene.cpp src/raytracer/scene/Texture.cpp src/raytracer/scene/VolumeImpl/UniformVolume.cpp src/raytracer/scene/Material.cpp src/raytracer/ISuperSamplePattern.cpp src/raytracer/utilities/MathHelper.cpp src/raytracer/utilities/Random.cpp src/raytracer/utilities/Ray.cpp src/raytracer/utilities/OBB.cpp src/raytracer/utilities/HashPoint.cpp src/raytracer/utilities/Triangle.cpp src/raytracer/PhaseFunctorImpl/PhaseHG.cpp src/raytracer/PhaseFunctorImpl/PhaseIsotropic.cpp src/raytracer/PhaseFunctorImpl/PhaseSchlick.cpp src/raytracer/PostEffectImpl/ClampOperator.cpp src/raytracer/PostEffectImpl/GammaEncode.cpp src/raytracer/PostEffectImpl/ReinhardOperator.cpp src/raytracer/XMLImpl/pugixml/pugixml.cpp src/raytracer/XMLImpl/XML.cpp src/raytracer/TracerImpl/StandardTracer.cpp src/raytracer/TracerImpl/VolumeTracer.cpp src/raytracer/TracerImpl/PhotonMapper.cpp src/raytracer/TracerImpl/BaseTracer.cpp src/raytracer/TracerImpl/AdvancedTracer.cpp src/raytracer/TracerImpl/PathTracer.cpp src/raytracer/TracerImpl/DebugTracer.cpp src/raytracer/PhotonMapImpl/ArrayPM.cpp src/raytracer/PhotonMapImpl/GridPM.cpp src/raytracer/PhotonMapImpl/HashPM.cpp

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.c=.o)

# define the executable file 
MAIN = elumi

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:    $(MAIN)
	@echo  ELUMI raytracer compiled successfully!

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
