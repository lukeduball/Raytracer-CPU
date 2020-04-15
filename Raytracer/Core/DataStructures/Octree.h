#pragma once

#include <vector>

#include "../Objects/AABB.h"

class OctreeNode
{
public:
	OctreeNode(bool leaf) : isLeafNode(leaf) {}

	bool isLeafNode;
	OctreeNode * parent;
};

class BranchNode : public OctreeNode
{
public:
	BranchNode() : OctreeNode(false) {}
	OctreeNode * children[8];
};

template <typename T>
class LeafNode: public OctreeNode
{
public:
	std::vector<T*> contents;
};

template <typename T>
class Octree
{
private:
	OctreeNode * root;
	uint32_t minObjects;
	uint32_t minSize;

public:
	Octree(uint32_t mObjects, uint32_t mSize) : minObjects(mObjects), minSize(mSize) {}
};