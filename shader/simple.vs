#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 transform;

out vec4 vertexColor;
out vec3 vectColor;

void main()
{
  gl_Position = transform * vec4(aPos, 1.0);
  vertexColor = vec4(aColor, 1.0);
  vectColor = aColor;
}