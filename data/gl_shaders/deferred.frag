#version 130

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

uniform sampler2D colortexture;

in float	fmaterial;
varying vec3 fnormal;
varying vec2 ftexcoord;

out vec4 onormal;
out vec2 otexcoord;

//we save the material as a float in the w component of the normals
//the normals.w must be cleared to -1

void main() 
{
  onormal = vec4(normalize(fnormal),fmaterial);
  otexcoord = ftexcoord; 
}