#include "camera.h"

Camera::Camera(Vector3d from, Vector3d to, Vector3d up, double fieldOfView, Vector2i imageResolution)
    : from(from),
    to(to),
    up(up),
    fieldOfView(fieldOfView),
    imageResolution(imageResolution)
{
    this->aspect = imageResolution.x / float(imageResolution.y);

    // Determine viewport dimensions in 3D
    double fovRadians = fieldOfView * M_PI / 180.0;
    double h = std::tan(fovRadians / 2.0);
    double viewportHeight = 2.0 * h * this->focusDistance;
    double viewportWidth = viewportHeight * this->aspect;

    // Calculate basis vectors of the camera for the given transform
    this->w = Normalize(this->from - this->to);
    this->u = Normalize(Cross(up, this->w));
    this->v = Normalize(Cross(this->w, this->u));

    // Pixel delta vectors
    Vector3d viewportU = viewportWidth * this->u;
    Vector3d viewportV = viewportHeight * (-this->v);

    this->pixelDeltaU = viewportU / float(imageResolution.x);
    this->pixelDeltaV = viewportV / float(imageResolution.y);

    // Upper left
    this->upperLeft = from - this->w * this->focusDistance - viewportU / 2.0 - viewportV / 2.0;
}

Ray Camera::generateRay(int x, int y)
{
    Vector3d pixelCenter = this->upperLeft + 0.5 * (this->pixelDeltaU + this->pixelDeltaV);
    pixelCenter = pixelCenter + x * this->pixelDeltaU + y * this->pixelDeltaV;

    Vector3d direction = Normalize(pixelCenter - this->from);

    return Ray(this->from, direction);
}