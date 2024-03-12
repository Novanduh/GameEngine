//create header file
#include "RescManager.hpp"
#include "gameobj.h"
#include "gameobj.cpp"
#include <string>
#include <stdlib.h>
#include <cmath>

using namespace std;

namespace resc {
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
		//delete go;
	}

	ProxBlock::ProxBlock() 
		:midX(),
		midY(),
		midZ(),
		radius(),
		proxTrigger(),
		proxList(),
		count()
	{

	}

	ProxBlock::ProxBlock(GameObj gos[], int size, float proximity)
		:proxList()
	{
		for (int i = 0; i < size; i++) {
			proxList.push_back(gos[i]);
		}
		count = size;
		proxTrigger = proximity;
		Calc();
	}

	ProxBlock::~ProxBlock(void) {
		
	}

	void ProxBlock::Calc(void) {
		float maxX, minX, maxY, minY, maxZ, minZ;

		maxX = minX = proxList[0].x;
		maxY = minY = proxList[0].y;
		maxZ = minZ = proxList[0].z;

		for (int i = 1; i < proxList.size(); i++) {
			maxX = max(proxList[i].x, maxX);
			minX = min(proxList[i].x, minX);
			maxY = max(proxList[i].y, maxY);
			minY = min(proxList[i].y, minY);
			maxZ = max(proxList[i].z, maxZ);
			minZ = min(proxList[i].z, minZ);
		}

		midX = maxX - (maxX - minX) / 2;
		midY = maxY - (maxY - minY) / 2;
		midZ = maxZ - (maxZ - minZ) / 2;

		radius = getDist(maxX, midX, maxY, midY, maxZ, midZ);
	}

	RescManager::RescManager(void)
		:proxList(),
		proxBlockList(),
		prevBlocks(),
		prevObjs(),
		pers(),
		prevx(0),
		prevy(0),
		prevz(0),
		maxTrigDist(0),
		prevCount(0),
		persCount(0),
		totgos(0)
	{

	}

	RescManager::~RescManager(void) {
		delete[] pers;
	}

	vector<GameObj> RescManager::Update(float plx, float ply, float plz) {
		vector<GameObj> tempObjs;
		float pldelta = getDist(plx, prevx, ply, prevy, plz, prevz);

		for (int i = 0; i < persCount; i++) {
			tempObjs.push_back(pers[i]);
		}

		if ((!prevObjs.empty() || !prevBlocks.empty()) && pldelta < maxTrigDist) {
			float newmax;

			for (int i = 0; i < prevObjs.size(); i++) {
			float dist = getDist(plx, prevObjs[i].go->x, ply, prevObjs[i].go->y, plz, prevObjs[i].go->z);
				/*if (tempObjs.size() == persCount || prevObjs[i].proxTrigger - dist < newmax) {
					newmax = prevObjs[i].proxTrigger - dist;
				}*/
				tempObjs.push_back(*prevObjs[i].go);
			}

			for (int i = 0; i < prevBlocks.size(); i++) {
				float dist = getDist(plx, prevBlocks[i].midX, ply, prevBlocks[i].midY, plz, prevBlocks[i].midZ);
				/*if (tempObjs.size() == persCount || prevBlocks[i].proxTrigger + prevBlocks[i].radius - dist < newmax) {
					newmax = prevBlocks[i].proxTrigger + prevBlocks[i].radius - dist;
					
				}*/
				for (int j = 0; j < prevBlocks[i].proxList.size(); j++) {
					tempObjs.push_back(prevBlocks[i].proxList[j]);
				}
			}

			maxTrigDist -= pldelta;
		} else {
			prevObjs.clear();
			prevBlocks.clear();
			
			for (int i = 0; i < proxList.size(); i++) {
				float dist = getDist(plx, proxList[i].go->x, ply, proxList[i].go->y, plz, proxList[i].go->z);
				if (dist <= proxList[i].proxTrigger) {
					if (tempObjs.size() == persCount || proxList[i].proxTrigger - dist < maxTrigDist) {
						maxTrigDist = proxList[i].proxTrigger - dist;
					}

					tempObjs.push_back(*proxList[i].go);
					prevObjs.push_back(proxList[i]);
				}
				else {
					if (dist - proxList[i].proxTrigger < maxTrigDist) {
						maxTrigDist = dist - proxList[i].proxTrigger;
					}
				}
			}

			for (int j = 0; j < proxBlockList.size(); j++) {
				float dist = getDist(plx, proxBlockList[j].midX, ply, proxBlockList[j].midY, plz, proxBlockList[j].midZ);
				if (dist <= proxBlockList[j].proxTrigger + proxBlockList[j].radius) {
					if (tempObjs.size() == persCount || proxBlockList[j].proxTrigger + proxBlockList[j].radius - dist < maxTrigDist) {
						maxTrigDist = proxBlockList[j].proxTrigger + proxBlockList[j].radius - dist;
					}
					for (int k = 0; k < proxBlockList[j].count; k++) {
						tempObjs.push_back(proxBlockList[j].proxList[k]);
					}
					prevBlocks.push_back(proxBlockList[j]);
				}
				else {
					if (dist - proxBlockList[j].proxTrigger - proxBlockList[j].radius < maxTrigDist) {
						maxTrigDist = dist - proxBlockList[j].proxTrigger - proxBlockList[j].radius;
					}
				}
			}
		}

		prevx = plx;
		prevy = ply;
		prevz = plz;
		prevCount = tempObjs.size();
		return tempObjs;
	}

	void RescManager::AddObj(ProxObj objs[], int size) {
		for (int i = 0; i < size; i++) {
			proxList.push_back(objs[i]);
		}
		totgos += size;
	}

	void RescManager::AddBlock(ProxBlock blocks[], int size) {
		for (int i = 0; i < size; i++) {
			proxBlockList.push_back(blocks[i]);
		}
		totgos += size;
	}

	void RescManager::AddPersistent(GameObj obj[], int size) {
		GameObj* gos = new GameObj[persCount + size];
		for (int i = 0; i < persCount; i++) {
			gos[i] = pers[i];
		}
		for (int i = 0; i < size; i++) {
			gos[persCount + i] = obj[i];
		}
		delete[] pers;
		pers = gos;
		persCount += size;
		totgos += size;
	}
}