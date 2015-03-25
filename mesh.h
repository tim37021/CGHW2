#ifndef _MESH_H_
#define _MESH_H_

#include <fstream>
#include <vector>
#include "vertex.h"

namespace SRenderer
{
	struct Face
	{
		Vertex o, a, b;
	};
	struct Mesh
	{
		std::vector<Face> faces;
	};

	void loadObjMesh(std::istream &, Mesh *out);
}

#endif