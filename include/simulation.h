

#include <stdint.h>
#include <cstdlib>
#include <shape.h>
#include <camera.h>
#include <Eigen/Dense>
#include <vector>
#include <map>

using namespace Eigen;

class Simulation {

    private:
        /* Holds pixel colors of current image after render step */
        int *frame_buffer;
        /* Camera to shoot rays from */
        Camera cam;

        void raymarch_worker_thread(int idx, int work);

        /* shapes in the scene */
        std::vector<std::shared_ptr<Shape>> scene_shapes;

        const float eps = 0.005;
        const float sim_bounds = 6.0;
        const Vector3d background_color = Vector3d(222,222,222);

    public:
        /* Options for the simulation output */
        const int width = 1280;
        const int height = 720;
        const float fov = 90.0;
        const int num_frames = 1;

        Simulation() {
            /* cache align to hopefully reduce false sharing */
            /* TODO find a better way to represent this */
            frame_buffer = static_cast<int*>(aligned_alloc(64, sizeof(int)*3*width*height));
            cam = Camera(Vector3f(0,0,0), width, height, fov);

            /* scene_shapes.push_back(std::make_shared<Sphere>(Vector3f(0,-1.0,3.0), 1.0, Vector3d(255,30,30)));
            scene_shapes.push_back(std::make_shared<Plane>(Vector3f(-1, -1, 2), Vector3f(0, 0.5, -0.1), Vector3d(30,30,200)));
            scene_shapes.push_back(std::make_shared<Plane>(Vector3f(-1, 0, 3), Vector3f(0.5, 0.5, -1.0), Vector3d(30,200,45)));
            scene_shapes.push_back(std::make_shared<Capsule>(Vector3f(-1.0,0.0,3.0), Vector3f(1.0, 1.0, 2.0), 0.2, Vector3d(255, 30,30))); */

            /* example dna sequence */
            std::string dna_seq = "ggggaacccgcatgaaggtccgagtgagggcatgaacaagtcctactccacggtacggacattggctctgaacgcggaggtcatgtgttgttatgggggc";
            
            std::map<char, char> comp_base = {
                {'a', 't'},
                {'t', 'a'},
                {'c', 'g'},
                {'g', 'c'}
            };
            std::map<char, Vector3d> base_color = {
                {'a', Vector3d(0, 127, 255)}, // adenine -> azure
                {'t', Vector3d(253, 245, 1)}, // thymine -> tweety bird
                {'c', Vector3d(20, 253, 20)}, // guanine -> green
                {'g', Vector3d(150, 10, 24)}  // cytosine -> carmine
            };

            /* this might not be the right name */
            Vector3d pentose_color = Vector3d(170, 120, 230);

            /* simplified double helix */ 
            for (int y_idx = 0; y_idx <= 50; y_idx++) { 
                
                float y = -2.5 + 0.1*((float)y_idx);
                float cir_rad = 0.115;
                float snd_point_bias = 0.62;
                float z_bias = 3.0;
                float cyl_rad = 0.03;
                Vector3f pointA, pointB, midpoint;

                pointA << 0.5*sin(4.0*y),
                          y,
                          z_bias + 0.5*cos(4.0*y);

                pointB << 0.5*sin(4.0*(y+snd_point_bias)),
                          y,
                          z_bias + 0.5*cos(4.0*(y+snd_point_bias));

                midpoint = pointA + 0.5*(pointB - pointA);

                scene_shapes.push_back(std::make_shared<Sphere>(pointA, cir_rad, pentose_color));
                scene_shapes.push_back(std::make_shared<Sphere>(pointB, cir_rad, pentose_color));
                scene_shapes.push_back(std::make_shared<Capsule>(pointA, midpoint, cyl_rad, base_color[dna_seq[y_idx]]));
                scene_shapes.push_back(std::make_shared<Capsule>(midpoint, pointB, cyl_rad, base_color[comp_base[dna_seq[y_idx]]]));
            }
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