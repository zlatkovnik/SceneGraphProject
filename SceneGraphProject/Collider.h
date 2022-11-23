#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ctime>

#include "Component.h"
#include "AABB.h"
#include "SceneNode.h"
#include "Model.h"
#include "CoreManager.h"
#include "ResourceManager.h"
#include "AABBTree.h"

struct AABBObject: public IAABB {
	AABBObject(AABB aabb): m_aabb(aabb) {}
	~AABBObject() override {};
	virtual AABB& GetAABB() override {
		return m_aabb;
	};
private:
	AABB m_aabb;
};

class Collider: public Component {
public:
	Collider(glm::vec3 pos1, glm::vec3 pos2, SceneNode* node)
		: m_node(node), m_normalizedMax(pos1), m_normalizedMin(pos2) {
		AABB aabb(pos1, pos2, node);
		m_aabb.reset(new AABBObject(aabb));
	}

	void Start() override {
		Model* m = nullptr;
		if (m == nullptr) {
			Material* material = &ResourceManager::GetInstance().MaterialLookup["standard"];
			std::vector<float> vertices;
			std::vector<unsigned int> indices;
			CreateBoxVerticesAndIndices(vertices, indices);
			m = new Model(vertices, indices, material, false);
			Model::CachedModels["collision_box" + std::to_string(m->GetNumberOfInstances())] = m;
		}
		m_wire = new SceneNode("wire" + std::to_string(m->GetNumberOfInstances()));
		m->IncrementInstances();
		m->SetWireframe(true);
		m_wire->AddComponent(m);
		m_wire->GetTransform().MoveTo(m_node->GetTransform().GetPosition());
		CoreManager::GetInstance().GetRootNode()->AppendChild(m_wire);

		m_tree.InsertObject(m_aabb);
	}

	void Update(float deltaTime) override {
		auto worldTransform = Transform::GetWorldTransformMatrix(&m_node->GetTransform());
		glm::vec3 position(0.0f);
		glm::vec3 rotation(0.0f);
		glm::vec3 scale(0.0f);
		Transform::GetPositionRotationAndScale(worldTransform, position, rotation, scale);
		auto max = m_normalizedMax + position;
		auto min = m_normalizedMin + position;
		m_wire->GetTransform().MoveTo(position);
		m_aabb.get()->GetAABB().SetMax(max);
		m_aabb.get()->GetAABB().SetMin(min);
		auto overlaps = m_tree.QueryOverlaps(m_aabb);
		auto myName = m_node->GetName();
		if (myName == "box") {
			for (auto& overlap : overlaps) {
				std::string overlapName = overlap.get()->GetAABB().GetNode()->GetName();
				std::cout << myName << " :: " << overlapName << std::endl;
			}
		}
	}

private:
	std::shared_ptr<AABBObject> m_aabb;
	glm::vec3 m_normalizedMax;
	glm::vec3 m_normalizedMin;
	SceneNode* m_node;
	SceneNode* m_wire;
	static AABBTree m_tree;

	void CreateBoxVerticesAndIndices(std::vector<float> &vertices, std::vector<unsigned int> &indices) {
		auto max = m_aabb.get()->GetAABB().GetMax();
		auto min = m_aabb.get()->GetAABB().GetMin();
		// First
		vertices.push_back(min.x);
		vertices.push_back(max.y);
		vertices.push_back(max.z);
		for (int i = 0; i < 5; i++) vertices.push_back(0.0f);
		// Second
		vertices.push_back(max.x);
		vertices.push_back(max.y);
		vertices.push_back(max.z);
		for (int i = 0; i < 5; i++) vertices.push_back(0.0f);
		// Third
		vertices.push_back(min.x);
		vertices.push_back(min.y);
		vertices.push_back(max.z);
		for (int i = 0; i < 5; i++) vertices.push_back(0.0f);
		// Fourth
		vertices.push_back(max.x);
		vertices.push_back(min.y);
		vertices.push_back(max.z);
		for (int i = 0; i < 5; i++) vertices.push_back(0.0f);
		// Fifth
		vertices.push_back(min.x);
		vertices.push_back(max.y);
		vertices.push_back(min.z);
		for (int i = 0; i < 5; i++) vertices.push_back(0.0f);
		// Sixth
		vertices.push_back(max.x);
		vertices.push_back(max.y);
		vertices.push_back(min.z);
		for (int i = 0; i < 5; i++) vertices.push_back(0.0f);
		// Seventh
		vertices.push_back(min.x);
		vertices.push_back(min.y);
		vertices.push_back(min.z);
		for (int i = 0; i < 5; i++) vertices.push_back(0.0f);
		// Eighth
		vertices.push_back(max.x);
		vertices.push_back(min.y);
		vertices.push_back(min.z);
		for (int i = 0; i < 5; i++) vertices.push_back(0.0f);


		indices = {
			// front
			0, 2, 3,
			0, 3, 1,
			// right
			1, 3, 7,
			1, 7, 5,
			// back
			5, 7, 6,
			5, 6, 4,
			// left
			4, 6, 2,
			4, 2, 0,
			// bottom
			6, 3, 2,
			6, 7, 3,
			// top
			4, 0, 1,
			4, 1, 5
		};
	}
};