#ifndef _myModel_H_
#define _myModel_H_

#ifndef OS_LINUX
#include <Windows.h>
#include "gl/glut.h"
#pragma execution_character_set("utf-8")
#endif
#include <GL/glu.h>
#include <GL/gl.h>
#include "objshape.h"
#include <string>
#include <fstream>
#include <map>
#include <utility>
#include "glm/mat4x4.hpp"
#include "readmtl.h"
using namespace std;

class myModel {
public:
	myModel();
	void Draw();
	void DrawLines();
	void DrawPoints();
	void DrawMTL();
	void Rebuild();
	glm::vec3 GetVertexReal(int id);
	glm::vec3& GetVertex(int id);
	glm::vec3 GetVNReal(int id);
	glm::vec3& GetVN(int id);
	glm::vec2& GetVT(int id);
	void SetVertexPos(int id, double x, double y, double z);
	void MatMapVertices();
	void Save(const string filename);
	void BindTexture();
	void BindTexture(string);
public:
	int id;
	string name;
	string tex_name;
	string path;
	bool viewed;
	bool texed;
	std::vector<objPoly> ps; 
	std::vector<glm::vec3> vs; 
	std::vector<glm::vec3> vn; 
	std::vector<glm::vec2> vt; 
	map<string, MTL> mtls;
	glm::mat4 mat; 
	int triangleNum;
	int rectNum;
private:
	static int myModel_ID;
	static map<string, int> STR2TEX;
private:
	void DrawObjPoint(const objPoint &p);
	void BindMTL(string);
};

#endif
