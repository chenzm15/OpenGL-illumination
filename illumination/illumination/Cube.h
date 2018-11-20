#ifndef CUBE_H
#define CUBE_H

#include "Object.h"
#include <glm/glm.hpp>

class Cube : public Object {
private:
	bool is_scene_space_;
	AABB aabb;

	float width_;
	float height_;
	float depth_;
	glm::vec3 center_;

public:
	Cube(const glm::vec3 &center, float width, float height, float depth, bool is_scene_space = false)
		: center_(center), width_(width), height_(height), depth_(depth), is_scene_space_(is_scene_space)
	{
		float half_width = width / 2.0f, half_height = height / 2.0f, half_depth = depth / 2.0f;
		aabb = AABB(center.x - half_width, center.x + half_width,
			center.y - half_height, center.y + half_height,
			center.z - half_depth, center.z + half_depth);
	}

	virtual bool isSceneSpace() const override
	{
		return is_scene_space_;
	}

	bool isCollideWith(const Cube& another) const
	{
		if (!another.isSceneSpace())
			return this->aabb.isOverlap(another.aabb);
		else
			return !another.aabb.isContain(this->aabb);
	}
};

#endif
