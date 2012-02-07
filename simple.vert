#version 130

in vec3		position;
in vec3		color;
out vec4	outColor;
in	vec2	texCoordIn;	// incoming texcoord from the texcoord array
out	vec2	texCoord;	// outgoing interpolated texcoord to fragshader

uniform mat4 modelViewProjectionMatrix; 

void main() 
{
	gl_Position = modelViewProjectionMatrix * vec4(position,1);
	outColor = vec4(color,1); 
	//texCoord = texCoordIn; 
}