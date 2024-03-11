//create header file
#include "RescManager.h"
#include "gameobj.h"
#include "gameobj.cpp"
#include <string>
#include <stdlib.h>
#include <cmath>

using namespace std;

float getDist(float x1, float x2, float y1, float y2, float z1, float z2) {
	float xdif, ydif, zdif;
	xdif = x1 - x2;
	ydif = y1 - y2;
	zdif = z1 - z2;
	return sqrt(pow(xdif, 2) + pow(ydif, 2) + pow(zdif, 2));
}

ProxObj::ProxObj(GameObj g)
	:go(new GameObj(g.TexturePath, g.ObjPath, g.x, g.y, g.z)),
	proxTrigger(0)
{

}

ProxObj::ProxObj(GameObj g, float trigger)
	:go(new GameObj(g.TexturePath, g.ObjPath, g.x, g.y, g.z)),
	proxTrigger(trigger)
{

}

ProxObj::~ProxObj(void) {
	delete go;
}

ProxNode::ProxNode(ProxObj* content, ProxNode* previous, ProxNode* next)
	:prev(previous),
	content(*content),
	next(next)
{

}

ProxNode::~ProxNode(void) {
	ProxNode* curr = this;
	ProxNode* next = curr->next;
	while (curr) {
		delete curr;
		curr = next;
		next = next->next;
	}
}

ProxBlock::ProxBlock(GameObj* gos, float proximity)
	:proxList(new ProxNode(new ProxObj(gos[0]), nullptr, nullptr))
{
	ProxNode* curr = proxList;
	count = sizeof(gos);

	for (int i = 1; i < sizeof(gos); i++) {
		ProxNode* n = new ProxNode(new ProxObj(gos[i]), curr, nullptr);
		curr->next = n;
		curr = n;
	}

	curr->next = proxList;
	proxList->prev = curr;
	proxTrigger = proximity;
}

ProxBlock::~ProxBlock(void) {
	delete proxList;
}

void ProxBlock::Calc(void) {
	float maxX, minX, maxY, minY, maxZ, minZ;
	ProxNode* curr = proxList;
	float currX = curr->content.go->x;
	float currY = curr->content.go->y;
	float currZ = curr->content.go->z;

	maxX = currX;
	minX = currX;
	maxY = currY;
	minY = currY;
	maxZ = currZ;
	minZ = currZ;

	curr = curr->next;

	while (curr != proxList) {
		maxX = max(currX, maxX);
		minX = min(currX, minX);
		maxY = max(currY, maxY);
		minY = min(currY, minY);
		maxZ = max(currZ, maxZ);
		minZ = min(currZ, minZ);

		curr = curr->next;
	}

	midX = maxX - (maxX - minX) / 2;
	midY = maxY - (maxY - minY) / 2;
	midZ = maxZ - (maxZ - minZ) / 2;

	radius = sqrt(pow(maxX - midX, 2) + pow(maxY - midY, 2) + pow(maxZ - midZ, 2));
}

ProxBlockNode::ProxBlockNode(ProxBlock* content, ProxBlockNode* prev, ProxBlockNode* next)
	:prev(prev),
	content(content),
	next(next)
{
	
}

RescManager::RescManager(void)
	:proxList(NULL),
	proxBlockList(NULL),
	prevObjs(NULL),
	prevx(0),
	prevy(0),
	prevz(0),
	maxTrigDist(0),
	prevCount(0)
{
	
}

RescManager::~RescManager(void) {
	delete instance;
	delete proxList;
	free(prevObjs);
	free(prevBlocks);
}

RescManager* RescManager::getInstance(void) {
	if (instance == NULL) {
		instance = new RescManager();
	}
	return instance;
}

void RescManager::Update(float plx, float ply, float plz, GameObj* objs) {
	GameObj* objp;
	ProxNode* blockPtr, *t = proxList;
	ProxBlockNode* tb = proxBlockList;
	int proxObjCount, proxBlockCount, gocount = 0;
	float pldelta = sqrt(pow(plx - prevx, 2) + pow(ply - prevy, 2) + pow(plz - prevz, 2));


	if (prevObjs != NULL && prevBlocks != NULL && pldelta < maxTrigDist) {
		objs = (GameObj*)malloc(sizeof(GameObj) * prevCount);
		ProxObj* o = prevObjs;
		ProxBlock* b = prevBlocks;
		float newmax;
		int i = 0;

		do {
			float dist = getDist(plx, o->go->x, ply, o->go->y, plz, o->go->z);
			
			if (i == 0 || o->proxTrigger - dist < newmax) {
				newmax = o->proxTrigger - dist;
			}

			objs[i++] = *o->go;
			o += 1;
		} while (o != NULL);

		do {
			float dist = getDist(plx, b->midX, ply, b->midY, plz, b->midZ);

			if (b->proxTrigger + b->radius - dist < newmax) {
				newmax = b->proxTrigger + b->radius - dist;
			}

			ProxNode* bt = b->proxList;

			do {
				objs[i++] = *bt->content.go;
				bt = bt->next;
			} while (bt != b->proxList);
			b += 1;
		} while (b != NULL);
	}


	free(prevObjs);
	free(prevBlocks);
	objs = NULL;

	proxObjCount = 0;
	if (t) {
		do {
			float dist = getDist(plx, t->content.go->x, ply, t->content.go->y, plz, t->content.go->z);
			if (dist <= t->content.proxTrigger) {
				if (gocount == 0 || t->content.proxTrigger - dist < maxTrigDist) {
					maxTrigDist = t->content.proxTrigger - dist;
				}

				realloc(objs, sizeof(GameObj) * ++gocount);
				objs[gocount - 1] = *t->content.go;
				realloc(prevObjs, sizeof(ProxObj) * ++proxObjCount);
				prevObjs[proxObjCount - 1] = t->content;
			}
			t = t->next;
		} while (t != proxList);
	}
	
	proxBlockCount = 0;
	if (tb) {
		do {
			float dist = getDist(plx, tb->content->midX, ply, tb->content->midY, plz, tb->content->midZ);
			if (dist <= tb->content->proxTrigger + tb->content->radius) {
				if (gocount == 0 || tb->content->proxTrigger + tb->content->radius - dist < maxTrigDist) {
					maxTrigDist = tb->content->proxTrigger + tb->content->radius - dist;
				}
				realloc(objs, sizeof(GameObj) * (gocount + tb->content->count));
				blockPtr = tb->content->proxList;
				for (int i = 0; i < tb->content->count; i++) {
					objs[gocount++] = *blockPtr->content.go;
					blockPtr = blockPtr->next;
				}
				realloc(prevBlocks, sizeof(ProxBlock) * ++proxBlockCount);
				prevBlocks[proxBlockCount - 1] = *tb->content;
			}
			tb = tb->next;
		} while (tb != proxBlockList);
	}

	prevx = plx;
	prevy = ply;
	prevz = plz;
	prevCount = gocount;
}