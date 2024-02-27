/*#include <ode/ode.h>
#include <stdlib.h>

class Phys {
private:
	dWorldID worldID;
	dBodyID* bodies;
	dBodyID** nullBodies;
	int nullCount;
	int bodyCount;

	dBodyID* getBodyPtr(dBodyID id) {
		for (int i = 0; i < sizeof(bodies); i++) {
			if (*(bodies + i) == id) {
				return bodies + i;
			}
		}
		return NULL;
	}

	void addNullAt(dBodyID* body) {
		*body = NULL;
		realloc(nullBodies, (nullCount + 1) * sizeof(dBodyID*));
		*(nullBodies + nullCount) = body;
		nullCount++;
		bodyCount--;
	}

public:
	Phys() {
		dInitODE();
		worldID = dWorldCreate();
		bodyCount = 0;
		nullCount = 0;
	}

	void UpdatePhysics() {
		dWorldStep(worldID, 1);
	}

	dBodyID AddRigidBody() {
		if (sizeof(nullBodies) == 0) {
			realloc(bodies, (bodyCount + 1) * sizeof(dBodyID));
			dBodyID* end = bodies + bodyCount;
			*end = dBodyCreate(worldID);
			bodyCount++;
			return *end;
		}
		else {
			dBodyID* nPtr = *(nullBodies + nullCount - 1);
			*nPtr = dBodyCreate(worldID);
			realloc(nullBodies, (nullCount - 1) * sizeof(dBodyID*));
			bodyCount++;
			nullCount--;
			return *nPtr;
		}

	}

	void RemoveRigidBody(dBodyID id) {
		addNullAt(getBodyPtr(id));
		dBodyDestroy(id);
	}

	~Phys() {
		free(bodies);
		free(nullBodies);
		dWorldDestroy(worldID);
		dCloseODE();
	}
};*/