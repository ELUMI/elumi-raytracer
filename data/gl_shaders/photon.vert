#version 110

in vec3 direction; //pointing away from position, by convention
in vec3 position;
in vec3 normal;
in vec3 power; //color

uniform float radius = 0.1;
float width;
float height;

uniform sampler2D normal_tex;
uniform sampler2D depth_tex;

out vec3 photon_direction;
out vec3 photon_position;
out vec3 photon_normal;
out vec3 photon_power;

uniform vec3 camera_position;

uniform mat4 modelViewProjectionMatrix; 


const vec4 konst   = vec4(0, 0.5, 1, 2);
const vec4 diag    = vec4(1, 1, 1, -1);

float dph(in vec4 a, in vec4 b) {
  return dot(a.xyz,b.xyz) + b.w;
}

void sphere_pointsize2(in vec4 _center, in float radius) {
  float _rad      = radius;

  //rows of model view matrix
  vec4 mvp_r0 = vec4(modelViewProjectionMatrix[0][0], modelViewProjectionMatrix[1][0], modelViewProjectionMatrix[2][0], modelViewProjectionMatrix[3][0]);
  vec4 mvp_r1 = vec4(modelViewProjectionMatrix[0][1], modelViewProjectionMatrix[1][1], modelViewProjectionMatrix[2][1], modelViewProjectionMatrix[3][1]);
  vec4 mvp_r3 = vec4(modelViewProjectionMatrix[0][3], modelViewProjectionMatrix[1][3], modelViewProjectionMatrix[2][3], modelViewProjectionMatrix[3][3]);
  //vec4 mvp_r0     = gl_ModelViewProjectionMatrixTranspose[0];
  //vec4 mvp_r1     = gl_ModelViewProjectionMatrixTranspose[1];
  //vec4 mvp_r3     = gl_ModelViewProjectionMatrixTranspose[3];
  //mat4 mv_inv     = gl_ModelViewMatrixInverse;
  //vec4 p_inv_c2   = gl_ProjectionMatrixInverse[2];
  vec4 viewport   = vec4(width, height, 0.0016667,0.002499);

  vec4 tc_c0, d_tc_c0;
  vec4 tc_c1, d_tc_c1;
  vec4 tc_c3, d_tc_c3;
  vec4 eqn;
  //gl_FrontColor = gl_Color;

  //screen planes in parameter space
  tc_c0.xyz = mvp_r0.xyz * _rad;
  tc_c0.w = dph(_center,mvp_r0);

  tc_c1.xyz = mvp_r1.xyz * _rad;
  tc_c1.w = dph(_center,mvp_r1);

  tc_c3.xyz = mvp_r3.xyz * _rad;
  tc_c3.w = dph(_center,mvp_r3);

  //multiply by sign (spheres: {1,1,1,-1})
  d_tc_c0 = diag * tc_c0;
  d_tc_c1 = diag * tc_c1;
  d_tc_c3 = diag * tc_c3;

  //solve two quadratic equations (x,y)
  eqn.x = dot(d_tc_c3,tc_c0);
  eqn.z = dot(d_tc_c0,tc_c0);

  eqn.y = dot(d_tc_c3,tc_c1);
  eqn.w = dot(d_tc_c1,tc_c1);

  tc_c0.w = dot(d_tc_c3,tc_c3);
  tc_c0.w = 1.0 / tc_c0.w;
  eqn = eqn * tc_c0.w;

  //transformed vertex position
  gl_Position.xy = eqn.xy;
  gl_Position.zw = konst.xz;

  //radius
  tc_c1.xy = ((eqn * eqn) - eqn.zwxy).xy;
  tc_c0.x = 1.0 / sqrt(tc_c1.x);
  tc_c0.y = 1.0 / sqrt(tc_c1.y);
  tc_c1.xy = (tc_c1 * tc_c0).xy;

  //pointsize
  tc_c1.xy = (tc_c1 * viewport).xy;
  tc_c1.xy = (tc_c1 * konst.w).xy;
  gl_PointSize = max(tc_c1.x,tc_c1.y);

  //tc_c1.w = 1.0 / _rad;
}


void sphere_pointsize(in vec3 _center, in float radius) {
  //Quadrics papers

  //get rows
  vec4 MPV_r0 = vec4(modelViewProjectionMatrix[0][0], modelViewProjectionMatrix[1][0], modelViewProjectionMatrix[2][0], modelViewProjectionMatrix[3][0]);
  vec4 MPV_r1 = vec4(modelViewProjectionMatrix[0][1], modelViewProjectionMatrix[1][1], modelViewProjectionMatrix[2][1], modelViewProjectionMatrix[3][1]);
  vec4 MPV_r3 = vec4(modelViewProjectionMatrix[0][3], modelViewProjectionMatrix[1][3], modelViewProjectionMatrix[2][3], modelViewProjectionMatrix[3][3]);

  //screen planes in parameter space
  vec4 PMVT_c0, PMVT_c1, PMVT_c3;
  PMVT_c0.xyz = MPV_r0.xyz*radius;
  PMVT_c0.w = dot(_center, MPV_r0.xyz) + MPV_r0.w;

  PMVT_c1.xyz = MPV_r1.xyz*radius;
  PMVT_c1.w = dot(_center, MPV_r1.xyz) + MPV_r0.w;

  PMVT_c3.xyz = MPV_r3.xyz*radius;
  PMVT_c3.w = dot(_center, MPV_r3.xyz) + MPV_r0.w;

  vec4 diag = vec4(1,1,1,-1); //parameter matrix
  vec4 PMVTD_c0 = diag*PMVT_c0;
  vec4 PMVTD_c1 = diag*PMVT_c1;
  vec4 PMVTD_c3 = diag*PMVT_c3;

  //solve two quadratic equations (x,y)
  vec4 eqn;
  eqn.x = dot(PMVTD_c3, PMVT_c0); //-b_x/2
  eqn.z = dot(PMVTD_c0, PMVT_c0); //c_x

  eqn.y = dot(PMVTD_c3, PMVT_c1); //-b_y/2
  eqn.w = dot(PMVTD_c1, PMVT_c1); //c_y

  eqn *= 1/dot(PMVTD_c3, PMVT_c3);

  //transformed vertex position
  gl_Position = vec4(eqn.x,eqn.y,0,1);

  //radius (avoid division by zero)
  vec2 nradius = eqn.xy*eqn.xy-eqn.zw;

  if(nradius.x>0)
    nradius.x /= sqrt(nradius.x);
  if(nradius.y>0)
    nradius.y /= sqrt(nradius.y);

  //pointsize
  //nradius *= viewport_size;
  gl_PointSize = 100;//max(nradius.x*width, nradius.y*height);
}

void main() 
{
  vec4 p = modelViewProjectionMatrix * vec4(position,1);
  p = vec4(p.xyz*1/p.w,1);
  //gl_Position = p;

  photon_direction = direction;
  photon_position = position;
  photon_normal = normal;
  photon_power = power;

  sphere_pointsize2(vec4(position,1), radius); //point_radius/p.z;
}
