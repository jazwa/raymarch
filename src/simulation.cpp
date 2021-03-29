
#include "simulation.h"
#include <iostream>
#include <limits>

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

void Simulation::render_step() {

    #pragma omp parallel for schedule(dynamic, 64)
    for (int idx = 0; idx < height*width; idx++) {
            
        int yidx = idx / width;
        int xidx = idx % width;

        Vector3d pixel_color = raymarch(cam.pixel_ray(xidx, yidx));

        /* write to the frame buffer */
        int fb_idx = 3*(yidx*width + xidx);
        frame_buffer[fb_idx] = pixel_color[0];
        frame_buffer[fb_idx+1] = pixel_color[1];
        frame_buffer[fb_idx+2] = pixel_color[2];
    }
}