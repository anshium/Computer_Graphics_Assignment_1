#include "bvh.h"

// Here are a few thoughts that would help me in navigating the process.

// I would have a BVH_Node structure.
// This BVH_Node would have a left BVH_Node* and a right BVH_Node*.

// We would also store a vector of surfaces with the BVH_Node for the purpose of making the BVH_Node and later simply applying the surface interaction.

// Then when we would reach the end of the recursion at the base case, we would make the is_leaf_node value to be one and the rest of them would have value zero.

void BVH_Node::createBVH(Scene* scene){
	std::cout << "Surface First Information -------------------------------------" << std::endl;
	
	for(int i = 0; i < scene->surfaces.size(); i++){
		std::cout << "Min " << 
					scene->surfaces[i].bounding_box.min.x << " " << 
					scene->surfaces[i].bounding_box.min.y << " " << 
					scene->surfaces[i].bounding_box.min.z << std::endl;
		std::cout << "Max " << 
					scene->surfaces[i].bounding_box.max.x << " " << 
					scene->surfaces[i].bounding_box.max.y << " " << 
					scene->surfaces[i].bounding_box.max.z << std::endl;
		this->surfaces_inside.push_back(&scene->surfaces[i]);		// Dunno if &surface would work. It didn't and wouldn't (dangling pointer)
	}

	std::cout << "Surface First Information Ends -------------------------------------" << std::endl;
	this->reallyCreateBVH();
	std::cout << "Added surfaces to main BVH" << std::endl;
	return;
}

void BVH_Node::reallyCreateBVH(){
	if(this->surfaces_inside.size() == 0 || this->surfaces_inside.size() == 1){
		this->is_leaf_node = 1;
		return;
	}

	if(this->surfaces_inside.size() >= 2){
		this->node_bounding_box.min = surfaces_inside[0]->bounding_box.min;
		this->node_bounding_box.max = surfaces_inside[0]->bounding_box.max;
		this->is_leaf_node = 0;
	}

	for(Surface* surface_ptr : this->surfaces_inside){
		this->node_bounding_box.update(surface_ptr->bounding_box.min);
		this->node_bounding_box.update(surface_ptr->bounding_box.max);
	}

	// I believe that it would a good idea to have the splitting position based on the extent of the centers and not the bounding boxes minimum and maximum as I was having problems with that previously in one case (Donuts).

	Vector3d range = this->node_bounding_box.max - this->node_bounding_box.min;

	std::cout << "+-+-+-Node Bounding Box Information: -+-+-+" << std::endl;
	std::cout << this->node_bounding_box.min.x << " " << this->node_bounding_box.min.y << " " << this->node_bounding_box.min.z << std::endl;
	std::cout << this->node_bounding_box.max.x << " " << this->node_bounding_box.max.y << " " << this->node_bounding_box.max.z << std::endl;
	std::cout << "+-+-+-Node Bounding Box Information Ends: -+-+-+	" << std::endl;

	
	int axis = 0;							// by default the x-axis
	if(range.y > range.x) axis = 1;			// if y axis is larger than the x axis
	if(range.z > range[axis]) axis = 2;		// if z is the longest axis

	double min_value = 1e30; 	// Some very large value - Might cause bugs here if scene is very large which is usually unlikely
	double max_value = -1e30;

	for(Surface* surface_ptr : this->surfaces_inside){
		float center_of_bb = (surface_ptr->bounding_box.min[axis] + surface_ptr->bounding_box.max[axis]) / 2;
		if(center_of_bb < min_value){
			min_value = center_of_bb;
		}
		if(center_of_bb > max_value){
			max_value = center_of_bb;
		}
	}

	// double splitting_position = this->node_bounding_box.min[axis] + 0.5 * range[axis];
	double splitting_position = (min_value + max_value) / 2;

	// Now add the surfaces accordingly to the left node or the right node.
	this->left_node = new BVH_Node();
	this->right_node = new BVH_Node();

	int added_to_left = 0;
	int added_to_right = 0;

	for(Surface* surface_ptr : this->surfaces_inside){
		std::cout << "Min " << surface_ptr->bounding_box.min.x << " " << surface_ptr->bounding_box.min.y << " " << surface_ptr->bounding_box.min.z << std::endl;
		std::cout << "Max " << surface_ptr->bounding_box.max.x << " " << surface_ptr->bounding_box.max.y << " " << surface_ptr->bounding_box.max.z << std::endl;
	}
	
	std::cout << "Chosen Axis: " << axis << std::endl;
	std::cout << "Spliiting Position: " << splitting_position << std::endl;

	for(Surface* surface_ptr : this->surfaces_inside){
		std::cout << "Surface Information -------------------------------------" << std::endl;
		std::cout << surface_ptr->bounding_box.min[axis] << " " << surface_ptr->bounding_box.max[axis] << std::endl;
		std::cout << splitting_position << std::endl;
		std::cout << "Surface Information Ends-------------------------------------" << std::endl;

		double bounding_box_center = (surface_ptr->bounding_box.max[axis] + surface_ptr->bounding_box.min[axis]) / 2;
		std::cout << "Bounding Box Center: " << bounding_box_center<< std::endl;

		if(bounding_box_center < splitting_position){
			this->left_node->surfaces_inside.push_back(surface_ptr);
			this->is_leaf_node = 0;

			std::cout << "Put to Left" << std::endl;

			added_to_left += 1;
		}
		// This tie-breaker funnily works! :)
		else if(bounding_box_center == splitting_position && added_to_left % 2 == 0){
			this->left_node->surfaces_inside.push_back(surface_ptr);
			this->is_leaf_node = 0;

			std::cout << "Put to Left" << std::endl;

			added_to_left += 1;
		}
		else{
			this->right_node->surfaces_inside.push_back(surface_ptr);
			this->is_leaf_node = 1;

			std::cout << "Put to Right" << std::endl;
			
			added_to_right += 1;
		}
	}

	std::cout << "Added to left: " << added_to_left << std::endl;
	std::cout << "Added to right: " << added_to_right << std::endl;

	std::cout << "Called Recursion on left" << std::endl;
	this->left_node->reallyCreateBVH();
	std::cout << "Called Recursion on right" << std::endl;
	this->right_node->reallyCreateBVH();


	std::cout << "Exiting reallyCreateBVH" << std::endl;
}

Interaction BVH_Node::rayIntersect(Ray ray){
	// Phele dekhna hai ki iske bounding box par intersect hui ki nahi.

	// Phir dehna hai ki left pe hui ki right pe
	// Jispe hui, uske ander jaana hai

	Interaction bvhi;

	// base case hai ki ki agar ek hi bounding box hai aur uspe intersect kare to us se associated surface par rayIntersect lagao.
	if(this->surfaces_inside.size() == 1 && this->node_bounding_box.rayIntersect(ray).didIntersect == 1){
		bvhi = this->surfaces_inside[0]->rayIntersect(ray);
	}

	bvhi = this->node_bounding_box.rayIntersect(ray);

	if(bvhi.didIntersect == 1){
		if(this->left_node->node_bounding_box.rayIntersect(ray).didIntersect == 1){
			bvhi = this->left_node->rayIntersect(ray);
		}
		if(this->right_node->node_bounding_box.rayIntersect(ray).didIntersect == 1){
			bvhi = this->right_node->rayIntersect(ray);
		}
	}

	return bvhi;
}