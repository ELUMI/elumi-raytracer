#version 130

in vec3 direction; //pointing away from position, by convention
in vec3 position;
in vec3 normal;
in vec3 power; //color
in vec3 vertex;

uniform float radius = 0.1;
uniform float sz = 0.1;

uniform sampler2D normal_tex;
uniform sampler2D depth_tex;

out vec3 photon_direction;
out vec3 photon_position;
out vec3 photon_normal;
out vec3 photon_power;
out float rad;

uniform vec3 camera_position;

uniform mat4 modelViewProjectionMatrix; 

void main() {

  float r = radius*length(direction);
  rad = r;

  vec3 v = vertex;
  v *= r * (1.0/0.7557613141); //make inscribed spheres radius = radius
  v += -normal*dot(v,normal)*(1-sz); //scale by normal
  v += position; //move to position
  vec4 p = vec4(v,1);
  p = modelViewProjectionMatrix * p; //do modeltransforms
  p = vec4(p.xyz*1/p.w,1);
  gl_Position = p;


  //vec4 p = modelViewProjectionMatrix * vec4(position,1);
  //p = vec4(p.xyz*1/p.w,1);
  //gl_Position = p;

  photon_direction = normalize(direction);
  photon_position = position;
  photon_normal = normal;
  photon_power = power;
}
