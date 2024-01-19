#include "boundingBox.h"

void BoundingBox::update(const Vector3f& point){
	// Using Vector3(T x, T y, T z) : x(x), y(y), z(z) { }
	min = Vector3f(std::min(this->min.x, point.x), std::min(this->min.y, point.y), std::min(this->min.z, point.z));

	max = Vector3f(std::max(this->max.x, point.x), std::max(this->max.y, point.y), std::max(this->max.z, point.z));
}

// Directly using the slab test. Not sure if MOSS would be decided for this. I hope not.

// Ye function yeh nahi batata hai ki kahan intersect hua. Ye bas yahi batata hai ki hua ki nahi hua
Interaction BoundingBox::rayIntersect(Ray ray){
	Interaction bbi; // bounding box interaction
	
	float tx1 = (this->min.x - ray.o.x) / ray.d.x;
	float tx2 = (this->max.x - ray.o.x) / ray.d.x;

    float tmin = std::min(tx1, tx2);
	float tmax = std::max(tx1, tx2);

    float ty1 = (this->min.y - ray.o.y) / ray.d.y;
	float ty2 = (this->max.y - ray.o.y) / ray.d.y;

    tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));

    float tz1 = (this->min.z - ray.o.z) / ray.d.z;
	float tz2 = (this->max.z - ray.o.z) / ray.d.z;

    tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));

    if(tmax >= tmin && tmin < ray.t && tmax > 0){
		bbi.didIntersect = 1;
	}
	else{
		bbi.didIntersect = 0;
	}

	return bbi;
}