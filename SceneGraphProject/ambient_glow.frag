#version 460 core

out vec4 FragColor;

uniform vec3 color;
uniform float time;

void main()
{
    FragColor = vec4(0.69, 1.0, 0.0, 1.0f);
}