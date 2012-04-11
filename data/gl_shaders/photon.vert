#version 130


in vec3 direction; //pointing away from position, by convention
in vec3 position;
in vec3 normal;
in vec3 power; //color

uniform sampler2D normal_tex;
uniform sampler2D depth_tex;


uniform mat4 modelViewProjectionMatrix; 

void main() 
{
  gl_Position = modelViewProjectionMatrix * vec4(position,1);
}
