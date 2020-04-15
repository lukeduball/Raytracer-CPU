#pragma once

#include <vector>

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

public:
};