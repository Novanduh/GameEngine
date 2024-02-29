#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "gameobj.h"

typedef struct ProxObj ProxObj;

// A game object that is loaded based on
// proximity to the player
struct ProxObj {
	GameObject* go;
	float proxTrigger; // The maximum distance from the player at which the object is rendered.
};

// A node in a binary search tree of ProxObjs
private class ProxNode {
public:
	ProxObj* parent;
	ProxObj* content;
	ProxObj* left;
	ProxObj* right;
};

// A collection of 
public class rblock {
public:
	
};

public class RescManager {
public:


private:
	
	ProxNode proxtree; // The head of a binary search tree of ProxNodes
};