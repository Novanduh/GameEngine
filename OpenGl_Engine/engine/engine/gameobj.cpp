#include <string>
#include "gameobj.h"

GameObj::GameObj(string texturePath, string objPath, float x, float y, float z)
	:TexturePath(texturePath),
	ObjPath(objPath),
	x(x),
	y(y),
	z(z)
{

}