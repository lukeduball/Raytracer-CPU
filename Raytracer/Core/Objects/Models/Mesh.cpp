#include "Mesh.h"

#include "../AABB.h"

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

