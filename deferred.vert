#version 130

in vec3		position;
in vec3 	color;
in vec2		texCoordIn;	// incoming texcoord from the texcoord array
in vec3		normal;
in float	material;

out float	fmaterial;
out	vec3	fnormal;
out	vec2	ftexcoord;	// outgoing interpolated texcoord to fragshader

uniform mat4 modelViewProjectionMatrix; 

void main() 
{
  fnormal = normal;
  fmaterial = material;
  gl_Position = modelViewProjectionMatrix * vec4(position,1);
  ftexcoord = texCoordIn; 
}
