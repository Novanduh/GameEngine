#ifndef RESCMANAGER_H
#define RESCMANAGER_H

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "gameobj.h"

// A game object that is loaded based on
// proximity to the player
class ProxObj {
public:
	GameObj* go;
	float proxTrigger; // The maximum distance from the player at which the object is rendered.

	ProxObj(GameObj g);
	ProxObj(GameObj g, float trigger);
	~ProxObj(void);
};

// A node in a doubly linked list of ProxObjs
class ProxNode {
public:
	ProxNode* prev;
	ProxObj content;
	ProxNode* next;

	ProxNode(ProxObj* content, ProxNode* previous, ProxNode* next);
	~ProxNode(void);
};

// A collection of ProxObjs that all share a proxTrigger.
// A sphere is calculated that encompasses the position
// vectors of all ProxObjs. The proxTrigger is then added
// to the radius of the sphere.
class ProxBlock {
public:
	float midX;
	float midY;
	float midZ;
	float radius;
	float proxTrigger;
	ProxNode* proxList;
	int count;

	ProxBlock(GameObj* gos, float proximity);
	~ProxBlock(void);

private:
	void Calc(void);
};

class ProxBlockNode {
public:
	ProxBlockNode* prev;
	ProxBlock* content;
	ProxBlockNode* next;

	ProxBlockNode(ProxBlock* content, ProxBlockNode* prev, ProxBlockNode* next);
	~ProxBlockNode(void);
};

class RescManager {
public:
	RescManager* getInstance(void);
	~RescManager(void);
	void Update(float plx, float ply, float plz, GameObj* objs);


private:
	RescManager* instance;
	ProxNode* proxList; // The head of a doubly linked list of ProxNodes
	ProxBlockNode* proxBlockList; // The head of a doubly linked list of ProxBlockNodes
	float prevx;
	float prevy;
	float prevz;
	float maxTrigDist;
	ProxObj* prevObjs;
	ProxBlock* prevBlocks;
	int prevCount;

	RescManager(void);
};

#endif