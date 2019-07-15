#ifndef arenaH
#define arenaH

#include "../objects/ContactListener.h"

class MArena {
private:
	//map data
	glm::vec2 TileSize;
	unsigned int TilesCount[2];
	
	//level
	glm::vec3 RandColor;
	GLuint VertexBuffer;
	GLuint ColorBuffer;
	vector<glm::vec2> VertexData;
	vector<glm::vec3> ColorData;
	
	//physic
	b2World* World;
	MContactListener ContactListener;
	b2Vec2 Gravity;
	float timeStep;
	int velocityIterations;
	int positionIterations;
	float PhysicScale;//100 pixel - 1 meter
	b2Body* staticBody;
	vector<b2Fixture*> staticFixtures;
	
public:
	MArena();
	bool Initialize(unsigned int TilesCountX, unsigned int TilesCountY, float TileSizeX, float TileSizeY);
	bool CreateSimple();
	void Draw();
	void WorldStep();
	void Close();
	float GetScale();
	unsigned int* GetTilesCount();
	glm::vec2 GetTileSize();
	b2World* GetWorld();
};

#endif
