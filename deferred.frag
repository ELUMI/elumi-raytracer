#version 130

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

uniform sampler2D colortexture;

in float	fmaterial;
smooth in vec3 fnormal;
smooth in vec2 ftexcoord;

out vec3 onormal;
out vec3 otexcoord;

//we save the material as a float in the z component of the texcoords
//the buffer is cleared to 0, thus we need to differentiate material 0 from it
//we do that by using the formula below

void main() 
{
  onormal = fnormal;
  //otexcoord = vec3(1,1,1); 
  otexcoord = vec3(ftexcoord.x, ftexcoord.y, (fmaterial+1)/256); 
}