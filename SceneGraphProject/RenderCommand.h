#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Model;
class Shader;
struct RenderCommand {
	Model* m_model;
	glm::mat4 m_transformMatrix;
	Shader* m_shader;
	RenderCommand()
		:m_model(nullptr), m_shader(nullptr), m_transformMatrix(glm::mat4(1.0f)) {}
	RenderCommand(Model* model, Shader* shader, glm::mat4 tranformMatrix)
		:m_model(model), m_shader(shader), m_transformMatrix(tranformMatrix) {}
};