#include "../../stdafx.h"
#include "Dungeon.h"

void MDungeon::CreateLevel() {
	//clear buffers data
	VertexData.clear();
	ColorData.clear();
	//clear adjency table
	AdjacencyTable.clear();
	//clear tree
	for(int i=0; i<Leafs.size(); i++) {
		Leafs[i]->Halls.clear();
		if(Leafs[i]->Room) delete Leafs[i]->Room;
		if(Leafs[i]->LeafLeft) delete Leafs[i]->LeafLeft;
		if(Leafs[i]->LeafRight) delete Leafs[i]->LeafRight;
	}
	if(LeafRoot) delete LeafRoot;
	Leafs.clear();
	//clear map
	for(unsigned int i=0; i<TilesCount[1]; i++) {
		memset(Map[i], 0, TilesCount[0]);
	}
	cout<<"Map size: "<<sizeof(Map)<<endl;
	
	//fill tree
	LeafRoot = new MLeaf(NVector2(0,0), NVector2(TilesCount[0], TilesCount[1]));
	Leafs.push_back(LeafRoot);
	bool SplitSuccess = true;
	int LeafsCount = 0;
	while(SplitSuccess) {
		SplitSuccess = false;
		LeafsCount = Leafs.size();
		for(int i=0; i<LeafsCount; i++) {
			if(!Leafs[i]->LeafLeft && !Leafs[i]->LeafRight) {
				if (Leafs[i]->Size.x > MAX_LEAF_SIZE_2 || Leafs[i]->Size.y > MAX_LEAF_SIZE_2 || rand() % 100 > 25) {
					if(Leafs[i]->Split()) {
						Leafs.push_back(Leafs[i]->LeafLeft);
						Leafs.push_back(Leafs[i]->LeafRight);
						SplitSuccess = true;
					}
				}
			}
		}
	}
	//create rooms
	LeafRoot->CreateRooms();
	
	//create adjecency table
	map<NRectangle2*, vector<NRectangle2*> >::iterator it_map;
	vector<NRectangle2*>::iterator it_vec;
	vector<NRectangle2*> Add;
	for(int i=0; i<LeafsCount; i++) {
		if(Leafs[i]->ConnectedRooms[0] && Leafs[i]->ConnectedRooms[1]) {
			//0 - 1
			if((it_map = AdjacencyTable.find(Leafs[i]->ConnectedRooms[0])) == AdjacencyTable.end()) {
				Add.clear();
				Add.push_back(Leafs[i]->ConnectedRooms[1]);
				AdjacencyTable.insert(pair<NRectangle2*, vector<NRectangle2*> >(Leafs[i]->ConnectedRooms[0], Add));
			}
			else {
				it_vec = find(AdjacencyTable[Leafs[i]->ConnectedRooms[0]].begin(), AdjacencyTable[Leafs[i]->ConnectedRooms[0]].end(), Leafs[i]->ConnectedRooms[1]);
				if(it_vec == AdjacencyTable[Leafs[i]->ConnectedRooms[0]].end()) AdjacencyTable[Leafs[i]->ConnectedRooms[0]].push_back(Leafs[i]->ConnectedRooms[1]);
			}
			//1 - 0
			if((it_map = AdjacencyTable.find(Leafs[i]->ConnectedRooms[1])) == AdjacencyTable.end()) {
				Add.clear();
				Add.push_back(Leafs[i]->ConnectedRooms[0]);
				AdjacencyTable.insert(pair<NRectangle2*, vector<NRectangle2*> >(Leafs[i]->ConnectedRooms[1], Add));
			}
			else {
				it_vec = find(AdjacencyTable[Leafs[i]->ConnectedRooms[1]].begin(), AdjacencyTable[Leafs[i]->ConnectedRooms[1]].end(), Leafs[i]->ConnectedRooms[0]);
				if(it_vec == AdjacencyTable[Leafs[i]->ConnectedRooms[1]].end()) AdjacencyTable[Leafs[i]->ConnectedRooms[1]].push_back(Leafs[i]->ConnectedRooms[0]);
			}
		}
	}
	
	//fill map
	RandColor = glm::vec3(1, 1, 1);
	LeafsCount = Leafs.size();
	for(int i=0; i<LeafsCount; i++) {
		if(Leafs[i]->Room) {
			for(int j=Leafs[i]->Room->Position.y; j<Leafs[i]->Room->Position.y + Leafs[i]->Room->Size.y; j++) {
				for(int k=Leafs[i]->Room->Position.x; k<Leafs[i]->Room->Position.x + Leafs[i]->Room->Size.x; k++) {
					Map[j][k] = 1;
				}
			}
		}
		for(int j=0; j<Leafs[i]->Halls.size(); j++) {
			for(int k=Leafs[i]->Halls[j].Position.y; k<Leafs[i]->Halls[j].Position.y + Leafs[i]->Halls[j].Size.y; k++) {
				for(int l=Leafs[i]->Halls[j].Position.x; l<Leafs[i]->Halls[j].Position.x + Leafs[i]->Halls[j].Size.x; l++) {
					Map[k][l] = 1;
				}
			}
		}
	}
	//map
	for(int i=0; i<TilesCount[1]; i++) {
		for(int j=0; j<TilesCount[0]; j++) {
			if(!Map[i][j]) {
				VertexData.push_back(glm::vec2(j * TileSize.x, i * TileSize.y));
				VertexData.push_back(glm::vec2((j + 1) * TileSize.x, i * TileSize.y));
				VertexData.push_back(glm::vec2((j + 1) * TileSize.x, (i + 1) * TileSize .y));
				VertexData.push_back(glm::vec2(j * TileSize.x, (i + 1) * TileSize.y));
				ColorData.push_back(RandColor);
				ColorData.push_back(RandColor);
				ColorData.push_back(RandColor);
				ColorData.push_back(RandColor);
			}
		}
	}
	
	//exits
	LevelExitPrev = NULL;
	LevelExitNext = NULL;
	for(it_map = AdjacencyTable.begin(); it_map != AdjacencyTable.end(); ++it_map) {
		if(LevelExitPrev && LevelExitNext) break;
		if(it_map->second.size() == 1) {
			if(!LevelExitPrev) LevelExitPrev = it_map->first;
			else if(!LevelExitNext) LevelExitNext = it_map->first;
		}
	}
	
	//clear and fill buffers
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2), NULL, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * VertexData.size(), &VertexData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ColorData.size(), &ColorData[0], GL_STATIC_DRAW);
}

void MDungeon::CreateWalls() {
	//clear data
	Lines.clear();
	
	NLine2 Line1, Line2;
	//get horizontal edges
	for(int i=0; i<TilesCount[1] - 1; i++) {
		for(int j=0; j<TilesCount[0]; j++) {
			if(!Map[i][j] && Map[i+1][j]) {
				if(!Line1.a.x ) {
					Line1.a.x = j;
					Line1.b.x = j;
					Line1.a.y = i+1;
					Line1.b.y = i+1;
				}
				else Line1.b.x ++;
			}
			else {
				if(Line1.a.x) {
					Line1.b.x ++;
					Lines.push_back(Line1);
					Line1.a.x = 0;
				}
			}
			if(Map[i][j] && !Map[i+1][j]) {
				if(!Line2.a.x ) {
					Line2.a.x = j;
					Line2.b.x = j;
					Line2.a.y = i+1;
					Line2.b.y = i+1;
				}
				else Line2.b.x ++;
			}
			else {
				if(Line2.a.x) {
					Line2.b.x ++;
					Lines.push_back(Line2);
					Line2.a.x = 0;
				}
			}
		}
	}
	//get vertical edges
	for(int i=0; i<TilesCount[0] - 1; i++) {
		for(int j=0; j<TilesCount[1]; j++) {
			if(!Map[j][i] && Map[j][i+1]) {
				if(!Line1.a.y) {
					Line1.a.y = j;
					Line1.b.y = j;
					Line1.a.x = i+1;
					Line1.b.x = i+1;
				}
				else Line1.b.y ++;
			}
			else {
				if(Line1.a.y) {
					Line1.b.y ++;
					Lines.push_back(Line1);
					Line1.a.y = 0;
				}
			}
			if(Map[j][i] && !Map[j][i+1]) {
				if(!Line2.a.y ) {
					Line2.a.y = j;
					Line2.b.y = j;
					Line2.a.x = i+1;
					Line2.b.x = i+1;
				}
				else Line2.b.y ++;
			}
			else {
				if(Line2.a.y) {
					Line2.b.y ++;
					Lines.push_back(Line2);
					Line2.a.y = 0;
				}
			}
		}
	}
	
	b2BodyDef groundDef;
	b2EdgeShape edgeShape;
	b2FixtureDef fixtureDef;
	
	if(staticBody) {
		for(int i=0; i<staticFixtures.size(); i++) {
			if(staticFixtures[i]) staticBody->DestroyFixture(staticFixtures[i]);
		}
		staticFixtures.clear();
		World->DestroyBody(staticBody);
	}
	
	//level edges
	groundDef.position.Set(0.0f, 0.0f);
	groundDef.type = b2_staticBody;
	staticBody = World->CreateBody(&groundDef);
	
	for(int i=0; i<Lines.size(); i++) {
		if(Lines[i].a.x) {
			edgeShape.Set(b2Vec2(Lines[i].a.x * TileSize.x * PhysicScale, Lines[i].a.y * TileSize.y * PhysicScale), 
				b2Vec2(Lines[i].b.x * TileSize.x * PhysicScale, Lines[i].b.y * TileSize.y * PhysicScale));
			fixtureDef.shape = &edgeShape;
			staticFixtures.push_back(staticBody->CreateFixture(&fixtureDef));
		}
	}
}

void MDungeon::CreateExits() {
	NVector2 c1, c2;
	c1 = NVector2(LevelExitPrev->Position.x + LevelExitPrev->Size.x * 0.5, LevelExitPrev->Position.y + LevelExitPrev->Size.y * 0.5);
	c2 = NVector2(LevelExitNext->Position.x + LevelExitNext->Size.x * 0.5, LevelExitNext->Position.y + LevelExitNext->Size.y * 0.5);
	
	LevelExitBuffer.Clear();
	qLevelExitPrev->Close();
	qLevelExitNext->Close();
	if(LevelNumber > 0) {
		qLevelExitPrev->Create(glm::vec2((c1.x - 0.5) * TileSize.x, (c1.y - 0.5) * TileSize.x), 
			glm::vec2(TileSize.x, TileSize.y), glm::vec3(0.5, 0, 0), PhysicScale, 
			World, b2_kinematicBody, OT_LEPREV, OT_HERO, true);
		LevelExitBuffer.AddQuad(qLevelExitPrev->GetColorQuad());
	}
	qLevelExitNext->Create(glm::vec2((c2.x - 0.5) * TileSize.x, (c2.y - 0.5) * TileSize.x), 
		glm::vec2(TileSize.x, TileSize.y), glm::vec3(1, 0, 0), PhysicScale, 
		World, b2_kinematicBody, OT_LENEXT, OT_HERO, true);
	LevelExitBuffer.AddQuad(qLevelExitNext->GetColorQuad());
	LevelExitBuffer.Relocate();
}

void MDungeon::CreateChests() {
	//clear
	ChestBuffer.Clear();
	for(int i=0; i<Chests.size(); i++) {
		if(Chests[i]) {
			Chests[i]->Close();
			delete Chests[i];
		}
	}
	Chests.clear();
	
	NVector2 c;
	for(int i=0; i<Leafs.size(); i++) {
		if(Leafs[i]->Room && Leafs[i]->Room != LevelExitPrev && Leafs[i]->Room != LevelExitNext && RandomChance(3)) {
			c = NVector2(Leafs[i]->Room->Position.x + 1 + rand() % (Leafs[i]->Room->Size.x - 1), Leafs[i]->Room->Position.y + 1 + rand() % (Leafs[i]->Room->Size.y - 1));
			Chests.push_back(new MChest);
			Chests.back()->Create(glm::vec2((c.x - 0.1) * TileSize.x, (c.y - 0.1) * TileSize.x), 
				glm::vec2(0.2 * TileSize.x, 0.2 * TileSize.y), glm::vec3(1, 1, 0), PhysicScale, 
				World, b2_kinematicBody, OT_CHEST, OT_HERO, true);
			ChestBuffer.AddQuad(Chests.back()->GetColorQuad());
		}
	}
	ChestBuffer.Relocate();
}

void MDungeon::CreateEnemies() {
	EnemyBuffer.Clear();
	for(int i=0; i<Enemies.size(); i++) {
		if(Enemies[i]) {
			Enemies[i]->Close();
			delete Enemies[i];
		}
	}
	Enemies.clear();
	
	//need extended check on positions
	NVector2 c;
	int MobsCount;
	for(int i=0; i<Leafs.size(); i++) {
		if(Leafs[i]->Room && Leafs[i]->Room != LevelExitPrev && Leafs[i]->Room != LevelExitNext) {
			MobsCount = rand() % 3 + 1;
			for(int j=0; j<MobsCount; j++) {
				c = NVector2(Leafs[i]->Room->Position.x + 1 + rand() % (Leafs[i]->Room->Size.x - 1), Leafs[i]->Room->Position.y + 1 + rand() % (Leafs[i]->Room->Size.y - 1));
				Enemies.push_back(new MEnemy);
				Enemies.back()->Create(glm::vec2((c.x - 0.2) * TileSize.x, (c.y - 0.2) * TileSize.x), 
					glm::vec2(0.4 * TileSize.x, 0.4 * TileSize.y), glm::vec3(0, 0, 1), PhysicScale, 
					World, b2_dynamicBody, OT_ENBODY, OT_WALL, false);
				EnemyBuffer.AddQuad(Enemies.back()->GetColorQuad());
				Enemies.back()->SetVelocity(b2Vec2(0.2 * ((rand() % 2) * 2 - 1), 0.2 * ((rand() % 2) * 2 - 1)));
				Enemies.back()->SetHealth(10);
				Enemies.back()->SetExpirience(15);
			}
		}
	}
	EnemyBuffer.Relocate();
}

MDungeon::MDungeon() {
	Map = NULL;
	TileSize = glm::vec2(0.0f, 0.0f);
	TilesCount[0] = TilesCount[1] = 0;
	VertexBuffer = 0;
	ColorBuffer = 0;
	
	Gravity = b2Vec2(0, 0);
	timeStep = 1 / 60.0;
	velocityIterations = 8;
	positionIterations = 3;
	PhysicScale = 0.01;
	World = NULL;
	staticBody = NULL;
	
	LevelNumber = -1;
	
	qLevelExitPrev = NULL;
	qLevelExitNext = NULL;
	pHero = NULL;
}

bool MDungeon::Initialize(unsigned int MapSizeX, unsigned int MapSizeY, float TileSizeX, float TileSizeY) {
	if(TileSizeX <= 0 || TileSizeY <= 0) return false;
	
	TilesCount[0] = MapSizeX;
	TilesCount[1] = MapSizeY;
	TileSize = glm::vec2(TileSizeX, TileSizeY);
	if(!Map) {
		Map = new bool* [MapSizeY];
		for(unsigned int i=0; i<MapSizeY; i++) {
			Map[i] = new bool [MapSizeX];
		}
	}
	
	if(VertexBuffer <= 0) glGenBuffers(1, &VertexBuffer);
	if(ColorBuffer <= 0) glGenBuffers(1, &ColorBuffer);
	
	LevelExitBuffer = MQuadBuffer(GL_STATIC_DRAW);
	ChestBuffer = MQuadBuffer(GL_STATIC_DRAW);
	EnemyBuffer = MQuadBuffer(GL_STREAM_DRAW);
	
	World = new b2World(Gravity);
	World->SetContactListener(&ContactListener);
	
	qLevelExitPrev = new MLevelExit;
	qLevelExitNext = new MLevelExit;
	
	return true;
}

bool MDungeon::Generate() {
	if(!World || !qLevelExitPrev || !qLevelExitNext) return false;
	if(!Map) return false;
	
	LevelNumber ++;
	CreateLevel();
	CreateWalls();
	CreateExits();
	CreateChests();
	CreateEnemies();
	
	return true;
}

void MDungeon::ProcessEnemies(){
	itv = Enemies.begin();
	while(itv != Enemies.end()) {
		if((*itv)->GetNeedRemove()) {
			if(pHero) {
				pHero->AddExpirience((*itv)->GetExpirience());
				cout<<"Exp +"<<(*itv)->GetExpirience()<<endl;
			}
			EnemyBuffer.RemoveQuad((*itv)->GetColorQuad());
			(*itv)->Close();
			delete *itv;
			*itv = NULL;
			itv = Enemies.erase(itv);
		}
		else {
			(*itv)->Update();
			(*itv)->Move();
			++ itv;
		}
	}
	EnemyBuffer.UpdateAll();
}

void MDungeon::Draw() {
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(SHR_LAYOUT_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer);
	glVertexAttribPointer(SHR_LAYOUT_COLOR, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_QUADS, 0, VertexData.size());
	
	LevelExitBuffer.DrawAll();
	ChestBuffer.DrawAll();
	EnemyBuffer.DrawAll();
}

b2World* MDungeon::GetWorld() {
	return World;
}

MLeaf* MDungeon::GetLeafRoot() {
	return LeafRoot;
}

float MDungeon::GetScale() {
	return PhysicScale;
}

void MDungeon::WorldStep() {
	World->Step(timeStep, velocityIterations, positionIterations);
}

void MDungeon::Close() {
	if(qLevelExitPrev) {
		qLevelExitPrev->Close();
		delete qLevelExitPrev;
	}
	if(qLevelExitNext) {
		qLevelExitNext->Close();
		delete qLevelExitNext;
	}
	for(int i=0; i<Chests.size(); i++) {
		if(Chests[i]) {
			Chests[i]->Close();
			delete Chests[i];
		}
	}
	Chests.clear();
	for(int i=0; i<Enemies.size(); i++) {
		if(Enemies[i]) {
			Enemies[i]->Close();
			delete Enemies[i];
		}
	}
	Enemies.clear();
	
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
	LevelExitBuffer.Close();
	ChestBuffer.Close();

	for(unsigned int i=0; i<TilesCount[1]; i++) {
		if(Map[i]) delete [] Map[i];
	}
	if(Map) delete [] Map;
	
	VertexData.clear();
	ColorData.clear();
	Lines.clear();
	
	AdjacencyTable.clear();
	
	for(int i=0; i<Leafs.size(); i++) {
		Leafs[i]->Halls.clear();
		if(Leafs[i]->Room) delete Leafs[i]->Room;//SUSPISIOUS!
		if(Leafs[i]->LeafLeft) delete Leafs[i]->LeafLeft;
		if(Leafs[i]->LeafRight) delete Leafs[i]->LeafRight;
	}
	if(LeafRoot) delete LeafRoot;
	Leafs.clear();
}

unsigned int MDungeon::GetLevelNumber() {
	return LevelNumber;
}

NRectangle2* MDungeon::GetLevelExitPrev() {
	return LevelExitPrev;
}

void MDungeon::SetHero(MHero* inpHero) {
	pHero = inpHero;
}
