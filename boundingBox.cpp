#include "boundingBox.h"

void BoundingBox::update(const Vector3f& point){
	// Using Vector3(T x, T y, T z) : x(x), y(y), z(z) { }
	min = Vector3f(std::min(min.x, point.x), std::min(min.y, point.y), std::min(min.z, point.z));

	max = Vector3f(std::max(max.x, point.x), std::max(max.y, point.y), std::max(max.z, point.z));
}