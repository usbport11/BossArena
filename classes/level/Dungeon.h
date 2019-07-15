#ifndef dungeonH
#define dungeonH

#include "Leaf.h"
#include "../buffers/QuadBuffer.h"
#include "../objects/ContactListener.h"
#include "../objects/Chest.h"
#include "../objects/LevelExit.h"
#include "../objects/Enemy.h"
#include "../objects/Hero.h"

class MDungeon {
private:
	int LevelNumber;
	MHero* pHero;
	
	//tree
	MLeaf* LeafRoot;
	vector<MLeaf*> Leafs;
	map<NRectangle2*, vector<NRectangle2*> > AdjacencyTable;
	vector<NLine2> Lines;//edges
	
	//level
	glm::vec3 RandColor;
	GLuint VertexBuffer;
	GLuint ColorBuffer;
	vector<glm::vec2> VertexData;
	vector<glm::vec3> ColorData;
	
	//exits
	NRectangle2* LevelExitPrev;
	NRectangle2* LevelExitNext;
	MLevelExit* qLevelExitPrev;
	MLevelExit* qLevelExitNext;
	MQuadBuffer LevelExitBuffer;
	//chests
	vector<MChest*> Chests;
	MQuadBuffer ChestBuffer;
	//mobs
	vector<MEnemy*> Enemies;
	vector<MEnemy*>::iterator itv;
	MQuadBuffer EnemyBuffer;
	
	//tile and map
	glm::vec2 TileSize;
	unsigned int TilesCount[2];
	bool** Map;
	
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
	
	//dungeon create stages
	void CreateLevel();
	void CreateWalls();
	void CreateExits();
	void CreateChests();
	void CreateEnemies();
public:
	MDungeon();
	bool Initialize(unsigned int MapSizeX, unsigned int MapSizeY, float TileSizeX, float TileSizeY);
	bool Generate();
	void ProcessEnemies();
	void Draw();
	b2World* GetWorld();
	MLeaf* GetLeafRoot();
	float GetScale();
	void WorldStep();
	void Close();
	unsigned int GetLevelNumber();
	NRectangle2* GetLevelExitPrev();
	void SetHero(MHero* inpHero);
};

#endif
