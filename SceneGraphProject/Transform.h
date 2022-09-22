#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SceneNode;

class Transform
{
private:
	glm::vec3 m_position;
	glm::vec3 m_scale;
	float m_yaw;
	float m_pitch;
	float m_roll;
	Transform* m_parentTransform;

	glm::mat4 m_cachedTransformMatrix;
public:
	Transform();
	Transform(glm::vec3 position, glm::vec3 scale, float pitch, float yaw, float roll, Transform* parentTransform);

	glm::mat4 GetTransformMatrix() const;

	void MoveTo(glm::vec3 position);
	void Translate(glm::vec3 position);
	void SetRotation(float pitch, float yaw, float roll);
	void Rotate(float pitch, float yaw, float roll);
	void SetScale(glm::vec3 scale);
	void Scale(glm::vec3 scale);
	void SetParent(Transform* transform);
	Transform* GetParent() const;
	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	static glm::mat4 GetWorldTransformMatrix(Transform* currentTransform);
	static void GetPositionRotationAndScale(glm::mat4 &mat, glm::vec3 &position, glm::vec3 &rotation, glm::vec3 scale);
private:
	void UpdateTransformMatrix();
};

