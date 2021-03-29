

#include <stdint.h>
#include <cstdlib>
#include <shape.h>
#include <camera.h>
#include <Eigen/Dense>
#include <vector>

using namespace Eigen;

class Simulation {

    private:
        /* Holds pixel colors of current image after render step */
        int *frame_buffer;
        /* Camera to shoot rays from */
        Camera cam;
        /* Return color when a ray is cast from orig in the direction dir */
        Vector3d castRay(Vector3f orig, Vector3f dir);
        /* shapes in the scene */
        std::vector<std::shared_ptr<Shape>> scene_shapes;

        const float eps = 0.01;
        const float sim_bounds = 5.0;
        const Vector3d background_color = Vector3d(0,0,0);

    public:
        /* Options for the simulation output */
        const int width = 480;
        const int height = 360;
        const float fov = 90.0;
        const int num_frames = 1;

        Simulation() {
            /* cache align to hopefully reduce false sharing */
            /* TODO find a better way to represent this */
            frame_buffer = static_cast<int*>(aligned_alloc(64, sizeof(int)*3*width*height));
            cam = Camera(Vector3f(0,0,0), width, height, fov);

            scene_shapes.push_back(std::make_shared<Sphere>(Vector3f(0,-1.0,3.0), 1.0, Vector3d(255,30,30)));
            scene_shapes.push_back(std::make_shared<Plane>(Vector3f(-1, -1, 2), Vector3f(0, 0.5, -0.1), Vector3d(30,30,200)));
            scene_shapes.push_back(std::make_shared<Plane>(Vector3f(-1, 0, 3), Vector3f(0.5, 0.5, -1.0), Vector3d(30,200,45)));
        }

        ~Simulation() {
            free(frame_buffer);
        }

        /* determine the color of every pixel, write to the frame_buffer */
        void render_step();

        std::shared_ptr<Shape> nearest_shape(Vector3f p);

        Vector3d raymarch(Vector3f dir);

        /* return a pointer to the frame_buffer, not the safest */
        int* get_current_frame() {
            return frame_buffer;
        }

};