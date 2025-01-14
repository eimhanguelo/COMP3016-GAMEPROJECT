#include "myModel.h"
#include "texture.h"

int myModel::myModel_ID = 1;
map<string, int> myModel::STR2TEX;

myModel::myModel() :mat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) {
	name = "undefined";
	id = myModel::myModel_ID++;
	viewed = true;
	texed = true;
}

void myModel::BindTexture() {
	if (!tex_name.empty() && texed) {
		if (!myModel::STR2TEX.count(tex_name)) {
			myModel::STR2TEX[tex_name] = LoadTexture(tex_name);
		}
		glBindTexture(GL_TEXTURE_2D, myModel::STR2TEX[tex_name]);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void myModel::BindTexture(string tex_name) {
	if (tex_name.empty()) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else {
		if (!myModel::STR2TEX.count(tex_name)) {
			myModel::STR2TEX[tex_name] = LoadTexture(tex_name);
		}
		glBindTexture(GL_TEXTURE_2D, myModel::STR2TEX[tex_name]);
	}
}

void myModel::Draw() {
	glColor4ub(156, 156, 170, 255);

	int k = 0;
	int endk = triangleNum;

	glBegin(GL_TRIANGLES);
	for (; k < endk; ++k) {
		objPoly &t = ps[k];
		for (int i = 0; i < 3; ++i)
			DrawObjPoint(t.points[i]);
	}
	glEnd();
	endk += rectNum;
	glBegin(GL_QUADS);
	for (; k < endk; ++k) {
		objPoly &t = ps[k];
		for (int i = 0; i < 4; ++i)
			DrawObjPoint(t.points[i]);
	}
	glEnd();
	endk = ps.size();
	for (; k < endk; ++k) {
		objPoly &t = ps[k];
		glBegin(GL_POLYGON);
		for (const objPoint &p : t.points) {
			DrawObjPoint(p);
		}
		glEnd();
	}

}

void myModel::BindMTL(string mtl_name) {
	if (mtls.count(mtl_name)) {
		MTL &mtl = mtls[mtl_name];
		BindTexture(path + mtl.map_Ka);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mtl.Ka);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mtl.Kd);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mtl.Ks);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void myModel::DrawMTL() {
	glColor4ub(156, 156, 170, 255);
	map<string, vector<objPoly> > st;
	map<string, vector<objPoly> > sr;
	map<string, vector<objPoly> > sp;

	int k = 0;
	int endk = triangleNum;
	for (; k < endk; ++k) {
		objPoly &t = ps[k];
		st[t.mtl].push_back(t);
	}
	endk += rectNum;
	for (; k < endk; ++k) {
		objPoly &t = ps[k];
		sr[t.mtl].push_back(t);
	}
	endk = ps.size();
	for (; k < endk; ++k) {
		objPoly &t = ps[k];
		sp[t.mtl].push_back(t);
	}

	for (auto &m : st) {
		BindMTL(m.first);
		glBegin(GL_TRIANGLES);
		for (objPoly &t : m.second) {
			for (int i = 0; i < 3; ++i)
				DrawObjPoint(t.points[i]);
		}
		glEnd();
	}

	for (auto &m : sr) {
		BindMTL(m.first);
		glBegin(GL_QUADS);
		for (objPoly &t : m.second) {
			for (int i = 0; i < 4; ++i)
				DrawObjPoint(t.points[i]);
		}
		glEnd();
	}

	for (auto &m : sp) {
		BindMTL(m.first);
		for (objPoly &t : m.second) {
			glBegin(GL_POLYGON);
			for (const objPoint &p : t.points) {
				DrawObjPoint(p);
			}
			glEnd();
		}
	}

}

glm::vec3& myModel::GetVertex(int id) {
	return vs[id - 1];
}
glm::vec3 myModel::GetVertexReal(int id) {
	glm::vec3 &ov = vs[id - 1];
	glm::vec4 cv = mat * glm::vec4(ov.x, ov.y, ov.z, 1);
	return glm::vec3(cv.x, cv.y, cv.z);
}
glm::vec3 myModel::GetVNReal(int id) {
	glm::vec3 &ov = vn[id - 1];
	glm::vec4 cv = mat * glm::vec4(ov.x, ov.y, ov.z, 0);
	return glm::vec3(cv.x, cv.y, cv.z);
}

glm::vec3& myModel::GetVN(int id) { return vn[id - 1]; }
glm::vec2& myModel::GetVT(int id) { return vt[id - 1]; }

void myModel::SetVertexPos(int id, double x, double y, double z) {
	glm::mat4 mati = glm::inverse(mat);
	glm::vec4 v(x, y, z, 1);
	glm::vec4 t = mati * v;
	vs[id - 1] = glm::vec3(t.x, t.y, t.z);
}

void myModel::DrawLines() {
	glLineWidth(1);


	for (const objPoly &t : ps) {
		glBegin(GL_LINE_LOOP);
		for (const objPoint &p : t.points) {
			DrawObjPoint(p);
		}
		glEnd();
	}
}

void myModel::DrawPoints() {
	glPointSize(3);
	glBegin(GL_POINTS);


	for (const objPoly &t : ps) {
		for (const objPoint &p : t.points) {
			DrawObjPoint(p);
		}
	}
	glEnd();
}

#include <iostream>
using namespace std;
void myModel::DrawObjPoint(const objPoint &p) {
	if (p.isTextureVector()) {
		glm::vec2 tv = GetVT(p.tid);
		glTexCoord2f(tv.x, tv.y);
	}
	if (p.isNormalVector()) {
		glm::vec3 nv = GetVNReal(p.nid);
		glNormal3f(nv.x, nv.y, nv.z);
	}
	glm::vec3 ov = GetVertex(p.id); 
	glm::vec4 cv = mat * glm::vec4(ov.x, ov.y, ov.z, 1);
	glVertex3f(cv.x, cv.y, cv.z);
}

void myModel::Rebuild() {
	vector<objPoly> triangles;
	vector<objPoly> rects;
	vector<objPoly> others;
	for (objPoly &p : ps) {
		if (p.points.size() < 3)continue;
		switch (p.points.size()) {
		case 3:
			triangles.push_back(p);
			break;
		case 4:
			rects.push_back(p);
			break;
		default:
			others.push_back(p);
		};
	}
	triangleNum = triangles.size();
	rectNum = rects.size();
	ps.resize(triangles.size() + rects.size() + others.size());
	int k = 0;
	for (objPoly &p : triangles) {
		ps[k++] = p;
	}
	for (objPoly &p : rects) {
		ps[k++] = p;
	}
	for (objPoly &p : others) {
		ps[k++] = p;
	}

}

void myModel::Save(const string filename) {
	ofstream fout(filename.c_str());
	for (glm::vec3 &v : vs) {
		fout << "v " << v.x << ' ' << v.y << ' ' << v.z << endl;
	}
	fout << "# " << vs.size() << "vertices" << endl;

	for (glm::vec2 &v : vt) {
		fout << "vt " << v.x << ' ' << v.y << endl;
	}
	fout << "# " << vt.size() << "texture vertices" << endl;

	for (glm::vec3 &v : vn) {
		fout << "vn " << v.x << ' ' << v.y << ' ' << v.z << endl;
	}
	fout << "# " << vs.size() << "vertices normals" << endl;
	for (objPoly &p : ps) {
		fout << "f ";
		for (objPoint &o : p.points) {
			fout << o.id;
			if (o.tid || o.nid) {
				fout << '/';
				if (o.tid)fout << o.tid;
				fout << '/';
				if (o.nid)fout << o.nid;
			}
			fout << ' ';
		}
		fout << endl;
	}
	fout.close();
}

void myModel::MatMapVertices() {
	for (size_t i = 1; i <= vs.size(); ++i) {
		vs[i - 1] = GetVertexReal(i);
	}
	for (size_t i = 1; i <= vn.size(); ++i) {
		vn[i - 1] = GetVNReal(i);
	}
	mat = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}
