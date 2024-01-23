#include "bvh.h"

// Here are a few thoughts that would help me in navigating the process.

// I would have a BVH_Node structure.
// This BVH_Node would have a left BVH_Node* and a right BVH_Node*.

// We would also store a vector of surfaces with the BVH_Node for the purpose of making the BVH_Node and later simply applying the surface interaction.

// Then when we would reach the end of the recursion at the base case, we would make the is_leaf_node value to be one and the rest of them would have value zero.

void BVH_Node::createBVH(Scene* scene){
	// // Hehe // Hehe std::cout << "Surface First Information -------------------------------------" << std::endl;
	// Hehe // Hehe std::cout << "Here 1" << std::endl;
	for(int i = 0; i < scene->surfaces.size(); i++){
		scene->surfaces[i].updateBoundingBox();
		// // Hehe // Hehe std::cout << "Min " << 
					// scene->surfaces[i].bounding_box.min.x << " " << 
					// scene->surfaces[i].bounding_box.min.y << " " << 
					// scene->surfaces[i].bounding_box.min.z << std::endl;
		// // Hehe // Hehe std::cout << "Max " << 
					// scene->surfaces[i].bounding_box.max.x << " " << 
					// scene->surfaces[i].bounding_box.max.y << " " << 
					// scene->surfaces[i].bounding_box.max.z << std::endl;
		this->surfaces_inside.push_back(&scene->surfaces[i]);		// Dunno if &surface would work. It didn't and wouldn't (dangling pointer)	
	}

	// Hehe // Hehe std::cout << "Here 2" << std::endl;
	// // Hehe // Hehe std::cout << "Surface First Information Ends -------------------------------------" << std::endl;
	this->reallyCreateBVH();
	
	// // Hehe // Hehe std::cout << "Added surfaces to main BVH" << std::endl;
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

	
	// // Hehe // Hehe std::cout << "+-+-+-Node Bounding Box Information: -+-+-+" << std::endl;
	// // Hehe // Hehe std::cout << this->node_bounding_box.min.x << " " << this->node_bounding_box.min.y << " " << this->node_bounding_box.min.z << std::endl;
	// // Hehe // Hehe std::cout << this->node_bounding_box.max.x << " " << this->node_bounding_box.max.y << " " << this->node_bounding_box.max.z << std::endl;
	// // Hehe // Hehe std::cout << "+-+-+-Node Bounding Box Information Ends: -+-+-+	" << std::endl;

	
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
		// // Hehe // Hehe std::cout << "Min " << surface_ptr->bounding_box.min.x << " " << surface_ptr->bounding_box.min.y << " " << surface_ptr->bounding_box.min.z << std::endl;
		// // Hehe // Hehe std::cout << "Max " << surface_ptr->bounding_box.max.x << " " << surface_ptr->bounding_box.max.y << " " << surface_ptr->bounding_box.max.z << std::endl;
	}
	
	
	// // Hehe // Hehe std::cout << "Chosen Axis: " << axis << std::endl;
	// // Hehe // Hehe std::cout << "Spliiting Position: " << splitting_position << std::endl;

	for(Surface* surface_ptr : this->surfaces_inside){
		// // Hehe // Hehe std::cout << "Surface Information -------------------------------------" << std::endl;
		// Hehe // Hehe std::cout << surface_ptr->bounding_box.min[axis] << " " << surface_ptr->bounding_box.max[axis] << std::endl;
		// Hehe // Hehe std::cout << splitting_position << std::endl;
		// // Hehe // Hehe std::cout << "Surface Information Ends-------------------------------------" << std::endl;

		double bounding_box_center = (surface_ptr->bounding_box.max[axis] + surface_ptr->bounding_box.min[axis]) / 2;
		// // Hehe // Hehe std::cout << "Bounding Box Center: " << bounding_box_center<< std::endl;
	

		if(bounding_box_center < splitting_position){
			this->left_node->surfaces_inside.push_back(surface_ptr);
			this->is_leaf_node = 0;

			// // Hehe // Hehe std::cout << "Put to Left" << std::endl;

			added_to_left += 1;
		
		}
		// This tie-breaker funnily works! :)
		else if(bounding_box_center == splitting_position && added_to_left % 2 == 0){
			this->left_node->surfaces_inside.push_back(surface_ptr);
			this->is_leaf_node = 0;

			// // Hehe // Hehe std::cout << "Put to Left" << std::endl;

			added_to_left += 1;
		
		}
		else{
			this->right_node->surfaces_inside.push_back(surface_ptr);
			this->is_leaf_node = 0;

			// // Hehe // Hehe std::cout << "Put to Right" << std::endl;
			
			added_to_right += 1;
		
		}
	}

	// // Hehe // Hehe std::cout << "Added to left: " << added_to_left << std::endl;
	// // Hehe // Hehe std::cout << "Added to right: " << added_to_right << std::endl;

	// // Hehe // Hehe std::cout << "Called Recursion on left" << std::endl;
		
	this->left_node->reallyCreateBVH();
		
	// // Hehe // Hehe std::cout << "Called Recursion on right" << std::endl;
	this->right_node->reallyCreateBVH();
		


	// // Hehe // Hehe std::cout << "Exiting reallyCreateBVH" << std::endl;
}

void BVH_Node::printBVH_Information(int level){
	if(level == 0){
		// Hehe // Hehe std::cout << "Level 0 has " << this->surfaces_inside.size() << " surfaces." << std::endl;
	}
	static int index_assignment = 1;
    if (this->surfaces_inside.size() == 0){
        // // Hehe // Hehe std::cout << "This is a leaf node 0" << std::endl;
        return;
    }
	if (this->surfaces_inside.size() == 1){
		this->index = index_assignment++;
        // Hehe // Hehe std::cout << "This is a leaf node 1, has index: " << this->index << "." << std::endl;
		// Hehe // Hehe std::cout << "|---- Bounding box limits:" << std::endl;
		// Hehe // Hehe std::cout << "|---- " << this->node_bounding_box.min.x << " " << this->node_bounding_box.min.y << " " << this->node_bounding_box.min.z << std::endl;
	// Hehe // Hehe std::cout << "|---- " << this->node_bounding_box.max.x << " " << this->node_bounding_box.max.y << " " << this->node_bounding_box.max.z << std::endl;
		// Hehe // Hehe std::cout << "|----" << "Bounding box limits end:" << std::endl;
        return;
    }
    else{
        // Hehe // Hehe std::cout << "Printing information of left to level: " << level << ", has surfaces: " << this->left_node->surfaces_inside.size() << std::endl;
		if(this->left_node) this->left_node->printBVH_Information(level + 1);

        // Hehe // Hehe std::cout << "Printing information of right to level: " << level << ", has surfaces: " << this->right_node->surfaces_inside.size() << std::endl;
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
			// Hehe // Hehe std::cout << "Case 5" << std::endl;
		}
		// 
        // No intersection with the bounding box, return the default value
		// // Hehe // Hehe std::cout << "No bounding box intersection." << std::endl;

		// // Hehe // Hehe std::cout << "Bounding box limits:" << std::endl;
		// // Hehe // Hehe std::cout << this->node_bounding_box.min.x << " " << this->node_bounding_box.min.y << " " << this->node_bounding_box.min.z << std::endl;
	// // Hehe // Hehe std::cout << this->node_bounding_box.max.x << " " << this->node_bounding_box.max.y << " " << this->node_bounding_box.max.z << std::endl;
		// // Hehe // Hehe std::cout << "Bounding box limits end:" << std::endl;

		// // Hehe // Hehe std::cout << "Ray Information" << std::endl;
		// // Hehe // Hehe std::cout << "Direction " << ray.d.x << ", " << ray.d.y << ", " << ray.d.z << std::endl;
		// // Hehe // Hehe std::cout << "Direction " << ray.o.x << ", " << ray.o.y << ", " << ray.o.z << std::endl;

        return bvhi_final;
    }

    if (this->is_leaf_node == 1) {
		
		if(ray.x == 800 && ray.y == 540){
			// Hehe // Hehe std::cout << "Case 6" << std::endl;
		}
        // If it's a leaf node and option is 2, directly compute the intersection with the surface
		Interaction si;
		if(option == 2){
        	si = this->surfaces_inside[0]->rayIntersect(ray);
		}

		// If the option is 3, there should be a BVH ready for each surface as well, go inside it.
		if(option == 3){
		
			si = this->surfaces_inside[0]->bvh_node->rayIntersect_ForEachSurface(ray, this->surfaces_inside[0]);
		
		}

        // // Hehe // Hehe std::cout << "Leaf node intersection. Surface index: " << this->index << ", t: " << si.t << std::endl;
        return si;
    }
    else {
		if(ray.x == 800 && ray.y == 540){
			// Hehe // Hehe std::cout << "This is the target ray" << std::endl;
		}
        // Recursive intersection with both left and right nodes
        Interaction left_intersection = this->left_node->rayIntersect(ray);
        Interaction right_intersection = this->right_node->rayIntersect(ray);

        // Check which intersection is closer
        if (left_intersection.didIntersect && right_intersection.didIntersect) {
			if(ray.x == 800 && ray.y == 540){
				// Hehe // Hehe std::cout << "Case 1" << std::endl;
			}
            float left_distance = left_intersection.t;
            float right_distance = right_intersection.t;

            // Choose the closer intersection
            if (left_distance < right_distance) {
                bvhi_final = left_intersection;
                
            } else {
                bvhi_final = right_intersection;
                
            }
        } else if (left_intersection.didIntersect) {
			if(ray.x == 800 && ray.y == 540){
				// Hehe // Hehe std::cout << "Case 2" << std::endl;
			}
            bvhi_final = left_intersection;
            
        } else if (right_intersection.didIntersect) {
			if(ray.x == 800 && ray.y == 540){
				// Hehe // Hehe std::cout << "Case 3" << std::endl;
			}
            bvhi_final = right_intersection;
            
        }
		if(ray.x == 800 && ray.y == 540){
			// Hehe // Hehe std::cout << "Case 4" << std::endl;
		}
    }

    // // Hehe // Hehe std::cout << "Final intersection result: " << bvhi_final.didIntersect << ", t: " << bvhi_final.t << std::endl;

    return bvhi_final;
}

void BVH_Node::createBVH_ForEachSurface(Surface* surface){
	surface->bvh_node->node_bounding_box = surface->bounding_box;
	
	for(Vector3i face : surface->indices){	
		this->faces_inside.push_back(face);
	}

	surface->marryTrianglesWithBoundingBoxes();

	this->reallyCreateBVH_ForEachSurface(surface);
}

void BVH_Node::reallyCreateBVH_ForEachSurface(Surface* surface){
	// // Hehe std::cout << "Here: " << __LINE__ << std::endl;
	if(this->faces_inside.size() == 0){
	
		this->is_leaf_node = 1;
		return;
	}
	// // Hehe std::cout << "Here: " << __LINE__ << std::endl;
	

	if(this->faces_inside.size() == 1){
	
		this->node_bounding_box.min = surface->vertices[faces_inside[0][0]];
		this->node_bounding_box.max = surface->vertices[faces_inside[0][0]];

		this->node_bounding_box.update(surface->vertices[faces_inside[0][1]]);
		this->node_bounding_box.update(surface->vertices[faces_inside[0][2]]);

		this->is_leaf_node = 1;
	
		return;
	}
	// // Hehe std::cout << "Here: " << __LINE__ << std::endl;

	
	if(this->faces_inside.size() >= 2){
	
		this->node_bounding_box.min = surface->vertices[faces_inside[0][0]];
		this->node_bounding_box.max = surface->vertices[faces_inside[0][0]];

		this->node_bounding_box.update(surface->vertices[faces_inside[0][1]]);
		this->node_bounding_box.update(surface->vertices[faces_inside[0][2]]);

		this->is_leaf_node = 0;
	
	}

	// // Hehe std::cout << "Here: " << __LINE__ << std::endl;
	
	for(int i = 0; i < faces_inside.size(); i++){
		BoundingBox* bbptr = new BoundingBox();
		bbptr->min = bbptr->max = surface->vertices[faces_inside[i][0]];
		bbptr->update(surface->vertices[faces_inside[i][1]]);
		bbptr->update(surface->vertices[faces_inside[i][2]]);

		this->node_bounding_box.update(surface->vertices[faces_inside[i][0]]);
		this->node_bounding_box.update(surface->vertices[faces_inside[i][1]]);
		this->node_bounding_box.update(surface->vertices[faces_inside[i][2]]);
	
	}
	// // Hehe std::cout << "Here: " << __LINE__ << std::endl;

	Vector3d range = this->node_bounding_box.max - this->node_bounding_box.min;
	

	int axis = 0;
	if(range.y > range.x) axis = 1;
	if(range.z > range[axis]) axis = 2;

	
	double min_value = 1e30;
	double max_value = -1e30;

	
	for(int i = 0; i < this->faces_inside.size(); i++){
		float center_of_bb = (
			surface->vertices[faces_inside[i][0]][axis] + 
			surface->vertices[faces_inside[i][1]][axis] +
			surface->vertices[faces_inside[i][2]][axis]
		) / 3;


		if(center_of_bb < min_value){
			min_value = center_of_bb;
		}
	
		if(center_of_bb > max_value){
			max_value = center_of_bb;
		}
	// // Hehe std::cout << "Here: " << __LINE__ << std::endl;
	
	}

	double splitting_position = (min_value + max_value) / 2;
	// Hehe // Hehe std::cout << "Splitting Position: " << splitting_position << std::endl;

	
	this->left_node = new BVH_Node();
	this->right_node = new BVH_Node();

	
	int added_to_left = 0;
	int added_to_right = 0;

	// // Hehe std::cout << "Here: " << __LINE__ << std::endl;
	
	for(int i = 0; i < this->faces_inside.size(); i++){
		double centroid = (
			surface->vertices[faces_inside[i][0]][axis] +
			surface->vertices[faces_inside[i][1]][axis] +
			surface->vertices[faces_inside[i][2]][axis]
		) / 3;
	
		// Hehe // Hehe std::cout << "Centroid: " << centroid << ", Splitting position: " << splitting_position << ", Difference: " << std::abs(centroid - splitting_position) << std::endl;

		int a = std::abs(centroid - splitting_position) <= 0.0001;
		// Hehe std::cout << "a: " << a << std::endl;

		if(centroid < splitting_position && std::abs(centroid - splitting_position) > 0.0001){
			this->left_node->faces_inside.push_back(this->faces_inside[i]);
			this->is_leaf_node = 0;
			// Hehe std::cout << "Added to left A, added to left: " << added_to_left << std::endl;
			added_to_left += 1;
		}
		else if(std::abs(centroid - splitting_position) <= 0.0001 && added_to_left % 2 == 0){
			this->left_node->faces_inside.push_back(this->faces_inside[i]);
			this->is_leaf_node = 0;
			// Hehe std::cout << "Added to left B" << std::endl;

			added_to_left += 1;
		}
		else{
			this->right_node->faces_inside.push_back(this->faces_inside[i]);
			this->is_leaf_node = 0;
			// Hehe std::cout << "Added to right" << std::endl;

			added_to_right += 1;
		}
	
	// // Hehe std::cout << "Here: " << __LINE__ << std::endl;
	}
	// Hehe std::cout << "Added to Left: " << added_to_left << ", Added to Right: " << added_to_right << std::endl;
	
	// // Hehe std::cout << "Here: " << __LINE__ << std::endl;
	this->left_node->reallyCreateBVH_ForEachSurface(surface);
	// // Hehe std::cout << "Here: " << __LINE__ << std::endl;
	
	this->right_node->reallyCreateBVH_ForEachSurface(surface);
	// // Hehe std::cout << "Here: " << __LINE__ << std::endl;
	
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
			// Hehe // Hehe std::cout << this->faces_inside.size() << std::endl;
			
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

void BVH_Node::printSubBVHInformation(Surface* surface, int level){
	if(level == 0){
		// Hehe // Hehe std::cout << "Level 0 has " << this->faces_inside.size() << " surfaces." << std::endl;
	}
	static int index_assignment = 1;
    if (this->faces_inside.size() == 0){
        // // Hehe // Hehe std::cout << "This is a leaf node 0" << std::endl;
        return;
    }
	if (this->faces_inside.size() == 1){
		this->index = index_assignment++;
        // Hehe // Hehe std::cout << "This is a leaf node 1, has index: " << this->index << "." << std::endl;
	// 	// Hehe // Hehe std::cout << "|---- Bounding box limits:" << std::endl;
	// 	// Hehe // Hehe std::cout << "|---- " << this->node_bounding_box.min.x << " " << this->node_bounding_box.min.y << " " << this->node_bounding_box.min.z << std::endl;
	// // Hehe // Hehe std::cout << "|---- " << this->node_bounding_box.max.x << " " << this->node_bounding_box.max.y << " " << this->node_bounding_box.max.z << std::endl;
	// 	// Hehe // Hehe std::cout << "|----" << "Bounding box limits end:" << std::endl;
        return;
    }
    else{
        // Hehe // Hehe std::cout << "Printing information of left to level: " << level << ", has surfaces: " << this->left_node->faces_inside.size() << std::endl;
		if(this->left_node) this->left_node->printSubBVHInformation(surface, level + 1);

        // Hehe // Hehe std::cout << "Printing information of right to level: " << level << ", has surfaces: " << this->right_node->faces_inside.size() << std::endl;
		if(this->right_node) this->right_node->printSubBVHInformation(surface, level + 1);
    }
    return;
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