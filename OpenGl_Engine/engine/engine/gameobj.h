#ifndef GAMEOBJ_H
#define GAMEOBJ_H
#include <string>

using namespace std;

class GameObj {
public:
	string TexturePath;
	string ObjPath;
	float x;
	float y;
	float z;

	GameObj();
	GameObj(string texturePath, string objPath, float x, float y, float z);
};

#endif