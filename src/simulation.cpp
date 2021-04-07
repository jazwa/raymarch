
#include "simulation.h"
#include "scene.h"
#include <iostream>
#include <limits>
#include <light.h>
#include <thread>


Vector3i Simulation::light_contribution(std::shared_ptr<PointLight> light, Vector3f surface_point, std::shared_ptr<Shape> shape) {
    Vector3f curr = light->position;
    (void) surface_point;
    (void) shape;

    return Vector3i(0,0,0);
}

Vector3i Simulation::raymarch(Vector3f& dir) {
    int max_steps = 128;
    Vector3f curr = cam.origin();

    for (int step = 0; step < max_steps; step++) {
        
        if (abs(curr[0]) >= scene->bounds || 
            abs(curr[1]) >= scene->bounds || 
            abs(curr[2]) >= scene->bounds) 
        {
            return scene->background.get_pixel_color();
        }

        auto nearest = scene->nearest_shape(curr);

        if (nearest == nullptr) {
            /* TODO handle this */
            return scene->background.get_pixel_color();
        }

        float nearest_dist = nearest->wdist(curr);
        if (nearest_dist < eps) {

            Vector3i pixel_color = nearest->get_texture().get_pixel_color();
            
            // calculate the light contributions 
            for (std::shared_ptr<PointLight> light: scene->lights) {
                pixel_color += light_contribution(light, curr, nearest); // color is additive?
            }

            return pixel_color;
        }

        /* move the ray forward by the length of the nearest shape */
        curr = curr + nearest_dist*dir;
    }
    return scene->background.get_pixel_color();
}


void Simulation::raymarch_worker_thread(int i, int work) {

    for (int idx = i; idx < i+work && idx < height*width; idx++) {
        int yidx = idx / width;
        int xidx = idx % width;

        Vector3f dir = cam.pixel_ray(xidx, yidx);
        Vector3i pixel_color = raymarch(dir);

        /* write to the frame buffer */
        int fb_idx = 3*(yidx*width + xidx);
        frame_buffer[fb_idx] = pixel_color[0];
        frame_buffer[fb_idx+1] = pixel_color[1];
        frame_buffer[fb_idx+2] = pixel_color[2];
    }
}

void Simulation::render_step() {

    int num_threads = 16;

    int work_per_thread = ceil((height*width) / (double)num_threads);
    /* Nearest upper multiple of cache line size */
    work_per_thread = ((work_per_thread-1)/64) * 64 + 64;
    
    std::vector<std::thread> workers;

    for (int t = 0; t < num_threads; t++) {
        workers.push_back(std::thread(&Simulation::raymarch_worker_thread, this, t*work_per_thread, work_per_thread));
    }

    for (auto& t : workers) {
        if (t.joinable())
            t.join();
    }

    this->scene->step_time();
}