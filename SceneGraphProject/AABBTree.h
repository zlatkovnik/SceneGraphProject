#pragma once

#include <memory>
#include <vector>
#include <map>
#include <forward_list>
#include "AABB.h"
#include "IAABB.h" 

#define AABB_NULL_NODE 0xffffffff

struct AABBNode {
	AABB m_aabb;
	std::shared_ptr<IAABB> m_object;
	 //tree links
	unsigned m_parentNodeIndex;
	unsigned m_leftNodeIndex;
	unsigned m_rightNodeIndex;
	// node linked list link
	unsigned m_nextNodeIndex;

	bool IsLeaf() const { return m_leftNodeIndex == AABB_NULL_NODE; }

	AABBNode()
		: m_object(nullptr), m_parentNodeIndex(AABB_NULL_NODE), m_leftNodeIndex(AABB_NULL_NODE), m_rightNodeIndex(AABB_NULL_NODE), m_nextNodeIndex(AABB_NULL_NODE)
	{ }
};

class AABBTree {
public:
	AABBTree(unsigned initialSize);
	~AABBTree();

	void InsertObject(const std::shared_ptr<IAABB>& object);
	void RemoveObject(const std::shared_ptr<IAABB>& object);
	void UpdateObject(const std::shared_ptr<IAABB>& object);
	std::forward_list<std::shared_ptr<IAABB>> QueryOverlaps(const std::shared_ptr<IAABB>& object) const;
	std::vector<AABBNode> GetNodes() const;
private:
	std::map<std::shared_ptr<IAABB>, unsigned> m_objectNodeIndexMap;
	std::vector<AABBNode> m_nodes;
	unsigned m_rootNodeIndex;
	unsigned m_allocatedNodeCount;
	unsigned m_nextFreeNodeIndex;
	unsigned m_nodeCapacity;
	unsigned m_growthSize;

	unsigned AllocateNode();
	void DeallocateNode(unsigned nodeIndex);
	void InsertLeaf(unsigned leafNodeIndex);
	void RemoveLeaf(unsigned leafNodeIndex);
	void UpdateLeaf(unsigned leafNodeIndex, const AABB& newAaab);
	void FixUpwardsTree(unsigned treeNodeIndex);
};