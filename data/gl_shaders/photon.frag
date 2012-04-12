#version 130

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

uniform sampler2D normal_tex;
uniform sampler2D depth_tex;

uniform float radius = 0.1;
uniform float scale;
uniform vec3 camera_position;

uniform mat4 inverseModelViewProjectionMatrix;

in vec3 photon_direction;
in vec3 photon_position;
in vec3 photon_normal;
in vec3 photon_power;

out vec3 ocolor;

#	define M_PI 3.14159265358979323846f

float filterKernel(in vec3 interPoint, in vec3 normal) {
  //return 1/(M_PI*radius*radius); //simple filter kernel

  //return (radius-d)/radius; //inverse distance
  
  //advanced filter kernel (ISPM paper)
  float dist = length(interPoint - photon_position);
  float rz = 0.1 * radius;
  float t = (dist / radius) * (1 - dot((interPoint - photon_position) / dist, normal) * (radius + rz) / rz);
  float scale = 4.0;
  float sigma = radius * scale;
  float a = 1 / (sqrt(2 * M_PI) * sigma);
  return scale * a * a * a * exp(-t * t / (2 * sigma * sigma));
}


vec3 brdf(in vec3 incoming_direction,
    in vec3 outgoing_direction, in vec3 normal)
{
  //diffuse
  vec3 color = max(0.0f, dot(-incoming_direction, normal));

  //specular
  //vec3 h = normalize(-outgoing_direction - incoming_direction);
  //vec3 specular = glm::pow(clamp(glm::dot(normal, h)), material->getShininess()) * material->getSpecular();

  return color; // + specular;
}

void main() 
{
  vec4 p = gl_FragCoord;
  p /= p.w;
  ivec2 coord = ivec2(p.x, p.y);
  p.z = texelFetch(depth_tex, coord, 0).r;
  p = inverseModelViewProjectionMatrix*p;
  p /= p.w;


  float d = length(photon_position - vec3(p));
  if(d>radius)
  	discard;

  vec3 normal = texelFetch(normal_tex, coord, 0).xyz;

  vec3 camera_direction = normalize(vec3(p) - camera_position);
  
  vec3 b = brdf(-photon_direction, camera_direction, normal);
  float a = max(0.0f, dot(photon_direction, normal));
  float k = filterKernel(vec3(p), normal);
  //ocolor = b * photon_power * a * k;
  ocolor = b * photon_power * a * k * scale;

}
