#include "AABBTree.h"
#include <cassert>
#include <stack>

AABBTree::AABBTree(unsigned initialSize) : m_rootNodeIndex(AABB_NULL_NODE), m_allocatedNodeCount(0), m_nextFreeNodeIndex(0), m_nodeCapacity(initialSize), m_growthSize(initialSize)
{
	m_nodes.resize(initialSize);
	for (unsigned nodeIndex = 0; nodeIndex < initialSize; nodeIndex++)
	{
		AABBNode& node = m_nodes[nodeIndex];
		node.m_nextNodeIndex = nodeIndex + 1;
	}
	m_nodes[initialSize - 1].m_nextNodeIndex = AABB_NULL_NODE;
}

AABBTree::~AABBTree()
{
}

unsigned AABBTree::AllocateNode()
{
	// if we have no free tree nodes then grow the pool
	if (m_nextFreeNodeIndex == AABB_NULL_NODE)
	{
		assert(m_allocatedNodeCount == m_nodeCapacity);

		m_nodeCapacity += m_growthSize;
		m_nodes.resize(m_nodeCapacity);
		for (unsigned nodeIndex = m_allocatedNodeCount; nodeIndex < m_nodeCapacity; nodeIndex++)
		{
			AABBNode& node = m_nodes[nodeIndex];
			node.m_nextNodeIndex = nodeIndex + 1;
		}
		m_nodes[m_nodeCapacity - 1].m_nextNodeIndex = AABB_NULL_NODE;
		m_nextFreeNodeIndex = m_allocatedNodeCount;
	}

	unsigned nodeIndex = m_nextFreeNodeIndex;
	AABBNode& allocatedNode = m_nodes[nodeIndex];
	allocatedNode.m_parentNodeIndex = AABB_NULL_NODE;
	allocatedNode.m_leftNodeIndex = AABB_NULL_NODE;
	allocatedNode.m_rightNodeIndex = AABB_NULL_NODE;
	m_nextFreeNodeIndex = allocatedNode.m_nextNodeIndex;
	m_allocatedNodeCount++;

	return nodeIndex;
}

void AABBTree::DeallocateNode(unsigned nodeIndex)
{
	AABBNode& deallocatedNode = m_nodes[nodeIndex];
	deallocatedNode.m_nextNodeIndex = m_nextFreeNodeIndex;
	m_nextFreeNodeIndex = nodeIndex;
	m_allocatedNodeCount--;
}

void AABBTree::InsertObject(const std::shared_ptr<IAABB>& object)
{
	unsigned nodeIndex = AllocateNode();
	AABBNode& node = m_nodes[nodeIndex];

	node.m_aabb = object->GetAABB();
	node.m_object = object;

	InsertLeaf(nodeIndex);
	m_objectNodeIndexMap[object] = nodeIndex;
}

void AABBTree::RemoveObject(const std::shared_ptr<IAABB>& object)
{
	unsigned nodeIndex = m_objectNodeIndexMap[object];
	RemoveLeaf(nodeIndex);
	DeallocateNode(nodeIndex);
	m_objectNodeIndexMap.erase(object);
}

void AABBTree::UpdateObject(const std::shared_ptr<IAABB>& object)
{
	unsigned nodeIndex = m_objectNodeIndexMap[object];
	UpdateLeaf(nodeIndex, object->GetAABB());
}

std::forward_list<std::shared_ptr<IAABB>> AABBTree::QueryOverlaps(const std::shared_ptr<IAABB>& object) const
{
	std::forward_list<std::shared_ptr<IAABB>> overlaps;
	std::stack<unsigned> stack;
	AABB testAabb = object->GetAABB();

	stack.push(m_rootNodeIndex);
	while (!stack.empty())
	{
		unsigned nodeIndex = stack.top();
		stack.pop();

		if (nodeIndex == AABB_NULL_NODE) continue;

		const AABBNode& node = m_nodes[nodeIndex];
		if (node.m_aabb.Overlaps(testAabb))
		{
			if (node.IsLeaf() && node.m_object != object)
			{
				overlaps.push_front(node.m_object);
			}
			else
			{
				stack.push(node.m_leftNodeIndex);
				stack.push(node.m_rightNodeIndex);
			}
		}
	}
	return overlaps;
}

std::vector<AABBNode> AABBTree::GetNodes() const
{
	return m_nodes;
}

void AABBTree::InsertLeaf(unsigned leafNodeIndex)
{
	// make sure we're inserting a new leaf
	assert(m_nodes[leafNodeIndex].m_parentNodeIndex == AABB_NULL_NODE);
	assert(m_nodes[leafNodeIndex].m_leftNodeIndex == AABB_NULL_NODE);
	assert(m_nodes[leafNodeIndex].m_rightNodeIndex == AABB_NULL_NODE);

	// if the tree is empty then we make the root the leaf
	if (m_rootNodeIndex == AABB_NULL_NODE)
	{
		m_rootNodeIndex = leafNodeIndex;
		return;
	}

	// search for the best place to put the new leaf in the tree
	// we use surface area and depth as search heuristics
	unsigned treeNodeIndex = m_rootNodeIndex;
	AABBNode& leafNode = m_nodes[leafNodeIndex];
	while (!m_nodes[treeNodeIndex].IsLeaf())
	{
		// because of the test in the while loop above we know we are never a leaf inside it
		const AABBNode& treeNode = m_nodes[treeNodeIndex];
		unsigned leftNodeIndex = treeNode.m_leftNodeIndex;
		unsigned rightNodeIndex = treeNode.m_rightNodeIndex;
		const AABBNode& leftNode = m_nodes[leftNodeIndex];
		const AABBNode& rightNode = m_nodes[rightNodeIndex];

		AABB combinedAabb = treeNode.m_aabb.Merge(leafNode.m_aabb);

		float newParentNodeCost = 2.0f * combinedAabb.GetSurfaceArea();
		float minimumPushDownCost = 2.0f * (combinedAabb.GetSurfaceArea() - treeNode.m_aabb.GetSurfaceArea());

		// use the costs to figure out whether to create a new parent here or descend
		float costLeft;
		float costRight;
		if (leftNode.IsLeaf())
		{
			costLeft = leafNode.m_aabb.Merge(leftNode.m_aabb).GetSurfaceArea() + minimumPushDownCost;
		}
		else
		{
			AABB newLeftAabb = leafNode.m_aabb.Merge(leftNode.m_aabb);
			costLeft = (newLeftAabb.GetSurfaceArea() - leftNode.m_aabb.GetSurfaceArea()) + minimumPushDownCost;
		}
		if (rightNode.IsLeaf())
		{
			costRight = leafNode.m_aabb.Merge(rightNode.m_aabb).GetSurfaceArea() + minimumPushDownCost;
		}
		else
		{
			AABB newRightAabb = leafNode.m_aabb.Merge(rightNode.m_aabb);
			costRight = (newRightAabb.GetSurfaceArea() - rightNode.m_aabb.GetSurfaceArea()) + minimumPushDownCost;
		}

		// if the cost of creating a new parent node here is less than descending in either direction then
		// we know we need to create a new parent node, errrr, here and attach the leaf to that
		if (newParentNodeCost < costLeft && newParentNodeCost < costRight)
		{
			break;
		}

		// otherwise descend in the cheapest direction
		if (costLeft < costRight)
		{
			treeNodeIndex = leftNodeIndex;
		}
		else
		{
			treeNodeIndex = rightNodeIndex;
		}
	}

	// the leafs sibling is going to be the node we found above and we are going to create a new
	// parent node and attach the leaf and this item
	unsigned leafSiblingIndex = treeNodeIndex;
	AABBNode& leafSibling = m_nodes[leafSiblingIndex];
	unsigned oldParentIndex = leafSibling.m_parentNodeIndex;
	unsigned newParentIndex = AllocateNode();
	AABBNode& newParent = m_nodes[newParentIndex];
	newParent.m_parentNodeIndex = oldParentIndex;
	newParent.m_aabb = leafNode.m_aabb.Merge(leafSibling.m_aabb); // the new parents aabb is the leaf aabb combined with it's siblings aabb
	newParent.m_leftNodeIndex = leafSiblingIndex;
	newParent.m_rightNodeIndex = leafNodeIndex;
	leafNode.m_parentNodeIndex = newParentIndex;
	leafSibling.m_parentNodeIndex = newParentIndex;

	if (oldParentIndex == AABB_NULL_NODE)
	{
		// the old parent was the root and so this is now the root
		m_rootNodeIndex = newParentIndex;
	}
	else
	{
		// the old parent was not the root and so we need to patch the left or right index to
		// point to the new node
		AABBNode& oldParent = m_nodes[oldParentIndex];
		if (oldParent.m_leftNodeIndex == leafSiblingIndex)
		{
			oldParent.m_leftNodeIndex = newParentIndex;
		}
		else
		{
			oldParent.m_rightNodeIndex = newParentIndex;
		}
	}

	// finally we need to walk back up the tree fixing heights and areas
	treeNodeIndex = leafNode.m_parentNodeIndex;
	FixUpwardsTree(treeNodeIndex);
}

void AABBTree::RemoveLeaf(unsigned leafNodeIndex)
{
	// if the leaf is the root then we can just clear the root pointer and return
	if (leafNodeIndex == m_rootNodeIndex)
	{
		m_rootNodeIndex = AABB_NULL_NODE;
		return;
	}

	AABBNode& leafNode = m_nodes[leafNodeIndex];
	unsigned parentNodeIndex = leafNode.m_parentNodeIndex;
	const AABBNode& parentNode = m_nodes[parentNodeIndex];
	unsigned grandParentNodeIndex = parentNode.m_parentNodeIndex;
	unsigned siblingNodeIndex = parentNode.m_leftNodeIndex == leafNodeIndex ? parentNode.m_rightNodeIndex : parentNode.m_leftNodeIndex;
	assert(siblingNodeIndex != AABB_NULL_NODE); // we must have a sibling
	AABBNode& siblingNode = m_nodes[siblingNodeIndex];

	if (grandParentNodeIndex != AABB_NULL_NODE)
	{
		// if we have a grand parent (i.e. the parent is not the root) then destroy the parent and connect the sibling to the grandparent in its
		// place
		AABBNode& grandParentNode = m_nodes[grandParentNodeIndex];
		if (grandParentNode.m_leftNodeIndex == parentNodeIndex)
		{
			grandParentNode.m_leftNodeIndex = siblingNodeIndex;
		}
		else
		{
			grandParentNode.m_rightNodeIndex = siblingNodeIndex;
		}
		siblingNode.m_parentNodeIndex = grandParentNodeIndex;
		DeallocateNode(parentNodeIndex);

		FixUpwardsTree(grandParentNodeIndex);
	}
	else
	{
		// if we have no grandparent then the parent is the root and so our sibling becomes the root and has it's parent removed
		m_rootNodeIndex = siblingNodeIndex;
		siblingNode.m_parentNodeIndex = AABB_NULL_NODE;
		DeallocateNode(parentNodeIndex);
	}

	leafNode.m_parentNodeIndex = AABB_NULL_NODE;
}

void AABBTree::UpdateLeaf(unsigned leafNodeIndex, const AABB& newAaab)
{
	AABBNode& node = m_nodes[leafNodeIndex];

	// if the node contains the new aabb then we just leave things
	// TODO: when we add velocity this check should kick in as often an update will lie within the velocity fattened initial aabb
	// to support this we might need to differentiate between velocity fattened aabb and actual aabb
	if (node.m_aabb.Contains(newAaab)) return;

	RemoveLeaf(leafNodeIndex);
	node.m_aabb = newAaab;
	InsertLeaf(leafNodeIndex);
}


void AABBTree::FixUpwardsTree(unsigned treeNodeIndex)
{
	while (treeNodeIndex != AABB_NULL_NODE)
	{
		AABBNode& treeNode = m_nodes[treeNodeIndex];

		// every node should be a parent
		assert(treeNode.m_leftNodeIndex != AABB_NULL_NODE && treeNode.m_rightNodeIndex != AABB_NULL_NODE);

		// fix height and area
		const AABBNode& leftNode = m_nodes[treeNode.m_leftNodeIndex];
		const AABBNode& rightNode = m_nodes[treeNode.m_rightNodeIndex];
		treeNode.m_aabb = leftNode.m_aabb.Merge(rightNode.m_aabb);

		treeNodeIndex = treeNode.m_parentNodeIndex;
	}
}