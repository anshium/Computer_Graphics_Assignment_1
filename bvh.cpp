#include "bvh.h"

// Here are a few thoughts that would help me in navigating the process.

// I would have a BVH_Node structure.
// This BVH_Node would have a left BVH_Node* and a right BVH_Node*.

// We would also store a vector of surfaces with the BVH_Node for the purpose of making the BVH_Node and later simply applying the surface interaction.

// Then when we would reach the end of the recursion at the base case, we would make the is_leaf_node value to be one and the rest of them would have value zero.

void BVH_Node::createBVH(Scene scene){
	for(Surface surface : scene.surfaces){
		this->surfaces_inside.push_back(&surface);		// Dunno if &surface would work
	}

	this->reallyCreateBVH();

	return;
}

void BVH_Node::reallyCreateBVH(){
	if(this->surfaces_inside.size() == 0 || this->surfaces_inside.size() == 1){
		this->is_leaf_node = 1;
		return;
	}

	if(this->surfaces_inside.size() != 0){
		this->node_bounding_box.min = surfaces_inside[0]->bounding_box.min;
		this->node_bounding_box.max = surfaces_inside[0]->bounding_box.max;
		this->is_leaf_node = 0;
	}

	for(Surface* surface_ptr : this->surfaces_inside){
		this->node_bounding_box.update(surface_ptr->bounding_box.min);
		this->node_bounding_box.update(surface_ptr->bounding_box.max);
	}

	Vector3d range = this->node_bounding_box.max - this->node_bounding_box.min;
	int axis = 0;							// by default the x-axis
	if(range.y > range.x) axis = 1;			// if y axis is larger than the x axis
	if(range.z > range[axis]) axis = 2;		// if z is the longest axis

	double splitting_position = this->node_bounding_box.min[axis] + 0.5 * range[axis];

	// Now add the surfaces accordingly to the left node or the right node.
	this->left_node = new BVH_Node();
	this->right_node = new BVH_Node();

	for(Surface* surface_ptr : this->surfaces_inside){
		if(surface_ptr->bounding_box.min[axis] <= splitting_position){
			this->left_node->surfaces_inside.push_back(surface_ptr);
			this->is_leaf_node = 0;
		}
		if(surface_ptr->bounding_box.max[axis] >= splitting_position){
			this->right_node->surfaces_inside.push_back(surface_ptr);
			this->is_leaf_node = 1;
		}
	}

	this->left_node->reallyCreateBVH();
	this->right_node->reallyCreateBVH();
}