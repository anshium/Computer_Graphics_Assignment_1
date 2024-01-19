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
    for (int x = 0; x < this->scene.imageResolution.x; x++) {
        for (int y = 0; y < this->scene.imageResolution.y; y++) {
            Ray cameraRay = this->scene.camera.generateRay(x, y);
            Interaction si = this->scene.rayIntersect(cameraRay);

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

int main(int argc, char **argv)
{
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

    for (auto& surface : scene.surfaces) {
        surface.updateBoundingBox(); // Target 1
    }

    if(option == 2){
        bvh_root->createBVH(&scene);
        scene.bvh_root = bvh_root;
    }
    // std::cout << scene.surfaces.size() << std::endl;

    Integrator rayTracer(scene);
    auto renderTime = rayTracer.render();

    std::cout << "Render Time: " << std::to_string(renderTime / 1000.f) << " ms" << std::endl;
    rayTracer.outputImage.save(argv[2]);

    return 0;
}
