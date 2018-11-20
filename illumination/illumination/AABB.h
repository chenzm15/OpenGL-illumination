#ifndef AABB_H
#define AABB_H

class AABB {
private:
	float min_x_;
	float max_x_;
	float min_y_;
	float max_y_;
	float min_z_;
	float max_z_;

public:
	AABB() : min_x_(0), max_x_(0), min_y_(0), max_y_(0)
	{ }
	AABB(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
		: min_x_(minX), max_x_(maxX), min_y_(minY), max_y_(maxY), min_z_(minZ), max_z_(maxZ)
	{ }
	bool isOverlap(const AABB& another) const
	{

	}
	bool isContain(const AABB& another) const
	{

	}
};

#endif
