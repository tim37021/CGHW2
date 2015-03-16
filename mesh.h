#ifndef _MESH_H_
#define _MESH_H_

#include <fstream>
#include <vector>
#include "vertex.h"

namespace SRenderer
{
	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<int> indices;
	};

	void loadObjMesh(std::istream &, Mesh *out);
}

#endif