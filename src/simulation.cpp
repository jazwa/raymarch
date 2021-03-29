
#include "simulation.h"
#include <iostream>
#include <limits>
#include <thread>

std::shared_ptr<Shape> Simulation::nearest_shape(Vector3f p) {

    std::shared_ptr<Shape> ret = nullptr;
    float min_distance = std::numeric_limits<float>::infinity();

    for (auto& s : scene_shapes) {
        float d = s->sdf(p);
        if (d < min_distance) {
            ret = s;
            min_distance = d;
        }
    }
    return ret;
}

Vector3d Simulation::raymarch(Vector3f dir) {
    int max_steps = 128;
    Vector3f curr = cam.origin();

    for (int step = 0; step < max_steps; step++) {
        
        if (abs(curr[0]) >= sim_bounds || 
            abs(curr[1]) >= sim_bounds || 
            abs(curr[2]) >= sim_bounds) 
        {
            return background_color;
        }

        auto nearest = nearest_shape(curr);

        if (nearest == nullptr) {
            /* TODO handle this */
            return background_color;
        }
        float nearest_dist = nearest->sdf(curr);
        if (nearest_dist < eps) {
            return nearest->get_color();
        }

        /* move the ray forward by the length of the nearest shape */
        curr = curr + nearest_dist*dir;
    }
    return background_color;
}


void Simulation::raymarch_worker_thread(int i, int work) {

    for (int idx = i; idx < i+work && idx < height*width; idx++) {
        int yidx = idx / width;
        int xidx = idx % width;

        Vector3f dir = cam.pixel_ray(xidx, yidx);
        Vector3d pixel_color = raymarch(dir);

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

    /* I really don't know what I'm doing here */
    for (int t = 0; t < num_threads; t++) {
        workers.push_back(std::thread(&Simulation::raymarch_worker_thread, this, t*work_per_thread, work_per_thread));
    }

    for (auto& t : workers) {
        if (t.joinable())
            t.join();
    }
}