#include <string>
#include <algorithm>
#include <sstream>
#include "mesh.h"
#include <iostream>
#include <cmath>
#include <glm/glm.hpp> // vec3 normalize cross

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

void SRenderer::loadObjMesh(std::istream &in, SRenderer::Mesh *out)
{
	std::string line;

	//Used for input
	std::string declarator;
	float fx, fy, fz;
	int ix, iy, iz;

	while(getline(in, line))
	{
		std::stringstream ss(line);
		ss>>declarator;
		
		if(declarator=="v")
		{
			ss>>fx>>fy>>fz;
			out->vertices.push_back({glm::vec3(fx, fy, fz)});
		}
		if(declarator=="f")
		{
			ss>>ix>>iy>>iz;
			out->indices.push_back(ix);
			out->indices.push_back(iy);
			out->indices.push_back(iz);

			Vertex &o=out->vertices[ix-1];
			Vertex &a=out->vertices[iy-1];
			Vertex &b=out->vertices[iz-1];

			const glm::vec3 &oa=a.pos-o.pos, &ob=b.pos-o.pos;

			const glm::vec3 &n=glm::normalize(glm::cross(oa, ob));

			o.normal+=n;
			a.normal+=n;
			b.normal+=n;
		}
	}

	for(int i=0; i<out->vertices.size(); i++)
		out->vertices[i].normal=glm::normalize(out->vertices[i].normal);
}