#pragma once

#include <vector>
#include <list>

#include "../Objects/AABB.h"
#include "../Renderer/Ray.h"
#include "../Math/MathFunctions.h"

class OctreeNode
{
public:
	OctreeNode(bool leaf, AABB * aaBB) : isLeafNode(leaf), boundingBox(aaBB) {}
	~OctreeNode()
	{
		if (boundingBox)
		{
			delete boundingBox;
		}
	}

	bool isLeafNode;
	OctreeNode * parent;
	AABB * boundingBox;
};

class BranchNode : public OctreeNode
{
public:
	BranchNode(AABB * aaBB) : OctreeNode(false, aaBB) {}
	OctreeNode * children[8];
};

template <typename T>
class LeafNode: public OctreeNode
{
public:
	LeafNode(AABB * aaBB) : OctreeNode(true, aaBB) {}
	std::vector<T> contents;
};

struct NodeDistancePair
{
	OctreeNode * n;
	float rayParameter;

	bool operator<(const NodeDistancePair &pair)
	{
		return this->rayParameter < pair.rayParameter;
	}
};


template <typename T>
class Octree
{
private:
	uint32_t minObjects;
	uint32_t maxDepth;

	void deleteChildren(OctreeNode * node)
	{
		if (node->isLeafNode)
		{
			delete node;
			return;
		}

		BranchNode * branchNode = (BranchNode *)node;
		for (int i = 0; i < 8; i++)
		{
			if (branchNode->children[i])
			{
				deleteChildren(branchNode->children[i]);
			}
		}
		delete node;
	}

public:
	Octree(uint32_t mObjects, uint32_t mDepth) : minObjects(mObjects), maxDepth(mDepth) {}

	~Octree()
	{
		if (root)
		{
			deleteChildren(root);
			delete root;
		}
	}

	OctreeNode * root;

	uint32_t getMinObjects()
	{
		return minObjects;
	}

	uint32_t getMaxDepth()
	{
		return maxDepth;
	}

	void expandWithRayIntersection(const Ray & ray, std::list<OctreeNode*> & intersectionsList)
	{
		//Continues to work through the list until the front node is a leaf node
		while (!intersectionsList.front()->isLeafNode)
		{
			BranchNode * branchNode = (BranchNode *)intersectionsList.front();
			intersectionsList.pop_front();

			std::list<NodeDistancePair> newNodes;
			for (uint32_t i = 0; i < 8; i++)
			{
				if (branchNode->children[i])
				{
					OctreeNode * node = branchNode->children[i];
					//If the ray intersects the bounding box, add it to the list while sorting it by closest intersection to furthest intersection distance
					float rayParameter = MathFunctions::T_INFINITY;
					if (node->boundingBox->intersect(ray, rayParameter))
					{
						NodeDistancePair pair;
						pair.n = node;
						pair.rayParameter = rayParameter;
						newNodes.push_back(pair);
					}
				}
			}
			//Sort the new nodes list by the ray parameter so the closer intersections come first
			newNodes.sort();
			while (!newNodes.empty())
			{
				NodeDistancePair pair = newNodes.back();
				newNodes.pop_back();
				intersectionsList.push_front(pair.n);
			}

			//Condition arises if the ray does not intersect with a leaf node
			if (intersectionsList.empty())
			{
				return;
			}
		}
	}
};