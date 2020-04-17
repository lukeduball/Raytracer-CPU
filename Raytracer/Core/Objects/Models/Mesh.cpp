#include "Mesh.h"

#include "../AABB.h"
#include "../../DataStructures/Octree.h"
#include "../../Math/MathFunctions.h"
#include "../Triangle.h"

#include <bitset>

Mesh::Mesh() 
{

}

Mesh::~Mesh()
{
	if (this->boundingOctree)
	{
		delete this->boundingOctree;
	}
}

void Mesh::constructOctree()
{
	this->boundingOctree = new Octree<uint32_t>(4, 5);
	AABB * meshBoundingBox = AABB::calculateBoundingBox(this->vertices);

	std::vector<uint32_t> triContents;
	for (uint32_t i = 0; i < this->faces.size(); i++)
	{
		triContents.push_back(i);
	}

	//Checks to see if the amount of faces is less than or equal to the minimum number of faces in a leaf node
	if (this->faces.size() <= this->boundingOctree->getMinObjects())
	{
		//Generate the octree as just the root as a leaf node with all the faces in its list
		LeafNode<uint32_t> * leafNode = new LeafNode<uint32_t>(meshBoundingBox);
		leafNode->contents = std::vector<uint32_t>(triContents.begin(), triContents.end());
		this->boundingOctree->root = leafNode;
	}
	//Otherwise generate all of the children recursively for the octree
	else
	{
		this->boundingOctree->root = new BranchNode(meshBoundingBox);
		//Start generating children at a depth of 1
		generateChildren(this->boundingOctree->root, triContents, 1);
	}
}

bool Mesh::intersectMesh(const Ray & ray, float & parameter, Face *& intersectedFace)
{
	std::list<OctreeNode*> intersectionsList;
	float r = MathFunctions::T_INFINITY;
	if (this->boundingOctree->root->boundingBox->intersect(ray, r))
	{
		intersectionsList.push_front(this->boundingOctree->root);
	}

	//Continue working through the list until an intersection is found and breaks out of the loop
	while(true)
	{
		//If the intersection list is empty, there is no intersection
		if (intersectionsList.empty())
		{
			return false;
		}

		//If the first node in the list is a leaf node, check its contents for triangle intersection
		if (intersectionsList.front()->isLeafNode)
		{
			LeafNode<uint32_t> * leafNode = (LeafNode<uint32_t> *)intersectionsList.front();
			intersectionsList.pop_front();
			for (uint32_t i = 0; i < leafNode->contents.size(); i++)
			{
				Face * face = &this->faces[leafNode->contents[i]];
				glm::vec3 vertex1 = this->vertices[face->indices[0]];
				glm::vec3 vertex2 = this->vertices[face->indices[1]];
				glm::vec3 vertex3 = this->vertices[face->indices[2]];

				float rayParameter = MathFunctions::T_INFINITY;
				//Check the triangle for intersection, do not accept an intersection if the rayParameter is zero because the intersection is with the same face and check it is the nearest intersection
				if (Triangle::intersectTriangle(ray, vertex1, vertex2, vertex3, rayParameter) && !ARE_FLOATS_EQUAL(rayParameter, 0.0f) && rayParameter < parameter)
				{
					parameter = rayParameter;
					intersectedFace = face;
				}
			}

			//If there is an intersection, stop executing the loop and return to the calling function
			if (parameter != MathFunctions::T_INFINITY)
			{
				return true;
			}
		}
		else
		{
			//Expand the current list so that the list will empty or the until the first node is a leaf node
			this->boundingOctree->expandWithRayIntersection(ray, intersectionsList);
		}
	}
}

void Mesh::generateChildren(OctreeNode * node, const std::vector<uint32_t> & triContents, uint32_t depth)
{
	AABB * parentBoundingBox = node->boundingBox;
	const glm::vec3 parentCenter = parentBoundingBox->getCenter();
	const glm::vec3 parentHalfDistances = parentBoundingBox->getHalfDistances();

	for (int i = 0; i < 8; i++)
	{
		std::vector<uint32_t> triangleContents;

		//A sign for every combination of directions in 3 dimensions can be represented by 3 bits represented from numbers 0 to 7
		std::bitset<3> bits = std::bitset<3>(i);
		int32_t s1 = -1 + bits[0] * 2;
		int32_t s2 = -1 + bits[1] * 2;
		int32_t s3 = -1 + bits[2] * 2;
		//Create the bounding box the the subdivided box by supplying the half distances and calculated centers
		glm::vec3 center = parentCenter + glm::vec3(parentHalfDistances.x * s1 * 0.5f, parentHalfDistances.y * s2 * 0.5f, parentHalfDistances.z * s3 * 0.5f);
		glm::vec3 halfDistances = parentHalfDistances * 0.5f;
		AABB * boundingBox = new AABB(center, halfDistances);

		for (uint32_t j = 0; j < triContents.size(); j++)
		{
			uint32_t index1 = this->faces[triContents[j]].indices[0];
			uint32_t index2 = this->faces[triContents[j]].indices[1];
			uint32_t index3 = this->faces[triContents[j]].indices[2];
			if (boundingBox->isTriangleOverlapping(this->vertices[index1], this->vertices[index2], this->vertices[index3]))
			{
				triangleContents.push_back(triContents[j]);
			}
		}

		//If there are no triangles in the list, continue leaving the child nullptr so it will not be processed in the future
		if (triangleContents.size() == 0)
		{
			continue;
		}
		//Otherwise, if the size is less than the min triangle count or the depth has reached the maximum depth stop creating nodes
		if (triangleContents.size() <= this->boundingOctree->getMinObjects() || depth == this->boundingOctree->getMaxDepth())
		{
			LeafNode<uint32_t> * leafNode = new LeafNode<uint32_t>(boundingBox);
			leafNode->contents = std::vector<uint32_t>(triangleContents.begin(), triangleContents.end());
			BranchNode * branchNode = (BranchNode*)node;
			branchNode->children[i] = leafNode;
			leafNode->parent = node;
			continue;
		}
		//Otherwise, create a branch node and continue making the octree
		else
		{
			BranchNode * branchNode = new BranchNode(boundingBox);
			BranchNode * parentNode = (BranchNode*)node;
			parentNode->children[i] = branchNode;
			branchNode->parent = node;
			generateChildren(branchNode, triangleContents, depth+1);
		}
	}
}

