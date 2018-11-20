#ifndef OBJECT_H
#define OBJECT_H

#include "AABB.h"

#define SCENE_SPACE 1
#define OBJECT      0

class Object {

public:
	virtual bool isSceneSpace() const = 0;
};

#endif
