#version 130

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

uniform sampler2D normal_tex;
uniform sampler2D depth_tex;

uniform float radius = 0.1;
uniform float scale;
uniform float sz = 0.1;
uniform vec3 camera_position;

uniform mat4 inverseModelViewProjectionMatrix;

in vec3 photon_direction;
in vec3 photon_position;
in vec3 photon_normal;
in vec3 photon_power;
in float rad;

out vec3 ocolor;

#define M_PI 3.14159265358979323846f

float filterKernel(in vec3 offset, in vec3 normal, in float r) {
  //return 1/(M_PI*r*r); //simple filter kernel

  //advanced filter kernel (ISPM paper)
  float dist = length(offset);
  float t = (dist / r) * (1 - dot(offset / dist, normal) * (r + sz*r) / sz);
  float sigma = 0.3; //t=1,k<0.1 => sigma<0.45
  return 1/(M_PI*r*r) * exp(-t*t/(2*sigma*sigma));
}

float brdf(in vec3 incoming_direction,
    in vec3 outgoing_direction, in vec3 normal)
{
  //diffuse
  float color = max(0.0f, dot(-incoming_direction, normal));

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

  float d = length(vec3(p) - photon_position);
  if(d>radius)
  	discard;

  vec3 normal = texelFetch(normal_tex, coord, 0).xyz;
  float k = filterKernel(vec3(p)-photon_position, photon_normal, rad);

  vec3 camera_direction = normalize(vec3(p) - camera_position);

  float b = brdf(-photon_direction, camera_direction, normal);
  float a = max(0.0f, dot(photon_direction, normal));
  float c = dot(photon_normal, normal);
  //ocolor = b * photon_power * a * k;
  ocolor = b * a * photon_power * k * c * scale;
  //if(d>radius)
  //  ocolor = vec4(0,1,0,1);

}
