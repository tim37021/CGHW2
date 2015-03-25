#include <string>
#include "mesh.h"
#include <cstdio>
#include <glm/glm.hpp> // vec3 normalize cross

#define BUFFER_SIZE 128
typedef bool (*PROCESS_FUNC)(const std::string &, 
	std::vector<glm::vec3> &, 
	std::vector<glm::vec2> &,
	std::vector<glm::vec3> &,
	SRenderer::Mesh *);

static bool process_v(const std::string &, 
	std::vector<glm::vec3> &, 
	std::vector<glm::vec2> &, 
	std::vector<glm::vec3> &,
	SRenderer::Mesh *);
static bool process_vt(const std::string &, 
	std::vector<glm::vec3> &, 
	std::vector<glm::vec2> &, 
	std::vector<glm::vec3> &,
	SRenderer::Mesh *);
static bool process_vn(const std::string &, 
	std::vector<glm::vec3> &, 
	std::vector<glm::vec2> &, 
	std::vector<glm::vec3> &,
	SRenderer::Mesh *);
static bool process_f(const std::string &, 
	std::vector<glm::vec3> &, 
	std::vector<glm::vec2> &, 
	std::vector<glm::vec3> &,
	SRenderer::Mesh *);

static PROCESS_FUNC pfunc_ptr[]=
{
process_v, process_vt, process_vn, process_f
};


void SRenderer::loadObjMesh(std::istream &in, SRenderer::Mesh *out)
{
	std::string line;
	std::vector<glm::vec3> v;
	std::vector<glm::vec2> vt;
	std::vector<glm::vec3> vn;

	while(getline(in, line))
	{
		for(int i=0;i<4;i++)
			if(pfunc_ptr[i](line, v, vt, vn, out))
				break;
	}
}

static bool process_v(const std::string &line, 
	std::vector<glm::vec3> &v, 
	std::vector<glm::vec2> &vt, 
	std::vector<glm::vec3> &vn, 
	SRenderer::Mesh *out)
{
	glm::vec3 vv;
	if(sscanf(line.c_str(), "v %f %f %f", &vv.x, &vv.y, &vv.z)==3)
	{
		v.push_back(vv);
		return true;
	}

	return false;
}

static bool process_vt(const std::string &line, 
	std::vector<glm::vec3> &v, 
	std::vector<glm::vec2> &vt, 
	std::vector<glm::vec3> &vn, 
	SRenderer::Mesh *out)
{
	glm::vec2 tex;
	if(sscanf(line.c_str(), "vt %f %f", &tex.x, &tex.y)==2)
	{
		vt.push_back(tex);
		return true;
	}

	return false;
}

static bool process_vn(const std::string &line, 
	std::vector<glm::vec3> &v, 
	std::vector<glm::vec2> &vt, 
	std::vector<glm::vec3> &vn, 
	SRenderer::Mesh *out)
{
	glm::vec3 vnn;
	if(sscanf(line.c_str(), "vn %f %f %f", &vnn.x, &vnn.y, &vnn.z)==3)
	{
		vn.push_back(vnn);
		return true;
	}

	return false;
}

static bool process_f(const std::string &line, 
	std::vector<glm::vec3> &v, 
	std::vector<glm::vec2> &vt, 
	std::vector<glm::vec3> &vn, 
	SRenderer::Mesh *out)
{
	unsigned int o, a, b, ot, at, bt, on, an, bn;
	SRenderer::Face f;
	if(sscanf(line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u"
		, &o, &ot, &on, &a, &at, &an, &b, &bt, &bn)==9)
	{
		glm::vec3 oo=f.o.pos=v[o-1];
		glm::vec3 aa=f.a.pos=v[a-1];
		glm::vec3 bb=f.b.pos=v[b-1];

		f.o.texCoord=vt[ot-1];
		f.a.texCoord=vt[at-1];
		f.b.texCoord=vt[bt-1];

		glm::vec3 oa=aa-oo, ob=bb-oo;

		f.o.normal=vn[on-1];
		f.a.normal=vn[an-1];
		f.b.normal=vn[bn-1];
		
		out->faces.push_back(f);
		return true;
	}

	if(sscanf(line.c_str(), "f %u/%u %u/%u %u/%u", &o, &ot, &a, &at, &b, &bt)==6)
	{
		glm::vec3 oo=f.o.pos=v[o-1];
		glm::vec3 aa=f.a.pos=v[a-1];
		glm::vec3 bb=f.b.pos=v[b-1];

		f.o.texCoord=vt[ot-1];
		f.a.texCoord=vt[at-1];
		f.b.texCoord=vt[bt-1];

		glm::vec3 oa=aa-oo, ob=bb-oo;

		f.o.normal=f.a.normal=f.b.normal=glm::normalize(glm::cross(oa, ob));
		
		out->faces.push_back(f);
		return true;
	}

	if(sscanf(line.c_str(), "f %u %u %u", &o, &a, &b)==3)
	{
		glm::vec3 oo=f.o.pos=v[o-1];
		glm::vec3 aa=f.a.pos=v[a-1];
		glm::vec3 bb=f.b.pos=v[b-1];

		glm::vec3 oa=aa-oo, ob=bb-oo;

		f.o.normal=f.a.normal=f.b.normal=glm::normalize(glm::cross(oa, ob));
		
		out->faces.push_back(f);
		return true;
	}
	return false;
}