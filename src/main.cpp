#include <iostream>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glfw.h>
#include <string>

#include "io/ExporterImpl/PNGExporter.h"
#include "io/ImporterImpl/OBJImporter.h"
#include "raytracer/Renderer.h"
#include "raytracer/AccDataStructImpl/VertexArrayDataStruct.h"
#include "raytracer/utilities/glutil.h"

#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp> //value_ptr
using namespace std;
using namespace raytracer;

GLuint shaderProgram;

void setUniformMVPCamera(GLuint Location, Camera camera) {
	//glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);

	//mat4 View = lookAt(camera.getPosition(), vec3(0,0,0),
	//		camera.getUpVector());
	//mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	//mat4 MVP = Projection * View * Model;
  mat4 view = camera.getViewMatrix();
	glUniformMatrix4fv(Location, 1, GL_FALSE, glm::value_ptr(view));
}

void init() {
	glewInit();
	startupGLDiagnostics();
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

raytracer::Camera camera;
vec2 mousePrev;

void mouseMove(int x, int y) {
  vec2 pos = vec2(x,y);
  if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)){
    vec2 diff = pos-mousePrev;
    camera.rotate(0.5f*diff);
    mousePrev = pos;
  }
}
void mouse(int button, int action) {
  int x,y;
  glfwGetMousePos(&x, &y);
  vec2 pos = vec2(x,y);
  if(action) {
    if(button==GLFW_MOUSE_BUTTON_LEFT) {
      mousePrev = pos;
    }
  }
  //mouseMove(pos.x,pos.y);
}
double prevTime;
void timedCallback() {
  double diffTime = glfwGetTime() - prevTime;
  prevTime += diffTime;

  double speed = diffTime*10.0;
  if (glfwGetKey('W')) { camera.translate(vec3( speed,0,0)); }
  if (glfwGetKey('S')) { camera.translate(vec3(-speed,0,0)); }
  if (glfwGetKey('D')) { camera.translate(vec3(0, speed,0)); }
  if (glfwGetKey('A')) { camera.translate(vec3(0,-speed,0)); }
  if (glfwGetKey(' ')) { camera.translate(vec3(0,0, speed)); }
  if (glfwGetKey('Z')) { camera.translate(vec3(0,0,-speed)); }
}

int main(int argc, char* argv[]) {

	int running = GL_TRUE;

	//Initialize GLFW
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	if (argc < 3) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
		std::cout << "Usage is <flags> <infile> <outfile>\n"; // Inform the user of how to use the program
		exit(0);
	} else { // if we got enough parameters...
		//Open an OpenGl window
		if (!glfwOpenWindow(300, 300, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		init();
		CHECK_GL_ERROR();

		char* inputFileName, *outputFileName, *settingsFile;
		inputFileName = argv[1];
		outputFileName = argv[2];

		raytracer::Settings settings;
		settings.width = 320;
		settings.height = 240;
		settings.backgroundColor[0] = 0;
		settings.backgroundColor[1] = 50;
		settings.backgroundColor[2] = 50;
		settings.backgroundColor[3] = 255;

		/* IMPORTER
		 ***************** */

		raytracer::IImporter* importer = new raytracer::OBJImporter();
		importer->loadFile(inputFileName);
		std::vector<raytracer::Triangle*> triangles =
				importer->getTriangleList();

		/*for(int i=0;i<triangles.size();++i)
		 {
		 vec3* vec0 = triangles[i]->getVertices()[0];
		 vec3* vec1 = triangles[i]->getVertices()[1];
		 vec3* vec2 = triangles[i]->getVertices()[2];

		 std::cout << vec0->x << ", " << vec0->y << ", " << vec0->z << std::endl;
		 std::cout << vec1->x << ", " << vec1->y << ", " << vec1->z << std::endl;
		 std::cout << vec2->x << ", " << vec2->y << ", " << vec2->z << std::endl << std::endl;
		 }*/

		std::cout << std::endl << "eAAAAAAAAAAAAAAAAAAAA" << std::endl;

		VertexArrayDataStruct vertices;
		vertices.setData(triangles);
		CHECK_GL_ERROR();

		/* RENDERER
		 ***************** */

		camera.setPosition(vec3(0.0f, 0.0f, 10.0f));
		camera.setDirection(vec3(0.0f, 0.0f, -1.0f));
		camera.setUpVector(vec3(0.0f, 1.0f, 0.0f));

		glfwSetMouseButtonCallback(mouse);
		glfwSetMousePosCallback(mouseMove);

		//Main loop
		while (running) {
			//OpenGl rendering goes here...d
			glClearColor(0.0, 0.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glViewport(0, 0, 300, 300);
			glUseProgram(shaderProgram);

			int loc = glGetUniformLocation(shaderProgram,
					"modelViewProjectionMatrix");
			setUniformMVPCamera(loc, camera);
			vec3 p = camera.getPosition();
	    std::cout << p.x << "," << p.y << "," << p.z << "\t";
      p = camera.getDirection();
      std::cout << p.x << "," << p.y << "," << p.z << std::endl;

      timedCallback();
      glfwSleep(0.01);

			vertices.draw();

			glUseProgram(0);
			CHECK_GL_ERROR();

			//glDrawPixels(300,300,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8_REV,buffer);

			//Swap front and back rendering buffers
			glfwSwapBuffers();
			CHECK_GL_ERROR();

			//Check if ESC key was pressed or window was closed
			running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(
					GLFW_OPENED);
		}

		//Close window and terminate GLFW
		glfwTerminate();

		delete importer;
		//delete exporter;
		std::cout << std::endl << "end of PROGRAM" << std::endl;
	}

	exit(EXIT_SUCCESS);
}
