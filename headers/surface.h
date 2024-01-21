#pragma once

#include "common.h"
#include "texture.h"
#include "boundingBox.h"

struct Surface {
    std::vector<Vector3f> vertices, normals;
    std::vector<Vector3i> indices;
    std::vector<Vector2f> uvs;

    std::vector<BoundingBox> triangles_ki_bbiyaan;   // I don't know if that is a misnomer. The bbiyaan would also keep the vertices captured. The length of the vector would be the number of indices = number of vertice / 3.
    BVH_Node* bvh_node;     // This is the BVH_Node for the the surface-triangles BVH.

    BoundingBox bounding_box;

    bool isLight;
    uint32_t shapeIdx;

    Vector3f diffuse;
    float alpha;

    Texture diffuseTexture, alphaTexture;

    Interaction rayPlaneIntersect(Ray ray, Vector3f p, Vector3f n);
    Interaction rayTriangleIntersect(Ray ray, Vector3f v1, Vector3f v2, Vector3f v3, Vector3f n);
    Interaction rayIntersect(Ray ray);

    void updateBoundingBox();
    
    void marryTrianglesWithBoundingBoxes();

private:
    bool hasDiffuseTexture();
    bool hasAlphaTexture();
};

std::vector<Surface> createSurfaces(std::string pathToObj, bool isLight, uint32_t shapeIdx);