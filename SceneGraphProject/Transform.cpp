#include "Transform.h"

Transform::Transform()
	:m_position(glm::vec3(0.0f)), m_scale(glm::vec3(1.0f)), m_yaw(0.0f), m_pitch(0.0f), m_roll(0.0f), m_cachedTransformMatrix(glm::mat4(1.0f))
{
}

Transform::Transform(glm::vec3 position, glm::vec3 scale, float pitch, float yaw, float roll)
	: m_position(position), m_scale(scale), m_yaw(yaw), m_pitch(pitch), m_roll(roll), m_cachedTransformMatrix(glm::mat4(1.0f))
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

void Transform::Scale(glm::vec3 scale)
{
	m_scale *= scale;
	UpdateTransformMatrix();
}


void Transform::UpdateTransformMatrix()
{
	m_cachedTransformMatrix = glm::translate(glm::mat4(1.0f), m_position);
	m_cachedTransformMatrix = glm::rotate(m_cachedTransformMatrix, m_pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	m_cachedTransformMatrix = glm::rotate(m_cachedTransformMatrix, m_yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	m_cachedTransformMatrix = glm::rotate(m_cachedTransformMatrix, m_roll, glm::vec3(0.0f, 0.0f, 1.0f));
	m_cachedTransformMatrix = glm::scale(m_cachedTransformMatrix, m_scale);
}

glm::mat4 Transform::GetTransformMatrix()
{
	return m_cachedTransformMatrix;
}
