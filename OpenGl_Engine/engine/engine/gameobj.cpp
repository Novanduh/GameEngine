#include <string>
#include "gameobj.h"

GameObj::GameObj()
	:TexturePath(""),
	ObjPath(""),
	x(0),
	y(0),
	z(0)
{

}

GameObj::GameObj(string texturePath, string objPath, float x, float y, float z)
	:TexturePath(texturePath),
	ObjPath(objPath),
	x(x),
	y(y),
	z(z)
{

}