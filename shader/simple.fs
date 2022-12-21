#version 330 core

in vec4 vertexColor;
in vec3 vectColor;
out vec4 fragColor;

uniform vec4 color;
// light
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;

void main()
{
  vec3 ambient = ambientStrength * lightColor;
  vec3 result = ambient * objectColor;
  fragColor = vec4(result, 1.0);
}