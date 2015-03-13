#include <string>
#include <algorithm>
#include <sstream>
#include "mesh.h"
#include <iostream>

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
			out->vertices.push_back({fx, fy, fz});
		}
		if(declarator=="f")
		{
			ss>>ix>>iy>>iz;
			out->indices.push_back(ix);
			out->indices.push_back(iy);
			out->indices.push_back(iz);
		}
	}
}