#include "../../stdafx.h"
#include "Arena.h"

MArena::MArena() {
	TileSize = glm::vec2(0, 0);
	TilesCount[0] = TilesCount[1] = 0;
	RandColor = glm::vec3(1, 1, 1);
	
	VertexBuffer = 0;
	ColorBuffer = 0;
	
	Gravity = b2Vec2(0, 0);
	timeStep = 1 / 60.0;
	velocityIterations = 8;
	positionIterations = 3;
	PhysicScale = 0.01;
	
	World = NULL;
	staticBody = NULL;
}

bool MArena::Initialize(unsigned int TilesCountX, unsigned int TilesCountY, float TileSizeX, float TileSizeY) {
	if(!TilesCountX || !TilesCountY || TileSizeX < 0 || TileSizeY < 0) return false;
	TileSize.x = TileSizeX;
	TileSize.y = TileSizeY;
	TilesCount[0] = TilesCountX;
	TilesCount[1] = TilesCountY;
	
	if(VertexBuffer <= 0) glGenBuffers(1, &VertexBuffer);
	if(ColorBuffer <= 0) glGenBuffers(1, &ColorBuffer);
	
	//create world
	World = new b2World(Gravity);
	World->SetContactListener(&ContactListener);
	
	return true;
}

bool MArena::CreateSimple() {
	//graphic (simple white quad)
	VertexData.clear();
	ColorData.clear();
	VertexData.push_back(glm::vec2(0, 0));
	VertexData.push_back(glm::vec2(TilesCount[0] * TileSize.x, 0));
	VertexData.push_back(glm::vec2(TilesCount[0] * TileSize.x, TilesCount[1] * TileSize.y));
	VertexData.push_back(glm::vec2(0, TilesCount[1] * TileSize.y));
	ColorData.push_back(RandColor);
	ColorData.push_back(RandColor);
	ColorData.push_back(RandColor);
	ColorData.push_back(RandColor);
		
	//clear and fill buffers
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2), NULL, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * VertexData.size(), &VertexData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ColorData.size(), &ColorData[0], GL_STATIC_DRAW);

	//physic
	b2BodyDef groundDef;
	b2EdgeShape edgeShape;
	b2FixtureDef fixtureDef;
	
	//level edges
	groundDef.position.Set(0.0f, 0.0f);
	groundDef.type = b2_staticBody;
	staticBody = World->CreateBody(&groundDef);
	
	//left
	edgeShape.Set(b2Vec2(0, 0), b2Vec2(0, TileSize.y * TilesCount[1] * PhysicScale));
	fixtureDef.shape = &edgeShape;
	staticFixtures.push_back(staticBody->CreateFixture(&fixtureDef));
	//right
	edgeShape.Set(b2Vec2(TileSize.x * TilesCount[0] * PhysicScale, 0), b2Vec2(TileSize.x * TilesCount[0] * PhysicScale, TileSize.y * TilesCount[1] * PhysicScale));
	fixtureDef.shape = &edgeShape;
	staticFixtures.push_back(staticBody->CreateFixture(&fixtureDef));
	//bottom
	edgeShape.Set(b2Vec2(0, 0), b2Vec2(TileSize.x * TilesCount[0] * PhysicScale, 0));
	fixtureDef.shape = &edgeShape;
	staticFixtures.push_back(staticBody->CreateFixture(&fixtureDef));
	//top
	edgeShape.Set(b2Vec2(0, TileSize.y * TilesCount[1] * PhysicScale), b2Vec2(TileSize.x * TilesCount[0] * PhysicScale, TileSize.y * TilesCount[1] * PhysicScale));
	fixtureDef.shape = &edgeShape;
	staticFixtures.push_back(staticBody->CreateFixture(&fixtureDef));
	
	return true;
}

void MArena::Draw() {
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(SHR_LAYOUT_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer);
	glVertexAttribPointer(SHR_LAYOUT_COLOR, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_QUADS, 0, VertexData.size());
}

void MArena::WorldStep() {
	World->Step(timeStep, velocityIterations, positionIterations);
}

void MArena::Close() {
	if(World) {
		if(staticBody) {
			for(int i=0; i<staticFixtures.size(); i++) {
				if(staticFixtures[i]) staticBody->DestroyFixture(staticFixtures[i]);
			}
			staticFixtures.clear();
			World->DestroyBody(staticBody);
		}
		delete World;
	}
	
	glDeleteBuffers(1, &VertexBuffer);
	glDeleteBuffers(1, &ColorBuffer);
	VertexData.clear();
	ColorData.clear();
}

float MArena::GetScale() {
	return PhysicScale;
}

unsigned int* MArena::GetTilesCount() {
	return (unsigned int*)TilesCount;
}

glm::vec2 MArena::GetTileSize() {
	return TileSize;
}

b2World* MArena::GetWorld() {
	return World;
}
