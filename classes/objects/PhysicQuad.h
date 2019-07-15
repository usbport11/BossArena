#ifndef physicquadH
#define physicquadH

#define OT_WALL 1
#define OT_HERO 2
#define OT_CHEST 4
#define OT_LEPREV 8
#define OT_LENEXT 16
#define OT_EXIT OT_LEPREV | OT_LENEXT
#define OT_ENBODY 32
#define OT_ENVISION 64
#define OT_ENEMY OT_ENBODY | OT_ENVISION
#define OT_BULLET 128
#define OT_ZONE 256

struct stColorQuad {
	glm::vec2 v[4];
	glm::vec3 c[4];
	stColorQuad() {
		v[0] = v[1] = v[2] = v[3] = glm::vec2(0, 0);
		c[0] = c[1] = c[2] = c[3] = glm::vec3(1, 1, 1);
	}
	stColorQuad(glm::vec2 Position, glm::vec2 Size, glm::vec3 Color) {
		v[0] = glm::vec2(Position.x, Position.y);
		v[1] = glm::vec2(Position.x + Size.x, Position.y);
		v[2] = glm::vec2(Position.x + Size.x, Position.y + Size.y);
		v[3] = glm::vec2(Position.x, Position.y + Size.y);
		c[0] = c[1] = c[2] = c[3] = Color;
	}
	stColorQuad(float CenterX, float CenterY, float Width, float Height, glm::vec3 Color) {
		glm::vec2 pos = glm::vec2(CenterX - Width / 2, CenterY - Height / 2);
		v[0] = glm::vec2(pos.x, pos.y);
		v[1] = glm::vec2(pos.x + Width, pos.y);
		v[2] = glm::vec2(pos.x + Width, pos.y + Height);
		v[3] = glm::vec2(pos.x, pos.y + Height);
		c[0] = c[1] = c[2] = c[3] = Color;
	}
	glm::vec2 GetCenter() {
		return glm::vec2(v[0].x + (v[2].x - v[0].x) / 2, v[0].y + (v[2].y - v[0].y) / 2);
	}
	glm::vec2 GetSize() {
		return glm::vec2(v[2].x - v[0].x, v[2].y - v[0].y);
	}
	glm::vec2 GetMinPoint() {
		glm::vec2 Min = v[0];
		Min.x = glm::min(Min.x, v[1].x);
		Min.x = glm::min(Min.x, v[2].x);
		Min.x = glm::min(Min.x, v[3].x);
		Min.y = glm::min(Min.y, v[1].y);
		Min.y = glm::min(Min.y, v[2].y);
		Min.y = glm::min(Min.y, v[3].y);
		return Min;
	}
	glm::vec2 GetMaxPoint() {
		glm::vec2 Max = v[0];
		Max.x = glm::max(Max.x, v[1].x);
		Max.x = glm::max(Max.x, v[2].x);
		Max.x = glm::max(Max.x, v[3].x);
		Max.y = glm::max(Max.y, v[1].y);
		Max.y = glm::max(Max.y, v[2].y);
		Max.y = glm::max(Max.y, v[3].y);
		return Max;
	}
	void Move(glm::vec2 Shift)
	{
		v[0] += Shift;
		v[1] += Shift;
		v[2] += Shift;
		v[3] += Shift;
	}
	glm::vec2 GetCenter2()
	{
		glm::vec2 Min = GetMinPoint();
		glm::vec2 Max = GetMaxPoint();
		return glm::vec2((Min.x + Max.x) * 0.5f, (Min.y + Max.y) * 0.5f);
	}
	void Rotate(float Angle)
	{
		glm::vec2 Center = GetCenter2();
		Move(-Center);
		v[0] = glm::rotate(v[0], Angle);
		v[1] = glm::rotate(v[1], Angle);
		v[2] = glm::rotate(v[2], Angle);
		v[3] = glm::rotate(v[3], Angle);
		Move(Center);
	}
	void Size(glm::vec2 Value, glm::vec2 Direction) {
	}
};

class MPhysicQuad {
private:
	stColorQuad ColorQuad;
	glm::vec2 Size;
	float Scale;
	b2World* pWorld;
protected:
	b2Body* Body;
	b2Fixture* Fixture;
	b2PolygonShape* Shape;
	bool NeedRemove;
public:
	MPhysicQuad();
	void Create(glm::vec2 Position, glm::vec2 inSize, glm::vec3 Color, float inScale, b2World* inpWorld, b2BodyType Type, uint16 FilterCategory, uint16 FilterMask, bool Sensor);
	void SetVelocity(b2Vec2 Velocity);
	void Update();
	void Close();
	bool GetNeedRemove();
	stColorQuad* GetColorQuad();
	b2Body* GetBody();
	
	virtual void OnBeginCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type);
	virtual void OnEndCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type);
	virtual void OnBeginWallCollide();
	virtual void OnEndWallCollide();
};

#endif
