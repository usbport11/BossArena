#include "../../stdafx.h"
#include "QuadBuffer.h"

MQuadBuffer::MQuadBuffer() {
	VerticesId = 0;
	ColorsId = 0;
}

MQuadBuffer::MQuadBuffer(GLenum inType) {
	Type = inType;
	glGenBuffers(1, &VerticesId);
	glBindBuffer(GL_ARRAY_BUFFER, VerticesId);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec2), NULL, Type);
	glGenBuffers(1, &ColorsId);
	glBindBuffer(GL_ARRAY_BUFFER, ColorsId);
	glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(glm::vec3), NULL, Type);
}

bool MQuadBuffer::AddQuad(stColorQuad* Quad) {
	if(!Quad) return false;
	vector<stQuadLink>::iterator it = find_if(Quads.begin(), Quads.end(), stFindQuadLink(Quad));
	if(it != Quads.end()) return false;
	if(it != Quads.end()) {
		UpdateQuad(Quad);
		return true;
	}
	Quads.push_back(stQuadLink(Quad, Vertices.size()));
	Vertices.push_back(Quad->v[0]);
	Vertices.push_back(Quad->v[1]);
	Vertices.push_back(Quad->v[2]);
	Vertices.push_back(Quad->v[3]);
	Colors.push_back(Quad->c[0]);
	Colors.push_back(Quad->c[1]);
	Colors.push_back(Quad->c[2]);
	Colors.push_back(Quad->c[3]);
	return true;
}

bool MQuadBuffer::RemoveQuad(stColorQuad* Quad) {
	if(!Quad) return false;
	vector<stQuadLink>::iterator it = find_if(Quads.begin(), Quads.end(), stFindQuadLink(Quad));
	if(it != Quads.end()) {
		vector<stQuadLink>::iterator itSave = it;
		for(itSave++; itSave != Quads.end(); itSave++) {
			itSave->Shift -= 4;
		}
		Vertices.erase(Vertices.begin() + it->Shift, Vertices.begin() + it->Shift + 4);
		Colors.erase(Colors.begin() + it->Shift, Colors.begin() + it->Shift + 4);
		Quads.erase(it);
	}
	return true;
}

bool MQuadBuffer::UpdateQuad(stColorQuad* Quad) {
	if(!Quad) return false;
	vector<stQuadLink>::iterator it = find_if(Quads.begin(), Quads.end(), stFindQuadLink(Quad));
	if(it == Quads.end()) return false;
	Vertices[it->Shift] = Quad->v[0];
	Vertices[it->Shift + 1] = Quad->v[1];
	Vertices[it->Shift + 2] = Quad->v[2];
	Vertices[it->Shift + 3] = Quad->v[3];
	Colors[it->Shift] = Quad->c[0];
	Colors[it->Shift + 1] = Quad->c[1];
	Colors[it->Shift + 2] = Quad->c[2];
	Colors[it->Shift + 3] = Quad->c[3];
	glBindBuffer(GL_ARRAY_BUFFER, VerticesId);
	glBufferSubData(GL_ARRAY_BUFFER, it->Shift * sizeof(glm::vec2), sizeof(glm::vec2) * 4, &Vertices[it->Shift][0]);
	glBindBuffer(GL_ARRAY_BUFFER, ColorsId);
	glBufferSubData(GL_ARRAY_BUFFER, it->Shift * sizeof(glm::vec3), sizeof(glm::vec3) * 4, &Colors[it->Shift][0]);
	return true;
}

void MQuadBuffer::UpdateAll() {
	for(int i=0; i < Quads.size(); i ++) {
		Vertices[Quads[i].Shift] = Quads[i].pColorQuad->v[0];
		Vertices[Quads[i].Shift + 1] = Quads[i].pColorQuad->v[1];
		Vertices[Quads[i].Shift + 2] = Quads[i].pColorQuad->v[2];
		Vertices[Quads[i].Shift + 3] = Quads[i].pColorQuad->v[3];
		Colors[Quads[i].Shift] = Quads[i].pColorQuad->c[0];
		Colors[Quads[i].Shift + 1] = Quads[i].pColorQuad->c[1];
		Colors[Quads[i].Shift + 2] = Quads[i].pColorQuad->c[2];
		Colors[Quads[i].Shift + 3] = Quads[i].pColorQuad->c[3];
	}
	Relocate();
}

void MQuadBuffer::Relocate() {
	glBindBuffer(GL_ARRAY_BUFFER, VerticesId);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec2), NULL, Type);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec2), &Vertices[0][0], Type);
	glBindBuffer(GL_ARRAY_BUFFER, ColorsId);
	glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(glm::vec3), NULL, Type);
	glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(glm::vec3), &Colors[0][0], Type);
}

void MQuadBuffer::DrawAll() {
	glBindBuffer(GL_ARRAY_BUFFER, VerticesId);
	glVertexAttribPointer(SHR_LAYOUT_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, ColorsId);
	glVertexAttribPointer(SHR_LAYOUT_COLOR, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_QUADS, 0, Vertices.size());
}

void MQuadBuffer::DrawQuad(stColorQuad* Quad) {
	if(!Quad) return;
	vector<stQuadLink>::iterator it = find_if(Quads.begin(), Quads.end(), stFindQuadLink(Quad));
	if(it == Quads.end()) return;
	glBindBuffer(GL_ARRAY_BUFFER, VerticesId);
	glVertexAttribPointer(SHR_LAYOUT_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, ColorsId);
	glVertexAttribPointer(SHR_LAYOUT_COLOR, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_QUADS, it->Shift, 4);
}

void MQuadBuffer::Clear() {
	if(!IsReady()) return;
	Vertices.clear();
	Colors.clear();
	Quads.clear();
	glBindBuffer(GL_ARRAY_BUFFER, VerticesId);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec2), NULL, Type);
	glBindBuffer(GL_ARRAY_BUFFER, ColorsId);
	glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(glm::vec2), NULL, Type);
}

void MQuadBuffer::Close() {
	Clear();
	glDeleteBuffers(1, &VerticesId);
	glDeleteBuffers(1, &ColorsId);
}

bool MQuadBuffer::IsReady() {
	return (VerticesId | ColorsId);
}
