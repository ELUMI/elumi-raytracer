#version 130


in vec3 direction; //pointing away from position, by convention
in vec3 position;
in vec3 normal;
in vec3 power; //color

uniform sampler2D normal_tex;
uniform sampler2D depth_tex;

out vec3 photon_position;

uniform mat4 modelViewProjectionMatrix; 

void main() 
{
  vec4 p = modelViewProjectionMatrix * vec4(position,1);
  gl_Position = vec4(p.xyz*1/p.w,1);
  photon_position = position;
  gl_PointSize = 100;
}
