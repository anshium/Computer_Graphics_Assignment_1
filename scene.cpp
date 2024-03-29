#include "scene.h"
#include "bvh.h"

Scene::Scene(std::string sceneDirectory, std::string sceneJson)
{
    nlohmann::json sceneConfig;
    try {
        sceneConfig = nlohmann::json::parse(sceneJson);
    }
    catch (std::runtime_error e) {
        std::cerr << "Could not parse json." << std::endl;
        exit(1);
    }

    this->parse(sceneDirectory, sceneConfig);
}

Scene::Scene(std::string pathToJson)
{
    std::string sceneDirectory;

#ifdef _WIN32
    const size_t last_slash_idx = pathToJson.rfind('\\');
#else
    const size_t last_slash_idx = pathToJson.rfind('/');
#endif

    if (std::string::npos != last_slash_idx) {
        sceneDirectory = pathToJson.substr(0, last_slash_idx);
    }

    nlohmann::json sceneConfig;
    try {
        std::ifstream sceneStream(pathToJson.c_str());
        sceneStream >> sceneConfig;
    }
    catch (std::runtime_error e) {
        std::cerr << "Could not load scene .json file." << std::endl;
        exit(1);
    }

    this->parse(sceneDirectory, sceneConfig);
}

void Scene::parse(std::string sceneDirectory, nlohmann::json sceneConfig)
{
    // Output
    try {
        auto res = sceneConfig["output"]["resolution"];
        this->imageResolution = Vector2i(res[0], res[1]);
    }
    catch (nlohmann::json::exception e) {
        std::cerr << "\"output\" field with resolution, filename & spp should be defined in the scene file." << std::endl;
        exit(1);
    }

    // Cameras
    try {
        auto cam = sceneConfig["camera"];

        this->camera = Camera(
            Vector3d(cam["from"][0], cam["from"][1], cam["from"][2]),
            Vector3d(cam["to"][0], cam["to"][1], cam["to"][2]),
            Vector3d(cam["up"][0], cam["up"][1], cam["up"][2]),
            float(cam["fieldOfView"]),
            this->imageResolution
        );
    }
    catch (nlohmann::json::exception e) {
        std::cerr << "No camera(s) defined. Atleast one camera should be defined." << std::endl;
        exit(1);
    }

    // Surface
    try {
        auto surfacePaths = sceneConfig["surface"];

        uint32_t surfaceIdx = 0;
        int i = 0;
        for (std::string surfacePath : surfacePaths) {
            surfacePath = sceneDirectory + "/" + surfacePath;

            auto surf = createSurfaces(surfacePath, /*isLight=*/false, /*idx=*/surfaceIdx);
            this->surfaces.insert(this->surfaces.end(), surf.begin(), surf.end());

            // Adding the bounding boxes to the surfaces. Don't know if parse is the right place to add it in accordance to SE rules.
            // I don't think I need this already. The surface is a temporary variable, so a dangling type thing might occur.
            // for(Surface surface : this->surfaces){
            //     surface.updateBoundingBox();
            // }

            surfaceIdx = surfaceIdx + surf.size();
            i++; 
        }
        std::cout << i << std::endl;
    }
    catch (nlohmann::json::exception e) {
        std::cout << "No surfaces defined." << std::endl;
    }
}


Interaction Scene::rayIntersect(Ray& ray)
{
    Interaction siFinal;

    siFinal.didIntersect = 1;

    if(option == 0 || option == 1){
        for (auto& surface : this->surfaces) {
            surface.updateBoundingBox(); // Target 1
            Interaction si = surface.rayIntersect(ray);
            if (si.t <= ray.t) {    
                siFinal = si;
                ray.t = si.t;
            }
        }
        if(ray.x == 800 && ray.y == 540){
            // std::cout << "======================Printing available surfaces information     ======================" << std::endl;
            // for(auto& surface : this->surfaces){
            //     std::cout << surface.bounding_box.min.x << " " << surface.bounding_box.min.y << " " << surface.bounding_box.min.z << std::endl;
            //     std::cout << surface.bounding_box.max.x << " " << surface.bounding_box.max.y << " " << surface.bounding_box.max.z << std::endl;

            //     std::cout << "------------------------" << std::endl;
            // }
            // std::cout << "======================Printing available surfaces information ends======================" << std::endl;
        }
    }

    if(option == 2){
        // search in bvh recursively
        if(print_once++ == 0) this->bvh_root->printBVH_Information(0);
        Interaction si = this->bvh_root->rayIntersect(ray);
        if (si.t <= ray.t) {    
            siFinal = si;
            ray.t = si.t;
        }

        if(ray.x == 800 && ray.y == 540){
            // std::cout << "======================Printing available surfaces information     ======================" << std::endl;
            // for(auto& surface : this->bvh_root->surfaces_inside){
            //     std::cout << surface->bounding_box.min.x << " " << surface->bounding_box.min.y << " " << surface->bounding_box.min.z << std::endl;
            //     std::cout << surface->bounding_box.max.x << " " << surface->bounding_box.max.y << " " << surface->bounding_box.max.z << std::endl;

            //     std::cout << "------------------------" << std::endl;
            // }
            // std::cout << "======================Printing available surfaces information ends======================" << std::endl;
        }
    }

    if(option == 3){
            // std::cout << "Here :|| " << __LINE__ << std::endl;
        Interaction si = this->bvh_root->rayIntersect(ray);
            // std::cout << "Here :|| " << __LINE__ << std::endl;
        if (si.t <= ray.t) {    
            siFinal = si;
            ray.t = si.t;
        }
    }

    return siFinal;
}