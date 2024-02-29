//create header file
#include "RescManager.h"
#include <string>
#include <stdlib.h>
#include <cmath>

#include <objloader.hpp>

using namespace std;

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

ProxBlock::ProxBlock(GameObject* gos, float proximity)
	:proxList(new ProxNode(new ProxObj(gos[0]), nullptr, nullptr))
{
	ProxNode* curr = proxList;

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

RescManager::RescManager(void) {
	
}

RescManager::~RescManager(void) {
	delete instance;
	delete proxList;
}

RescManager* RescManager::getInstance(void) {
	if (instance == NULL) {
		instance = new RescManager();
	}
	return instance;
}