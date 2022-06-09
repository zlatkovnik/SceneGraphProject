#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

class SceneNode;

struct AABB {
public:
	AABB();
	AABB(SceneNode* node);
	AABB(glm::vec3 pos1, glm::vec3 pos2, SceneNode* node);

	void SetMax(glm::vec3 &max) { m_max = max; };
	glm::vec3 GetMax() const { return m_max; };
	void SetMin(glm::vec3 &min) { m_min = min; };
	glm::vec3 GetMin() const { return m_min; };

	float GetWidth() const { return m_max.x - m_min.x; }
	float GetHeight() const { return m_max.y - m_min.y; }
	float GetDepth() const { return m_max.z - m_min.z; }

	float GetSurfaceArea() const { return m_surfaceArea; }

	SceneNode* GetNode() const { return m_node; }

	bool Overlaps(const AABB& other) const;
	AABB Merge(const AABB& other) const;
	bool Contains(const AABB& other) const;
private:
	glm::vec3 m_max;
	glm::vec3 m_min;
	float m_surfaceArea;
	SceneNode* m_node;

	float CalculateSurfaceArea() const { return 2.0f * (GetWidth() * GetHeight() + GetWidth() * GetDepth() + GetHeight() * GetDepth()); }
};