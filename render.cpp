#include "render.h"
#include "bvh.h"

Integrator::Integrator(Scene &scene)
{
    this->scene = scene;
    this->outputImage.allocate(TextureType::UNSIGNED_INTEGER_ALPHA, this->scene.imageResolution);
}

long long Integrator::render()
{
    auto startTime = std::chrono::high_resolution_clock::now();
    int x = 960;
    int y = 540;
    for (int x = 0; x < this->scene.imageResolution.x; x++) {
        for (int y = 0; y < this->scene.imageResolution.y; y++) {
            Ray cameraRay = this->scene.camera.generateRay(x, y);
            cameraRay.x = x;
            cameraRay.y = y;
            Interaction si = this->scene.rayIntersect(cameraRay);
            if(x == 800 && y == 540){
                std::cout << "This is the center point" << std::endl;
                if(si.didIntersect){
                    std::cout << "RAY INTERSECTED" << std::endl;
                }
                else if(!si.didIntersect){
                    std::cout << "RAY DID NOT INTERSECT" << std::endl;
                }
            }
            if (si.didIntersect)
                this->outputImage.writePixelColor(0.5f * (si.n + Vector3f(1.f, 1.f, 1.f)), x, y);
            else
                this->outputImage.writePixelColor(Vector3f(0.f, 0.f, 0.f), x, y);
        }
    }
    auto finishTime = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();
}

// I am not in favour of global variables
int option = 0;

int print_once = 0;

int main(int argc, char **argv)
{
    print_once = 0;
    if (argc != 4) {
        std::cerr << "Usage: ./render <scene_config> <out_path> <option>";
        return 1;
    }
    std::cout << argv[3] << std::endl;
    if(std::stoi(argv[3]) == 0){
        option = 0;
        std::cout << "Using Original" << std::endl;
    }
    else if(std::stoi(argv[3]) == 1){
        option = 1;
        std::cout << "Using PART_1" << std::endl;
    }
    else if(std::stoi(argv[3]) == 2){
        option = 2;
        std::cout << "Using PART_2" << std::endl;
    }

    Scene scene(argv[1]);

    BVH_Node* bvh_root = new BVH_Node();

    // for (auto& surface : scene.surfaces) {
    //     surface.updateBoundingBox(); // Target 1
    // }
    for(int i = 0; i < scene.surfaces.size(); i++){
        (&scene.surfaces[i])->updateBoundingBox();
    }

    if(option == 2){
        // for (auto& surface : scene.surfaces) {
        //     surface.updateBoundingBox(); // Target 1
        // }
        bvh_root->createBVH(&scene);
        scene.bvh_root = bvh_root;

        std::cout << "Bounding box limits:" << std::endl;
		std::cout << bvh_root->node_bounding_box.min.x << " " << bvh_root->node_bounding_box.min.y << " " << bvh_root->node_bounding_box.min.z << std::endl;
	std::cout << bvh_root->node_bounding_box.max.x << " " << bvh_root->node_bounding_box.max.y << " " << bvh_root->node_bounding_box.max.z << std::endl;
		std::cout << "Bounding box limits end:" << std::endl;
    }
    // std::cout << scene.surfaces.size() << std::endl;

    Integrator rayTracer(scene);
    auto renderTime = rayTracer.render();

    std::cout << "Render Time: " << std::to_string(renderTime / 1000.f) << " ms" << std::endl;
    rayTracer.outputImage.save(argv[2]);

    return 0;
}
