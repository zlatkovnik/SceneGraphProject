#include "AABB.h"

AABB::AABB()
	: m_max(glm::vec3(0.0f)), m_min(glm::vec3(0.0f)), m_surfaceArea(0.0f), m_node(nullptr)
{
}

AABB::AABB(SceneNode* node)
	: m_max(glm::vec3(0.0f)), m_min(glm::vec3(0.0f)), m_surfaceArea(0.0f), m_node(node)
{
}

AABB::AABB(glm::vec3 pos1, glm::vec3 pos2, SceneNode* node)
	: m_node(node)
{
	auto min_vec = glm::vec3(-100000.0f);
	if (glm::length(glm::vec3(pos1 - min_vec)) > glm::length(glm::vec3(pos2 - min_vec))) {
		m_max.x = pos1.x; m_max.y = pos1.y; m_max.z = pos1.z;
		m_min.x = pos2.x; m_min.y = pos2.y; m_min.z = pos2.z;
	}
	else {
		m_max.x = pos2.x; m_max.y = pos2.y; m_max.z = pos2.z;
		m_min.x = pos1.x; m_min.y = pos1.y; m_min.z = pos1.z;
	}
	m_surfaceArea = CalculateSurfaceArea();
}

bool AABB::Overlaps(const AABB& other) const
{
	return m_max.x > other.m_min.x &&
		m_min.x < other.m_max.x &&
		m_max.y > other.m_min.y &&
		m_min.y < other.m_max.y &&
		m_max.z > other.m_min.z &&
		m_min.z < other.m_max.z;
}

AABB AABB::Merge(const AABB& other) const
{
	return AABB(
		glm::vec3(std::min(m_min.x, other.m_min.x), std::min(m_min.y, other.m_min.y), std::min(m_min.z, other.m_min.z)),
		glm::vec3(std::max(m_max.x, other.m_max.x), std::max(m_max.y, other.m_max.y), std::max(m_max.z, other.m_max.z)),
		other.GetNode()
	);
}

bool AABB::Contains(const AABB& other) const
{
	return other.m_min.x >= m_min.x &&
		other.m_max.x <= m_max.x &&
		other.m_min.y >= m_min.y &&
		other.m_max.y <= m_max.y &&
		other.m_min.z >= m_min.z &&
		other.m_max.z <= m_max.z;
}
