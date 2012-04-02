#version 130

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

uniform sampler2D colortexture;

in vec4 outColor;
smooth in vec2 texCoord;

out vec4 fragmentColor;

void main() 
{
	fragmentColor = outColor; //texture(colortexture, texCoord.xy);
}