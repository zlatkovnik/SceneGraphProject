#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 mvps[128];
uniform mat4 models[128];

void main()
{
    gl_Position = mvps[gl_InstanceID] * vec4(aPos, 1.0);
}