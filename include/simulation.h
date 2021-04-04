
#include <stdint.h>
#include <cstdlib>
#include <Eigen/Dense>
#include <vector>
#include <map>
#include <texture.h>
#include <shape.h>
#include <sample_scenes/helix.h>
#include <sample_scenes/moving-torus.h>
#include <camera.h>
#include <scene.h>
#include <cmath>

using namespace Eigen;

class Simulation {

    private:
        /* Holds pixel colors of current image after render step */
        int *frame_buffer;
        /* Camera to shoot rays from */
        Camera cam;

        void raymarch_worker_thread(int idx, int work);
/* 
        std::vector<std::shared_ptr<Shape>> scene_shapes;
 */
        const float eps = 0.005;

    public:
        /* Options for the simulation output */
        const int width = 1280;
        const int height = 720;
        const float fov = 90.0;
        std::unique_ptr<Scene> scene;


        Simulation() {
            /* cache align to hopefully reduce false sharing */
            /* TODO find a better way to represent this */
            frame_buffer = static_cast<int*>(aligned_alloc(64, sizeof(int)*3*width*height));
            cam = Camera(Vector3f(0,0,0), width, height, fov);
            scene = std::make_unique<Moving_torus_scene>();

            
            //scene_shapes.push_back(std::make_shared<Plane>(Vector3f(-1, -1, 2), Vector3f(0, 0.5, -0.1), Vector3d(30,30,200)));
            //scene_shapes.push_back(std::make_shared<Plane>(Vector3f(-1, 0, 3), Vector3f(0.5, 0.5, -1.0), Vector3d(30,200,45)));
            
            //std::shared_ptr<Capsule> cap = std::make_shared<Capsule>(Vector3f(-1.0,0.0,1.0), Vector3f(1.0, 0.0, 1.0), 0.1, Texture(255, 30,30));
            //cap->apply_translate(Vector3f(0,0.5,0.0));
            //scene_shapes.push_back(cap);

        }

        ~Simulation() {
            free(frame_buffer);
        }

        /* determine the color of every pixel, write to the frame_buffer */
        void render_step();

        Vector3i raymarch(Vector3f& dir);

        /* return a pointer to the frame_buffer, not the safest */
        int* get_current_frame() {
            return frame_buffer;
        }

};