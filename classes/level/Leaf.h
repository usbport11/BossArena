#ifndef leafH
#define leafH

#include <math.h> //floor()
#include <stdio.h> //printf()
#include <stdlib.h> //rand()
#include <vector>

#define MIN_LEAF_SIZE_2 6
#define MAX_LEAF_SIZE_2 20
#define MIN_ROOM_SIZE_2 3

struct NVector2 {
	int x;
	int y;
	NVector2() {
		x = y = 0;
	}
	NVector2(int inX, int inY) {
		x = inX;
		y = inY;
	}
};

struct NLine2 {
	NVector2 a;
	NVector2 b;
	NLine2() {
		a = NVector2(0, 0);
		b = NVector2(0, 0);
	}
	NLine2(NVector2 inA, NVector2 inB) {
		a = inA;
		b = inB;
	}
};

struct NRectangle2 {
	NVector2 Position;
	NVector2 Size;
	NRectangle2() {
		Position = NVector2(0, 0);
		Size = NVector2(0, 0);
	}
	NRectangle2(NVector2 inPosition, NVector2 inSize) {
		Position = inPosition;
		Size = inSize;
	}
};

class MLeaf {
public:
	NVector2 Position;
	NVector2 Size;
	MLeaf* LeafLeft;
	MLeaf* LeafRight;
	NRectangle2* Room;
	std::vector<NRectangle2> Halls;
	NRectangle2* ConnectedRooms[2];
	MLeaf();
	MLeaf(NVector2 inPostition, NVector2 inSize);
	bool Split();
	void CreateRooms();
	NRectangle2* GetRoom();
	void CreateHall(NRectangle2* inRoomLeft, NRectangle2* inRoomRight);
};

#endif
