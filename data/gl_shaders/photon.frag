#version 130

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

uniform sampler2D normal_tex;
uniform sampler2D depth_tex;


out vec3 ocolor;

void main() 
{
  ocolor = vec3(1,100,0);
}