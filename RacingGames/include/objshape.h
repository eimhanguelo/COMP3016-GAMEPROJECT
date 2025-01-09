#ifndef _OBJ_SHAPE_H_
#define _OBJ_SHAPE_H_

#ifndef OS_LINUX
#pragma execution_character_set("utf-8")
#endif
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <vector>
#include <string>

class objPoint {
public:
	int id; 
	int tid;
	int nid;

private:

public:
	objPoint(int _id = 0, int _tid = 0, int _nid = 0);

	bool isTextureVector() const;

	bool isNormalVector() const;
};

class objTriangle {
public:

	objPoint points[3];
public:
	objTriangle();
	objTriangle(const objPoint&, const objPoint&, const objPoint&);
};


class objRect {
public:

	objPoint points[4];
public:
	objRect();
	objRect(const objPoint&, const objPoint&, const objPoint&, const objPoint&);
};

class objLine {
public:
	objPoint points[2];
public:
	objLine();
	objLine(const objPoint&, const objPoint&);
};

class objPoly {
public:
	std::string mtl;
	std::vector<objPoint> points;
public:
	void add(const objPoint &);
};



#endif
#pragma once
