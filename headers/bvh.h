#pragma once

#include "common.h"
#include "scene.h"
#include "boundingBox.h"

struct BVH_Node {
	BoundingBox node_bounding_box;
	BVH_Node* left_node = NULL;
	BVH_Node* right_node = NULL;

	int is_leaf_node;

	std::vector<Surface*> surfaces_inside; // I believe it would be fine, I store the pointers to the surfaces instead of the surface itself to save space.

	void createBVH(Scene* scene);
	void reallyCreateBVH();

	Interaction rayIntersect(Ray ray);
};