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
            if(x == 100 && y == 100){
                std::cout << "10%" << std::endl;
            }
            else if(x == 200 && y == 200){
                std::cout << "20%" << std::endl;
            }
            else if(x == 300 && y == 300){
                std::cout << "30%" << std::endl;
            }
            else if(x == 400 && y == 400){
                std::cout << "40%" << std::endl;
            }
            else if(x == 500 && y == 500){
                std::cout << "50%" << std::endl;
            }
            else if(x == 600 && y == 600){
                std::cout << "60%" << std::endl;
            }
            else if(x == 700 && y == 700){
                std::cout << "70%" << std::endl;
            }
            else if(x == 800 && y == 800){
                std::cout << "80%" << std::endl;
            }
            else if(x == 900 && y == 900){
                std::cout << "90%" << std::endl;
            }
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
    else if(std::stoi(argv[3]) == 3){
        option = 3;
        std::cout << "Using PART_3" << std::endl;
    }
    
    Scene scene(argv[1]);

    BVH_Node* bvh_root = new BVH_Node();

    for(int i = 0; i < scene.surfaces.size(); i++){
        (&scene.surfaces[i])->updateBoundingBox();
    }

    /**
    int i = 0;
    for(auto& surface : scene.surfaces){
        std::cout << "Surface Information, surface: " << i + 1 << std::endl;
        // for(int j = 0; i < surface.indices.size(); i++){
        //     std::cout << surface.indices[i].x << " " << surface.indices[i].y << " " << surface.indices[i].z << std::endl;
        // }
        std::cout << "Number of vertices: " << surface.vertices.size() << std::endl;
        std::cout << "Number of Indices: " << surface.indices.size() << std::endl;
        std::cout << "Surface Information ends:" << std::endl;
        i++;
    }
    **/

    if(option == 2){
        bvh_root->createBVH(&scene);
        scene.bvh_root = bvh_root;
    }
    
    if(option == 3){
        std::cout << "Reached here" << std::endl;
        bvh_root->createBVH(&scene);
        scene.bvh_root = bvh_root;
        std::cout << "Reached here" << std::endl;

        for(int i = 0; i < scene.surfaces.size(); i++){
            Surface* surface_ptr = &scene.surfaces[i];
            surface_ptr->bvh_node = new BVH_Node();
            for(auto& face : surface_ptr->indices){
                BoundingBox* tempBB = new BoundingBox();                    // Target
                surface_ptr->triangles_ki_bbiyaan.push_back(tempBB);
            }
            surface_ptr->bvh_node->createBVH_ForEachSurface(surface_ptr);

            surface_ptr->bvh_node->printSubBVHInformation(surface_ptr, 0);
            std::cout << "+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+" << std::endl;
        }
        std::cout << "Reached here" << std::endl;
        // scene.bvh_root->printBVH_Information(0);
    }
    
    Integrator rayTracer(scene);
    auto renderTime = rayTracer.render();

    std::cout << "Render Time: " << std::to_string(renderTime / 1000.f) << " ms" << std::endl;
    rayTracer.outputImage.save(argv[2]);

    return 0;
}
