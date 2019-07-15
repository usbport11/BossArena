#ifndef quadbufferH
#define quadbufferH

#include "../objects/PhysicQuad.h"

struct stQuadLink {
	stColorQuad* pColorQuad;
	unsigned int Shift;
	stQuadLink() {
		pColorQuad = NULL;
		Shift = 0;
	}
	stQuadLink(stColorQuad* inpColorQuad, unsigned int inShift) {
		pColorQuad = inpColorQuad;
		Shift = inShift;
	}
};

struct stFindQuadLink {
	stQuadLink QuadLink;
	stFindQuadLink(stColorQuad* inColorQuad) {
		QuadLink.pColorQuad = inColorQuad;
	}
	bool operator () (stQuadLink inQuadLink) {
		return QuadLink.pColorQuad == inQuadLink.pColorQuad;
	}
};

class MQuadBuffer {
private:
	GLuint VerticesId;
	GLuint ColorsId;
	GLenum Type;
	vector<glm::vec2> Vertices;
	vector<glm::vec3> Colors;
	vector<stQuadLink> Quads;
public:
	MQuadBuffer();
	MQuadBuffer(GLenum inType);
	bool AddQuad(stColorQuad* Quad);
	bool RemoveQuad(stColorQuad* Quad);
	bool UpdateQuad(stColorQuad* Quad);
	void UpdateAll();
	void Relocate();
	void DrawAll();
	void DrawQuad(stColorQuad* Quad);
	void Close();
	void Clear();
	bool IsReady();
};

#endif
