#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
private:
	glm::vec3 m_position;
	glm::vec3 m_scale;
	float m_yaw;
	float m_pitch;
	float m_roll;

	glm::mat4 m_cachedTransformMatrix;
public:
	Transform();
	Transform(glm::vec3 position, glm::vec3 scale, float pitch, float yaw, float roll);

	glm::mat4 GetTransformMatrix();

	void MoveTo(glm::vec3 position);
	void Translate(glm::vec3 position);
	void SetRotation(float pitch, float yaw, float roll);
	void Rotate(float pitch, float yaw, float roll);
	void SetScale(glm::vec3 scale);
	void Scale(glm::vec3 scale);
	glm::vec3 GetPosition();
private:
	void UpdateTransformMatrix();
};

