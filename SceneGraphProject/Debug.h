#pragma once
#include <glad/glad.h>
class Debug
{
public:
    static void OpenglCallbackFunction(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam);
};

