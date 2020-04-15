#include "Mesh.h"

#include "../AABB.h"
#include "../../DataStructures/Octree.h"

Mesh::Mesh() 
{

}

Mesh::~Mesh()
{
	if (this->boundingBox)
	{
		delete this->boundingBox;
	}
}

