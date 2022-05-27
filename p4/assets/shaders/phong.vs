#version 330 core

uniform mat4 vpMatrix;
uniform mat3 normalMatrix;
uniform mat4 transform;

layout(location=0) in vec4 position;
layout(location=1) in vec3 normal;

out vec3 vertexNormal;

void main()
{
  gl_Position = vpMatrix * transform * position;
  vertexNormal = normalMatrix * normal;
}