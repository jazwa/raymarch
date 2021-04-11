
#include <stdint.h>
#include <cstdlib>
#include <Eigen/Dense>
#include <vector>
#include <map>
#include <texture.h>
#include <shape.h>
#include <sample_scenes/helix.h>
#include <sample_scenes/moving-torus.h>
#include <sample_scenes/lighting.h>
#include <camera.h>
#include <scene.h>
#include <light.h>
#include <thread>
#include <simulation_option.h>
#include <cmath>

using namespace Eigen;
class Simulation {

    private:
        // Holds pixel colors of current image after render step
        int *frame_buffer;
        // Camera to shoot rays from
        Camera cam;

        // Worker pool, default initialized
        std::vector<std::thread> workers;

        void raymarch_worker_thread(int idx, int work);

    public:
        /* Options for the simulation output */
        const float eps = 0.0005;

        SimulationOption options;
        std::unique_ptr<Scene> scene;


        Simulation() {
            /* cache align to hopefully reduce false sharing */
            /* TODO find a better way to represent this */
            frame_buffer = static_cast<int*>(aligned_alloc(64, sizeof(int)*3*options.width*options.height));
            cam = Camera(Vector3f(0,0,0), options.width, options.height, options.fov);
            
            // choose scene to create
            scene = options.get_scene();

        }

        ~Simulation() {
            free(frame_buffer);
        }

        /* determine the color of every pixel, write to the frame_buffer */
        void render_step();

        Vector3i raymarch(Vector3f& dir);

        Vector3f light_contribution(std::shared_ptr<PointLight> light, Vector3f surface_point, std::shared_ptr<Shape> shape);

        /* return a pointer to the frame_buffer, not the safest */
        int* get_current_frame() {
            return frame_buffer;
        }

};