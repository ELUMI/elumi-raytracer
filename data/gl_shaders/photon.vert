#version 130


in vec3 direction; //pointing away from position, by convention
in vec3 position;
in vec3 normal;
in vec3 power; //color

uniform sampler2D normal_tex;
uniform sampler2D depth_tex;

out vec3 photon_direction;
out vec3 photon_position;
out vec3 photon_normal;
out vec3 photon_power;

uniform mat4 modelViewProjectionMatrix; 

void main() 
{
  vec4 p = modelViewProjectionMatrix * vec4(position,1);
  gl_Position = vec4(p.xyz*1/p.w,1);

  photon_direction = direction;
  photon_position = position;
  photon_normal = normal;
  photon_power = power;

  gl_PointSize = 100; //TODO: make this size adjust itself
}
