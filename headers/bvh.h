#pragma once

#include "common.h"
#include "scene.h"
#include "boundingBox.h"

struct BVH_Node {
	BoundingBox node_bounding_box;
	BVH_Node* left_node = NULL;
	BVH_Node* right_node = NULL;

	int is_leaf_node;

	std::vector<Surface*> surfaces_inside; // I believe it would be fine, I store the pointers to the surfaces instead of the surface itself to save space. This is only for top-level bvh and not for triangular ones.
	std::vector<Vector3i> faces_inside; // Formed using the indices in the main surface.

	void createBVH(Scene* scene);
	void reallyCreateBVH();

	Interaction rayIntersect(Ray ray);

	void printBVH_Information(int level);
	
	int index; // experimental, for leaf nodes only

	void createBVH_ForEachSurface(Surface* surface);
	void reallyCreateBVH_ForEachSurface(Surface* surface);
	Interaction rayIntersect_ForEachSurface(Ray ray, Surface* surface);

	// void updateSurfacesForBVH_Node(Scene* scene_ptr);	// Adding this function to ensure that the surfaces are being correctly filled.
};