#pragma once

#include "common.h"
#include "scene.h"
#include "boundingBox.h"

struct BVH_Node {
	BoundingBox node_bounding_box;
	BVH_Node* left_node = NULL;
	BVH_Node* right_node = NULL;

	int is_leaf_node;

	void create_BVH_using_self_as_root(Scene scene){

	}
};