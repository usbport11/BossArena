#include "Leaf.h"

int RandomValue(int Min, int Max) {
	if(Min > Max) return 0;
	if(Min == Max) return Min;
	return rand() % (Max - Min + 1) + Min;
}

MLeaf::MLeaf(NVector2 inPosition, NVector2 inSize) {
	Position = inPosition;
	Size = inSize;
	LeafLeft = NULL;
	LeafRight = NULL;
	Room = NULL;
	ConnectedRooms[0] = ConnectedRooms[1] = NULL;
}

MLeaf::MLeaf()
{
	Position = NVector2(0, 0);
	Size = NVector2(0, 0);
	LeafLeft = NULL;
	LeafRight = NULL;
	Room = NULL;
	ConnectedRooms[0] = ConnectedRooms[1] = NULL;
}

bool MLeaf::Split() {
	if(LeafLeft || LeafRight) return false;
	bool SplitByHeight = rand() % 2;
	if(Size.x > Size.y && Size.x / Size.y >= 1.25) SplitByHeight = false;
	else if(Size.y > Size.x && Size.y / Size.x >= 1.25) SplitByHeight = true;
	int MaxLeafSize = (SplitByHeight ? Size.y : Size.x) - MIN_LEAF_SIZE_2;
	if(MaxLeafSize <= MIN_LEAF_SIZE_2) return false;
	int SplitSize = rand() % (MaxLeafSize - MIN_LEAF_SIZE_2) + MIN_LEAF_SIZE_2;
	if(SplitByHeight) {
		LeafLeft = new MLeaf(NVector2(Position.x, Position.y), NVector2(Size.x, SplitSize));
		LeafRight = new MLeaf(NVector2(Position.x, Position.y + SplitSize), NVector2(Size.x, Size.y - SplitSize));
	}
	else {
		LeafLeft = new MLeaf(NVector2(Position.x, Position.y), NVector2(SplitSize, Size.y));
		LeafRight = new MLeaf(NVector2(Position.x + SplitSize, Position.y), NVector2(Size.x - SplitSize, Size.y));
	}
	return true;
}

void MLeaf::CreateRooms() {
	if(LeafLeft || LeafRight) {
		if(LeafLeft) {
			LeafLeft->CreateRooms();
		}
		if(LeafRight) {
			LeafRight->CreateRooms();
		}
		if(LeafLeft && LeafRight) {
			CreateHall(LeafLeft->GetRoom(), LeafRight->GetRoom());
			//printf("%i - %i\n", LeafLeft->GetRoom(), LeafRight->GetRoom());
		}
	}
	else {
		Room = new NRectangle2;
		Room->Size.x = rand() % (Size.x - (MIN_ROOM_SIZE_2 + 2)) + MIN_ROOM_SIZE_2;
		Room->Size.y = rand() % (Size.y - (MIN_ROOM_SIZE_2 + 2)) + MIN_ROOM_SIZE_2;
		Room->Position.x = Position.x + rand() % (Size.x - Room->Size.x - 2) + 1;
		Room->Position.y = Position.y + rand() % (Size.y - Room->Size.y - 2) + 1;
	}
}

NRectangle2* MLeaf::GetRoom() {
	if(Room) return Room;
	NRectangle2* RoomLeft = NULL;
	NRectangle2* RoomRight = NULL;
	if(LeafLeft) RoomLeft = LeafLeft->GetRoom();
	if(LeafRight) RoomRight = LeafRight->GetRoom();
	if(RoomLeft && RoomRight) {
		if(rand() % 2) {
			return RoomLeft;
		}
		else {
			return RoomRight;
		}
	}
	else {
		if(RoomLeft) return RoomLeft;
		else return RoomRight;
	}
	return NULL;
}

void MLeaf::CreateHall(NRectangle2* inRoomLeft, NRectangle2* inRoomRight) {
	if(!inRoomLeft || !inRoomRight) return;
	if(inRoomLeft->Size.x <= 0 || inRoomLeft->Size.y <= 0) return;
	
	ConnectedRooms[0] = inRoomLeft;
	ConnectedRooms[1] = inRoomRight;
	
	NVector2 PointLeft;
	NVector2 PointRight;
	NVector2 PointDiff;
	PointLeft.x = RandomValue(inRoomLeft->Position.x + 1, inRoomLeft->Position.x + inRoomLeft->Size.x - 2);
	PointLeft.y = RandomValue(inRoomLeft->Position.y + 1, inRoomLeft->Position.y + inRoomLeft->Size.y - 2);
	PointRight.x = RandomValue(inRoomRight->Position.x + 1, inRoomRight->Position.x + inRoomRight->Size.x - 2);
	PointRight.y = RandomValue(inRoomRight->Position.y + 1, inRoomRight->Position.y + inRoomRight->Size.y - 2);
	PointDiff = NVector2(PointRight.x - PointLeft.x, PointRight.y - PointLeft.y);
	if(PointDiff.x < 0) {
		if(PointDiff.y < 0) {
			if(rand() % 2) {
				Halls.push_back(NRectangle2(NVector2(PointRight.x, PointLeft.y), NVector2(abs(PointDiff.x), 1)));
				Halls.push_back(NRectangle2(NVector2(PointRight.x, PointRight.y), NVector2(1, abs(PointDiff.y))));
			}
			else {
				Halls.push_back(NRectangle2(NVector2(PointRight.x, PointRight.y), NVector2(abs(PointDiff.x), 1)));
				Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointRight.y), NVector2(1, abs(PointDiff.y))));
			}
		}
		else if(PointDiff.y > 0) {
			if(rand() % 2) {
				Halls.push_back(NRectangle2(NVector2(PointRight.x, PointLeft.y), NVector2(abs(PointDiff.x), 1)));
				Halls.push_back(NRectangle2(NVector2(PointRight.x, PointLeft.y), NVector2(1, abs(PointDiff.y))));
			}
			else {
				Halls.push_back(NRectangle2(NVector2(PointRight.x, PointRight.y), NVector2(abs(PointDiff.x) + 1, 1)));
				Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointLeft.y), NVector2(1, abs(PointDiff.y))));
			}
		}
		else Halls.push_back(NRectangle2(NVector2(PointRight.x, PointRight.y), NVector2(abs(PointDiff.x), 1)));
	}
	else if(PointDiff.x > 0) {
		if(PointDiff.y < 0) {
			if(rand() % 2) {
				Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointRight.y), NVector2(abs(PointDiff.x), 1)));
				Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointRight.y), NVector2(1, abs(PointDiff.y))));
			}
			else {
				Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointLeft.y), NVector2(abs(PointDiff.x) + 1, 1)));
				Halls.push_back(NRectangle2(NVector2(PointRight.x, PointRight.y), NVector2(1, abs(PointDiff.y))));
			}
		}
		else if(PointDiff.y > 0) {
			if(rand() % 2) {
				Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointLeft.y), NVector2(abs(PointDiff.x), 1)));
				Halls.push_back(NRectangle2(NVector2(PointRight.x, PointLeft.y), NVector2(1, abs(PointDiff.y))));
			}
			else {
				Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointRight.y), NVector2(abs(PointDiff.x), 1)));
				Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointLeft.y), NVector2(1, abs(PointDiff.y))));
			}
		}
		else Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointLeft.y), NVector2(abs(PointDiff.x), 1)));
	}
	else {
		if(PointDiff.y < 0) {
			Halls.push_back(NRectangle2(NVector2(PointRight.x, PointRight.y), NVector2(1, abs(PointDiff.y))));
		}
		else if(PointDiff.y > 0) {
			Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointLeft.y), NVector2(1, abs(PointDiff.y))));
		}
	}
}


