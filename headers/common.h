#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <cmath>

#include "vec.h"

#include "json/include/nlohmann/json.hpp"

#define M_PI 3.14159263

extern int option;

extern int print_once;

struct Ray {
    Vector3d o, d;
    double t = 1e30;
    double tmax = 1e30;

    // Original Pixels: For the time being - for debugging
    int x;
    int y;

    Ray(Vector3d origin, Vector3d direction, double t = 1e30, double tmax = 1e30)
        : o(origin), d(direction), t(t), tmax(tmax) {};
};

struct Interaction {
    Vector3d p, n;
    double t = 1e30;
    bool didIntersect = false;
};

struct BVH_Node;