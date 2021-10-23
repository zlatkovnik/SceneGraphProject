#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Model;
struct RenderCommand {
	Model* m_model;
	glm::mat4 m_transformMatrix;
	RenderCommand(Model* model, glm::mat4 tranformMatrix)
		:m_model(model), m_transformMatrix(tranformMatrix) {}
};