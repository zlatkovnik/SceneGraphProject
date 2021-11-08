#version 460 core

out vec4 FragColor;

uniform vec3 color;
uniform float time;

void main()
{
    float timeSine = abs(sin(time));
    FragColor = vec4(color.xy, color.z * timeSine, 1.0f);
}