#version 130

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

uniform sampler2D normal_tex;
uniform sampler2D depth_tex;

uniform float radius = 0.1;

uniform mat4 inverseModelViewProjectionMatrix;

in vec3 photon_position;

out vec3 ocolor;

void main() 
{
  vec4 p = gl_FragCoord;
  p = inverseModelViewProjectionMatrix*p;
  p /= p.w;

  ivec2 coord = ivec2(p.x, p.y);
  p.z = texelFetch(depth_tex, coord, 0).r;
  float d = length(photon_position - vec3(p));
  if(d>radius)
  	discard;
  ocolor = vec3((radius-d)/radius,p.z,0); //vec3(1,0.1,0);
}
