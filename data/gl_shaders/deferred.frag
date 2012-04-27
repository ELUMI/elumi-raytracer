#version 130

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

uniform sampler2D colortexture;

in float	fmaterial;
varying vec3 fnormal;
varying vec2 ftexcoord;

out vec3 onormal;
out vec3 otexcoord;

//we save the material as a float in the z component of the texcoords
//the texcoords.z must be cleared to -1

void main() 
{
  onormal = normalize(fnormal);
  otexcoord = vec3(ftexcoord.x,ftexcoord.y,fmaterial);
}