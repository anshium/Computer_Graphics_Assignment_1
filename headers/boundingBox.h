#pragma once

#include "common.h"

struct BoundingBox{
	Vector3f min, max;				// Define the minimum and the maximum coordinates for the bounding box.

	// The & operator means that a reference is being taken.
	void update(const Vector3f& point);
};