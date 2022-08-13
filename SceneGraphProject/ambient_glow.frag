#version 460 core

out vec4 FragColor;

uniform vec3 color;
uniform float time;

void main()
{
    FragColor = vec4(color.xy, color.z, 1.0f);
}