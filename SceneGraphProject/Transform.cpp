#include "Transform.h"
#include "SceneNode.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

Transform::Transform()
	:m_position(glm::vec3(0.0f)), m_scale(glm::vec3(1.0f)), m_yaw(0.0f), m_pitch(0.0f),
	m_roll(0.0f), m_cachedTransformMatrix(glm::mat4(1.0f)), m_parentTransform(nullptr)
{
}

Transform::Transform(glm::vec3 position, glm::vec3 scale, float pitch, float yaw, float roll, Transform *parentTransform)
	: m_position(position), m_scale(scale), m_yaw(yaw), m_pitch(pitch),
	  m_roll(roll), m_cachedTransformMatrix(glm::mat4(1.0f)), m_parentTransform(parentTransform)
{
}

void Transform::MoveTo(glm::vec3 position)
{
	m_position = position;
	UpdateTransformMatrix();
}

void Transform::Translate(glm::vec3 position)
{
	m_position = m_position + position;
	UpdateTransformMatrix();
}

void Transform::SetRotation(float pitch, float yaw, float roll)
{
	m_pitch = pitch;
	m_yaw = yaw;
	m_roll = roll;
	UpdateTransformMatrix();
}

void Transform::Rotate(float pitch, float yaw, float roll)
{
	m_pitch += pitch;
	m_yaw += yaw;
	m_roll += roll;
	UpdateTransformMatrix();
}

void Transform::SetScale(glm::vec3 scale)
{
	m_scale = scale;
	UpdateTransformMatrix();
}

Transform* Transform::GetParent() const
{
	return m_parentTransform;
}

void Transform::SetParent(Transform* transform)
{
	m_parentTransform = transform;
}

glm::vec3 Transform::GetPosition() const
{
	return m_position;
}

glm::mat4 Transform::GetWorldTransformMatrix(Transform* currentTransform)
{
	glm::mat4 currentMatrix = currentTransform->GetTransformMatrix();
	if (currentTransform->GetParent() == nullptr) {
		return currentMatrix;
	}
	return currentMatrix * GetWorldTransformMatrix(currentTransform->GetParent());
}

void Transform::GetPositionRotationAndScale(glm::mat4 &mat, glm::vec3& position, glm::vec3& rotation, glm::vec3 scale)
{
	glm::quat q;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mat, scale, q, position, skew, perspective);
	float yaw = atan2(2.0 * (q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);
	float pitch = asin(-2.0 * (q.x * q.z - q.w * q.y));
	float roll = atan2(2.0 * (q.x * q.y + q.w * q.z), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z);
	rotation = glm::vec3(pitch, yaw, roll);
}

void Transform::Scale(glm::vec3 scale)
{
	m_scale *= scale;
	UpdateTransformMatrix();
}

void Transform::UpdateTransformMatrix()
{
	// Rotacija oko proizvoljne ose
	// Razmisliti o redosledu transformacija
	// I visestrukim transformacijama
	//m_cachedTransformMatrix = glm::translate(glm::mat4(1.0f), m_position);
	//m_cachedTransformMatrix = glm::rotate(m_cachedTransformMatrix, m_pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	//m_cachedTransformMatrix = glm::rotate(m_cachedTransformMatrix, m_yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	//m_cachedTransformMatrix = glm::rotate(m_cachedTransformMatrix, m_roll, glm::vec3(0.0f, 0.0f, 1.0f));
	//m_cachedTransformMatrix = glm::scale(m_cachedTransformMatrix, m_scale);

	const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), m_pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), m_yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), m_roll, glm::vec3(0.0f, 0.0f, 1.0f));

	const glm::mat4 roationMatrix = transformY * transformX * transformZ;

	m_cachedTransformMatrix = glm::translate(glm::mat4(1.0f), m_position) * roationMatrix * glm::scale(glm::mat4(1.0f), m_scale);
}

glm::mat4 Transform::GetTransformMatrix() const
{
	return m_cachedTransformMatrix;
}
