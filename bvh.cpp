#include "bvh.h"

// Here are a few thoughts that would help me in navigating the process.

// I would have a BVH_Node structure.
// This BVH_Node would have a left BVH_Node* and a right BVH_Node*.

// We would also store a vector of surfaces with the BVH_Node for the purpose of making the BVH_Node and later simply applying the surface interaction.

// Then when we would reach the end of the recursion at the base case, we would make the is_leaf_node value to be one and the rest of them would have value zero.

void BVH_Node::createBVH(Scene* scene){
	// std::cout << "Surface First Information -------------------------------------" << std::endl;
	
	for(int i = 0; i < scene->surfaces.size(); i++){
		scene->surfaces[i].updateBoundingBox();
		// std::cout << "Min " << 
					// scene->surfaces[i].bounding_box.min.x << " " << 
					// scene->surfaces[i].bounding_box.min.y << " " << 
					// scene->surfaces[i].bounding_box.min.z << std::endl;
		// std::cout << "Max " << 
					// scene->surfaces[i].bounding_box.max.x << " " << 
					// scene->surfaces[i].bounding_box.max.y << " " << 
					// scene->surfaces[i].bounding_box.max.z << std::endl;
		this->surfaces_inside.push_back(&scene->surfaces[i]);		// Dunno if &surface would work. It didn't and wouldn't (dangling pointer)	
	}

	// std::cout << "Surface First Information Ends -------------------------------------" << std::endl;
	this->reallyCreateBVH();
	// std::cout << "Added surfaces to main BVH" << std::endl;
	return;
}

void BVH_Node::reallyCreateBVH(){
	if(this->surfaces_inside.size() == 0){ // Was the problem here?
		this->is_leaf_node = 1;
		return;
	}
	if(this->surfaces_inside.size() == 1){ // Was the problem here? Yes there was!
		this->node_bounding_box.min = surfaces_inside[0]->bounding_box.min;
		this->node_bounding_box.max = surfaces_inside[0]->bounding_box.max;
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

	// std::cout << "+-+-+-Node Bounding Box Information: -+-+-+" << std::endl;
	// std::cout << this->node_bounding_box.min.x << " " << this->node_bounding_box.min.y << " " << this->node_bounding_box.min.z << std::endl;
	// std::cout << this->node_bounding_box.max.x << " " << this->node_bounding_box.max.y << " " << this->node_bounding_box.max.z << std::endl;
	// std::cout << "+-+-+-Node Bounding Box Information Ends: -+-+-+	" << std::endl;

	
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
		// std::cout << "Min " << surface_ptr->bounding_box.min.x << " " << surface_ptr->bounding_box.min.y << " " << surface_ptr->bounding_box.min.z << std::endl;
		// std::cout << "Max " << surface_ptr->bounding_box.max.x << " " << surface_ptr->bounding_box.max.y << " " << surface_ptr->bounding_box.max.z << std::endl;
	}
	
	// std::cout << "Chosen Axis: " << axis << std::endl;
	// std::cout << "Spliiting Position: " << splitting_position << std::endl;

	for(Surface* surface_ptr : this->surfaces_inside){
		// std::cout << "Surface Information -------------------------------------" << std::endl;
		std::cout << surface_ptr->bounding_box.min[axis] << " " << surface_ptr->bounding_box.max[axis] << std::endl;
		std::cout << splitting_position << std::endl;
		// std::cout << "Surface Information Ends-------------------------------------" << std::endl;

		double bounding_box_center = (surface_ptr->bounding_box.max[axis] + surface_ptr->bounding_box.min[axis]) / 2;
		// std::cout << "Bounding Box Center: " << bounding_box_center<< std::endl;

		if(bounding_box_center < splitting_position){
			this->left_node->surfaces_inside.push_back(surface_ptr);
			this->is_leaf_node = 0;

			// std::cout << "Put to Left" << std::endl;

			added_to_left += 1;
		}
		// This tie-breaker funnily works! :)
		else if(bounding_box_center == splitting_position && added_to_left % 2 == 0){
			this->left_node->surfaces_inside.push_back(surface_ptr);
			this->is_leaf_node = 0;

			// std::cout << "Put to Left" << std::endl;

			added_to_left += 1;
		}
		else{
			this->right_node->surfaces_inside.push_back(surface_ptr);
			this->is_leaf_node = 0;

			// std::cout << "Put to Right" << std::endl;
			
			added_to_right += 1;
		}
	}

	// std::cout << "Added to left: " << added_to_left << std::endl;
	// std::cout << "Added to right: " << added_to_right << std::endl;

	// std::cout << "Called Recursion on left" << std::endl;
	this->left_node->reallyCreateBVH();
	// std::cout << "Called Recursion on right" << std::endl;
	this->right_node->reallyCreateBVH();


	// std::cout << "Exiting reallyCreateBVH" << std::endl;
}

void BVH_Node::printBVH_Information(int level){
	if(level == 0){
		std::cout << "Level 0 has " << this->surfaces_inside.size() << " surfaces." << std::endl;
	}
	static int index_assignment = 1;
    if (this->surfaces_inside.size() == 0){
        // std::cout << "This is a leaf node 0" << std::endl;
        return;
    }
	if (this->surfaces_inside.size() == 1){
		this->index = index_assignment++;
        std::cout << "This is a leaf node 1, has index: " << this->index << "." << std::endl;
		std::cout << "|---- Bounding box limits:" << std::endl;
		std::cout << "|---- " << this->node_bounding_box.min.x << " " << this->node_bounding_box.min.y << " " << this->node_bounding_box.min.z << std::endl;
	std::cout << "|---- " << this->node_bounding_box.max.x << " " << this->node_bounding_box.max.y << " " << this->node_bounding_box.max.z << std::endl;
		std::cout << "|----" << "Bounding box limits end:" << std::endl;
        return;
    }
    else{
        std::cout << "Printing information of left to level: " << level << ", has surfaces: " << this->left_node->surfaces_inside.size() << std::endl;
		if(this->left_node) this->left_node->printBVH_Information(level + 1);

        std::cout << "Printing information of right to level: " << level << ", has surfaces: " << this->right_node->surfaces_inside.size() << std::endl;
		if(this->right_node) this->right_node->printBVH_Information(level + 1);
    }
    return;
}

Interaction BVH_Node::rayIntersect(Ray ray){
    BVH_Node* node = this;
    Interaction bvhi_final;
    Interaction bvhi;
    bvhi_final.didIntersect = 0;

    if (this->node_bounding_box.rayIntersect(ray).didIntersect == 0) {
		if(ray.x == 800 && ray.y == 540){
			std::cout << "Case 5" << std::endl;
		}
        // No intersection with the bounding box, return the default value
		// std::cout << "No bounding box intersection." << std::endl;

		// std::cout << "Bounding box limits:" << std::endl;
		// std::cout << this->node_bounding_box.min.x << " " << this->node_bounding_box.min.y << " " << this->node_bounding_box.min.z << std::endl;
	// std::cout << this->node_bounding_box.max.x << " " << this->node_bounding_box.max.y << " " << this->node_bounding_box.max.z << std::endl;
		// std::cout << "Bounding box limits end:" << std::endl;

		// std::cout << "Ray Information" << std::endl;
		// std::cout << "Direction " << ray.d.x << ", " << ray.d.y << ", " << ray.d.z << std::endl;
		// std::cout << "Direction " << ray.o.x << ", " << ray.o.y << ", " << ray.o.z << std::endl;

        return bvhi_final;
    }

    if (this->is_leaf_node == 1) {
		if(ray.x == 800 && ray.y == 540){
			std::cout << "Case 6" << std::endl;
		}
        // If it's a leaf node, directly compute the intersection with the surface
		Interaction si;
		if(option == 2){
        	si = this->surfaces_inside[0]->rayIntersect(ray);
		}

		if(option == 3){
			si = this->rayIntersect_ForEachSurface(ray, this->surfaces_inside[0]);
		}

        // std::cout << "Leaf node intersection. Surface index: " << this->index << ", t: " << si.t << std::endl;
        return si;
    }
    else {
		if(ray.x == 800 && ray.y == 540){
			std::cout << "This is the target ray" << std::endl;
		}
        // Recursive intersection with both left and right nodes
        Interaction left_intersection = this->left_node->rayIntersect(ray);
        Interaction right_intersection = this->right_node->rayIntersect(ray);

        // Check which intersection is closer
        if (left_intersection.didIntersect && right_intersection.didIntersect) {
			if(ray.x == 800 && ray.y == 540){
				std::cout << "Case 1" << std::endl;
			}
            float left_distance = left_intersection.t;
            float right_distance = right_intersection.t;

            // Choose the closer intersection
            if (left_distance < right_distance) {
                bvhi_final = left_intersection;
                // std::cout << "Left ---" << std::endl;
            } else {
                bvhi_final = right_intersection;
                // std::cout << "Right ---" << std::endl;
            }
        } else if (left_intersection.didIntersect) {
			if(ray.x == 800 && ray.y == 540){
				std::cout << "Case 2" << std::endl;
			}
            bvhi_final = left_intersection;
            // std::cout << "Left ---" << std::endl;
        } else if (right_intersection.didIntersect) {
			if(ray.x == 800 && ray.y == 540){
				std::cout << "Case 3" << std::endl;
			}
            bvhi_final = right_intersection;
            // std::cout << "Right ---" << std::endl;
        }
		if(ray.x == 800 && ray.y == 540){
			std::cout << "Case 4" << std::endl;
		}
    }

    // std::cout << "Final intersection result: " << bvhi_final.didIntersect << ", t: " << bvhi_final.t << std::endl;

    return bvhi_final;
}

void BVH_Node::createBVH_ForEachSurface(Surface* surface){
	surface->bvh_node = new BVH_Node();
	surface->bvh_node->node_bounding_box = surface->bounding_box;

	int i = 0;
	for(Vector3i face : surface->indices){
		this->faces_inside[i] = face;
		i++;
	}

	surface->marryTrianglesWithBoundingBoxes();

	this->reallyCreateBVH_ForEachSurface(surface);
}

void BVH_Node::reallyCreateBVH_ForEachSurface(Surface* surface){
	Vector3d range = this->node_bounding_box.max - this->node_bounding_box.min;
	
	int axis = 0;							// by default the x-axis
	if(range.y > range.x) axis = 1;			// if y axis is larger than the x axis
	if(range.z > range[axis]) axis = 2;		// if z is the longest axis

	double min_value = 1e30; 	// Some very large value - Might cause bugs here if scene is very large which is usually unlikely
	double max_value = -1e30;

	for(int i = 0; i < faces_inside.size(); i++){
		// Calculate the centroid of the triangle along the chosen axis			// Target
		// Either that or simply find the center of their wives. Going with the latter for now.
		
		float center_of_bb = (surface->triangles_ki_bbiyaan[i].min[axis] + surface->triangles_ki_bbiyaan[i].max[axis]) / 2;
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

	for(int i = 0; i < this->faces_inside.size(); i++){
		double bounding_box_center = (surface->triangles_ki_bbiyaan[i].min[axis] + surface->triangles_ki_bbiyaan[i].max[axis]) / 2;

		if(bounding_box_center < splitting_position){
			this->left_node->faces_inside.push_back(this->faces_inside[i]);
			this->is_leaf_node = 0;

			added_to_left += 1;
		}
		// This tie-breaker funnily works! :)
		else if(bounding_box_center == splitting_position && added_to_left % 2 == 0){
			this->left_node->faces_inside.push_back(this->faces_inside[i]);
			this->is_leaf_node = 0;

			added_to_left += 1;
		}
		else{
			this->right_node->faces_inside.push_back(this->faces_inside[i]);
			this->is_leaf_node = 0;
			
			added_to_right += 1;
		}
	}

	return;
}

Interaction BVH_Node::rayIntersect_ForEachSurface(Ray ray, Surface* surface){
	BVH_Node* node = this;
    Interaction bvhi_final;
    Interaction bvhi;
    bvhi_final.didIntersect = 0;
	float tmin = 1e30;

    if (this->node_bounding_box.rayIntersect(ray).didIntersect == 0) {
        return bvhi_final;
    }

    if (this->is_leaf_node == 1) {
        // If it's a leaf node, directly compute the intersection with the surface
       	Vector3f p1 = surface->vertices[this->faces_inside[0].x];
		Vector3f p2 = surface->vertices[this->faces_inside[0].y];
		Vector3f p3 = surface->vertices[this->faces_inside[0].z];

		Vector3f n1 = surface->normals[this->faces_inside[0].x];
		Vector3f n2 = surface->normals[this->faces_inside[0].y];
		Vector3f n3 = surface->normals[this->faces_inside[0].z];
		Vector3f n = Normalize(n1 + n2 + n3);

		Interaction si = surface->rayTriangleIntersect(ray, p1, p2, p3, n);
		if (si.t <= tmin && si.didIntersect) {
			bvhi_final = si;
			tmin = si.t;            // But what are we getting if we have this tmin? It is anyway not being used aftwewards
		}

		return bvhi_final;
    }
    else {
        // Recursive intersection with both left and right nodes
        Interaction left_intersection = this->left_node->rayIntersect_ForEachSurface(ray, surface);
        Interaction right_intersection = this->right_node->rayIntersect_ForEachSurface(ray, surface);

        // Check which intersection is closer
        if (left_intersection.didIntersect && right_intersection.didIntersect) {
            float left_distance = left_intersection.t;
            float right_distance = right_intersection.t;

            // Choose the closer intersection
            if (left_distance < right_distance) {
                bvhi_final = left_intersection;
            } else {
                bvhi_final = right_intersection;
            }
        } else if (left_intersection.didIntersect) {
            bvhi_final = left_intersection;
        } else if (right_intersection.didIntersect) {
            bvhi_final = right_intersection;
        }
    }

    return bvhi_final;
}

// Interaction BVH_Node::rayIntersect(Ray ray){
// 	// Phele dekhna hai ki iske bounding box par intersect hui ki nahi.

// 	// Phir dehna hai ki left pe hui ki right pe
// 	// Jispe hui, uske ander jaana hai

// 	Interaction bvhi_final;
//     float tmin = ray.t;

// 	bvhi_final.didIntersect = 0;

// 	Interaction bvhi_root = this->node_bounding_box.rayIntersect(ray);

// 	// ============================BASE CASE SECTION BEGINS============================
// 	// Agar main bvhi_root pe hi intersect nahi kiya to to phir rehne hi do
// 	if(bvhi_root.didIntersect == 0) return bvhi_root;

// 	// But if it did intersect, then check if it the surfaces inside are one or more. (or zero)
// 	if(bvhi_root.didIntersect && this->surfaces_inside.size() == 0) return bvhi_root;
// 	if(bvhi_root.didIntersect && this->surfaces_inside.size() == 1){
// 		Interaction bvhi = this->surfaces_inside[0]->rayIntersect(ray);
// 		if (bvhi.t <= tmin && bvhi.didIntersect) {
// 			bvhi_final = bvhi;
// 			tmin = bvhi.t;
// 		}

// 		return bvhi_final;
// 	}

// 	// ============================BASE CASE SECTION ENDS  ============================

// 	if(bvhi_root.didIntersect == 1){
// 		if(this->left_node->node_bounding_box.rayIntersect(ray).didIntersect == 1){
// 			Interaction bvhi = this->left_node->rayIntersect(ray);
// 			if (bvhi.t <= tmin && bvhi.didIntersect) {
// 				bvhi_final = bvhi;
// 				tmin = bvhi.t;
// 			}
// 		}
// 		if(this->right_node->node_bounding_box.rayIntersect(ray).didIntersect == 1){
// 			Interaction bvhi = this->right_node->rayIntersect(ray);
// 			if (bvhi.t <= tmin && bvhi.didIntersect) {
// 				bvhi_final = bvhi;
// 				tmin = bvhi.t;
// 			}
// 		}
// 	}
// 	else{
// 		bvhi_final.didIntersect = 0;
// 	}

// 	return bvhi_final;

// 	// ---------------------------------------------------------------------------------------------------------------------------------
// 	/*
// 	// base case hai ki ki agar ek hi bounding box hai aur uspe intersect kare to us se associated surface par rayIntersect lagao.
// 	if(this->surfaces_inside.size() == 1 && this->node_bounding_box.rayIntersect(ray).didIntersect == 1){
// 		Interaction bvhi = this->surfaces_inside[0]->rayIntersect(ray);
// 		if (bvhi.t <= tmin && bvhi.didIntersect) {
// 			bvhi_final = bvhi;
// 			tmin = bvhi.t;
// 		}
// 		return bvhi_final;
// 	}

// 	// Should a tmin check be applied here?
// 	Interaction bvhi = this->node_bounding_box.rayIntersect(ray);

// 	if(bvhi.didIntersect == 1){
// 		if(this->left_node->node_bounding_box.rayIntersect(ray).didIntersect == 1){
// 			Interaction bvhi = this->left_node->rayIntersect(ray);
// 			if (bvhi.t <= tmin && bvhi.didIntersect) {
// 				bvhi_final = bvhi;
// 				tmin = bvhi.t;
// 			}
// 		}
// 		if(this->right_node->node_bounding_box.rayIntersect(ray).didIntersect == 1){
// 			Interaction bvhi = this->right_node->rayIntersect(ray);
// 			if (bvhi.t <= tmin && bvhi.didIntersect) {
// 				bvhi_final = bvhi;
// 				tmin = bvhi.t;
// 			}
// 		}
// 	}

// 	return bvhi_final;
// 	*/
// }