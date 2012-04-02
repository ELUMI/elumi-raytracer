#ifdef WIN32
  #define WIN32_LEAN_AND_MEAN
  #define VC_EXTRALEAN
  #define NOMINMAX //          - Macros min(a,b) and max(a,b)
#include <windows.h>
#endif // WIN32

#ifdef __linux__
#	include <unistd.h>
#endif // ! __linux__

#include <GL/glew.h>
#include <stdio.h>
#include <GL/glu.h>
#include <iostream>
//#include <GL/glut.h>

//#include <IL/il.h>
//#include <IL/ilut.h>

//#include "float2.h"
//#include "float3x3.h"
#include "glutil.h"

#include <cmath>
#include <cstring>
#include <cstdlib>

#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <stdio.h>

using std::vector;
//using namespace chag;
//
//
//float4x4 perspectiveMatrix(float fov, float aspectRatio, float n, float f) /* field of view, aspect ratio, near clipping plane, far clipping plane */
//{
//	// This matrix is created identically to gluPerspective()
//	// and takes identical parameters.
//	return make_perspective(fov, aspectRatio, n, f);
//}
//
//
//
//float4x4 lookAt(const float3 &eye, const float3 &center, const float3 &up)
//{
//	float3 dir = chag::normalize(eye - center);
//	float3 right = chag::normalize(cross(up, chag::normalize(dir)));
//	float3 newup = chag::normalize(cross(dir, right));
//	float3x3 R = make_matrix(right, newup, dir);
//	float4x4 invrot = make_matrix(transpose(R), make_vector(0.0f,0.0f,0.0f));
//	return invrot * make_translation(-eye);
//}

//GLuint loadCubeMap(const char* facePosX, const char* faceNegX, const char* facePosY, const char* faceNegY, const char* facePosZ, const char* faceNegZ)
//{
//	//********************************************
//	//	Creating a local helper function in C++
//	//********************************************
//	class tempTexHelper {
//	public:
//		static void loadCubeMapFace(std::string filename, GLenum face)
//		{
//			ILuint image;
//			ilGenImages(1, &image);
//			ilBindImage(image);
//
//			if(ilLoadImage(filename.c_str()) == IL_FALSE)   {
//				std::cout << "Failed to load texture: " << filename << std::endl;
//				ILenum Error;
//				while ((Error = ilGetError()) != IL_NO_ERROR)
//					printf("%d: %s\n", Error, iluErrorString(Error));
//			}
//			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
//			int s = std::max(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
//			iluScale(s, s, ilGetInteger(IL_IMAGE_DEPTH));
//			glTexImage2D(face, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
//		}
//	};
//
//
//	//************************************************
//	//	Creating a texture ID for the OpenGL texture
//	//************************************************
//	GLuint textureID;
//	glGenTextures(1, &textureID);
//
//	//************************************************
//	//	 Load the faces into the cube map texture
//	//************************************************
//	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
//
//	tempTexHelper::loadCubeMapFace(facePosX, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
//	tempTexHelper::loadCubeMapFace(faceNegX, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
//	tempTexHelper::loadCubeMapFace(facePosY, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
//	tempTexHelper::loadCubeMapFace(faceNegY, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
//	tempTexHelper::loadCubeMapFace(facePosZ, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
//	tempTexHelper::loadCubeMapFace(faceNegZ, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
//
//	//************************************************
//	//			Set filtering parameters
//	//************************************************
//
//	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
//
//	// Sets the type of mipmap interpolation to be used on magnifying and
//	// minifying the active texture.
//	// For cube maps, filtering across faces causes artifacts - so disable filtering
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//	// In case you want filtering anyway, try this below instead
////	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Use tri-linear mip map filtering
////	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);			  // or replace trilinear mipmap filtering with nicest anisotropic filtering
//
//
//	CHECK_GL_ERROR();
//	return textureID;
//}


bool checkGLError(const char *file, int line)
{
  bool wasError = false;

  for (GLenum glErr = glGetError(); glErr != GL_NO_ERROR; glErr = glGetError())
  {
    wasError = true;
    const GLubyte* sError = gluErrorString(glErr);

    if (!sError)
    {
      sError = reinterpret_cast<const GLubyte *>(" (no message available)");
    }

    std::cerr << "GL Error #" << glErr << "(" << sError << ") " << " in File " << file << " at line: " << line << std::endl;

#if defined(_WIN32)
    std::stringstream ss;
    ss  << file << "(" << line << "): GL Error #" << glErr << "(" << sError << ") " << std::endl;

    // outputs error message to debug console, if a debugger is attached.
    OutputDebugStringA(ss.str().c_str());
#endif
  }
  return wasError;
}

// startupGLDiagnostics
void startupGLDiagnostics()
{
	// print diagnostic information
	printf( "GL  VENDOR: %s\n", glGetString( GL_VENDOR ) );
	printf( "   VERSION: %s\n", glGetString( GL_VERSION ) );
	printf( "  RENDERER: %s\n", glGetString( GL_RENDERER ) );

	// test if we've got GL 3.0
	if( !GLEW_VERSION_3_0 )
	{
		fatal_error( "OpenGL 3.0 not supported.\n" 
			"Please update your drivers and/or buy a better graphics card."
		);
	}
}

// Error reporting
void fatal_error( std::string errorString, std::string title )
{
	if (title.empty())
	{
		title = "GL-Tutorial - Error";
	}
	if (errorString.empty())
	{
		errorString = "(unknown error)";
	}

	// On Win32 we'll use a message box. On !Win32, just print to stderr and abort()
#if defined(_WIN32)
	MessageBox( 0, errorString.c_str(), title.c_str(), MB_OK | MB_ICONEXCLAMATION );
#else
	fprintf( stderr, "%s : %s\n", title.c_str(), errorString.c_str() );
#endif

	abort();
}


const char *textFileRead( const char *fn, bool fatalError )
{
	/* Note: the `fatalError' thing is a bit of a hack, The proper course of
	 * action would be to have people check the return values of textFileRead,
	 * but lets avoid cluttering the lab-code even further.
	 */

	FILE *fp;
	char *content = NULL;
	int count = 0;

	if( fn != NULL ) 
	{
		fp = fopen( fn, "rt" );
		if( fp != NULL ) 
		{
			fseek( fp, 0, SEEK_END );
			count = ftell( fp );
			fseek( fp, 0, SEEK_SET );

			if( count > 0 ) 
			{
				content = new char[count+1];
				count = fread( content, sizeof(char), count, fp );
				content[count] = '\0';
			}
			else
			{
				if( fatalError )
				{
					char buffer[256];
					sprintf( buffer, "File '%s' is empty\n", fn );
					fatal_error( buffer );
				}
			}

			fclose( fp );
		}
		else
		{
			if( fatalError )
			{
				char buffer[256];
				sprintf( buffer, "Unable to read file '%s'\n", fn );
				fatal_error( buffer );
			}
		}
	}
	else
	{
		if( fatalError )
			fatal_error( "textFileRead - argument NULL\n" );
	}

	return content;
}



std::string GetShaderInfoLog(GLuint obj) {
	int logLength = 0;
	int charsWritten  = 0;
	char *tmpLog;
	std::string log;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength > 0) {
		tmpLog = new char[logLength];
		glGetShaderInfoLog(obj, logLength, &charsWritten, tmpLog);
		log = tmpLog;
		delete[] tmpLog;
	}

	return log;
}



GLuint loadShaderProgram(const std::string &vertexShader, const std::string &fragmentShader)
{
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char *vs = textFileRead(vertexShader.c_str());
	const char *fs = textFileRead(fragmentShader.c_str());

	glShaderSource(vShader, 1, &vs, NULL);
	glShaderSource(fShader, 1, &fs, NULL);
  // text data is not needed beyond this point
  delete [] vs;
	delete [] fs;

	glCompileShader(vShader);
	int compileOk = 0;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &compileOk);

	if (!compileOk)
  {
		std::string err = GetShaderInfoLog(vShader);
    std::cout << err;
		fatal_error( err );
		return 0;
	}

	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &compileOk);
	if (!compileOk)
  {
		std::string err = GetShaderInfoLog(fShader);
		fatal_error( err );
		return 0;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, fShader);
	glDeleteShader( fShader );
	glAttachShader(shaderProgram, vShader);
	glDeleteShader( vShader );
	CHECK_GL_ERROR();

	return shaderProgram; 
}


void linkShaderProgram(GLuint shaderProgram)
{
	glLinkProgram(shaderProgram);
  GLint linkOk = 0;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkOk);
  if (!linkOk)
	{
	  std::string err = GetShaderInfoLog(shaderProgram);
	  fatal_error(err);
	  return;
  }
}


GLuint createAddAttribBuffer(GLuint vertexArrayObject, const void *data, const size_t dataSize, GLuint attributeIndex, GLsizei attributeSize, GLenum type, GLenum bufferUsage)
{
	GLuint buffer = 0;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, bufferUsage);
	CHECK_GL_ERROR();

	// Now attach buffer to vertex array object.
	glBindVertexArray(vertexArrayObject);
	glVertexAttribPointer(attributeIndex, attributeSize, type, false, 0, 0 );	
	glEnableVertexAttribArray(attributeIndex);
	CHECK_GL_ERROR();

	return buffer;
}




//void setUniformSlow(GLuint shaderProgram, const char *name, const float4x4 &matrix)
//{
//	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, false, &matrix.c1.x);
//}
void setUniformSlow(GLuint shaderProgram, const char *name, const float value)
{
	glUniform1f(glGetUniformLocation(shaderProgram, name), value); 
}
void setUniformSlow(GLuint shaderProgram, const char *name, const GLint value)
{
  int loc = glGetUniformLocation(shaderProgram, name);
	glUniform1i(loc, value); 
}
//void setUniformSlow(GLuint shaderProgram, const char *name, const float3 &value)
//{
//	glUniform3fv(glGetUniformLocation(shaderProgram, name), 1, &value.x);
//}

//void debugDrawLight(const float4x4 &viewMatrix,
//					const float4x4 &projectionMatrix,
//					const float3 &worldSpaceLightPos)
//{
//	glPushAttrib(GL_ALL_ATTRIB_BITS);
//	GLint temp;
//	glColor3f(1.0, 1.0, 0.0);
//	glGetIntegerv(GL_CURRENT_PROGRAM, &temp);
//	glUseProgram(0);
//	glMatrixMode(GL_PROJECTION);
//	glLoadMatrixf(&projectionMatrix.c1.x);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadMatrixf(&viewMatrix.c1.x);
//	glEnable(GL_LINE_STIPPLE);
//	glLineStipple(1, 52428);
//	glBegin(GL_LINES);
//	glVertex3fv(&worldSpaceLightPos.x);
//	glVertex3f(0.0f, 0.0f, 0.0f);
//	glEnd();
//	glTranslatef(worldSpaceLightPos.x, worldSpaceLightPos.y, worldSpaceLightPos.z);
//	glutSolidSphere(1.0, 20, 20);
//	glUseProgram(temp);
//	glPopAttrib();
//}

#if defined(__linux__)
void linux_initialize_cwd()
{
	const unsigned kMaxPath = 1024; // Oh, well.

	/* Attempt to change CWD (current working directory) to 
	 * 		- ${BINDIR}/../share/${BINNAME}/
	 * 		- ${BINDIR}
	 *	where ${BINDIR} refers to the path of the directory where the binary
	 *	is located, and ${BINNAME} refers to the name of the binary.
	 *
	 *	For example, if we launch `bin/lab2' in `/home/foo/labs':
	 * 		$ cd /home/foo/labs
	 * 		$ bin/lab2
	 *	we'll try to change to 
	 *		- /home/foo/labs/share/lab2
	 *		- /home/foo/labs/bin
	 *	(in this case, it's assumed that the former succeeds.)
	 */
	
	// try to get the path to the binary
	char pathToBin[kMaxPath];
	ssize_t ret = readlink( "/proc/self/exe", pathToBin, kMaxPath-1 );

	if( ret == -1 )
		return;

	pathToBin[ret] = '\0';

	// extract parts
	char* dirsep = strrchr( pathToBin, '/' );
	if( dirsep ) *dirsep = '\0';

	const char* bindir = pathToBin;
	const char* binname = dirsep + 1;

	// attempt first ${BINDIR}/../share/${BINNAME}
	char pathToData[kMaxPath];
	snprintf( pathToData, kMaxPath, "%s/../share/%s", bindir, binname );

	if( 0 == chdir( pathToData ) )
	{
		fprintf( stderr, "Note: expecting to find data in `%s'\n", pathToData );
		return;
	}

	// then attempt ${BINDIR}
	if( 0 == chdir( pathToBin ) )
	{
		fprintf( stderr, "Note: expecting to find data in `%s'\n", pathToBin );
		return;
	}

	// neither seems to exist; give up.
	char currentCWD[kMaxPath];
	if( !getcwd( currentCWD, kMaxPath ) )
		strncpy( currentCWD, "???", kMaxPath );

	fprintf( stderr, "Note: expecting to find data in `%s'\n", currentCWD );
	return;
}

#endif // ! __linux__
