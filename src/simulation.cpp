
#include "simulation.h"
#include "scene.h"
#include <iostream>
#include <limits>
#include <cmath>
#include <light.h>
#include <thread>

using std::max, std::min;

Vector3f vclamp(Vector3f v, float mx, float mn) {
    return Vector3f(max(min(v(0), mx), mn), max(min(v(1), mx), mn), max(min(v(2), mx), mn));
}

Vector3i calc_color(Vector3i p, Vector3f m) {
    return Vector3i((int) (p(0)*m(0)), (int) (p(1)*m(1)), (int) (p(2)*m(2)));
}


Vector3f Simulation::light_contribution(std::shared_ptr<PointLight> light, Vector3f surface_point, std::shared_ptr<Shape> shape) {
    
    (void) surface_point;
    (void) shape;

    Vector3f curr = light->position;
    Vector3f dir = (surface_point - light->position).normalized();
    const float distance = (surface_point - light->position).norm();
    const int max_steps = 256;
    float t = 0.0;

    // march in the direction of the surface point
    for (int step = 0; step < max_steps; step++) {

        if (abs(curr[0]) >= scene->bounds || 
            abs(curr[1]) >= scene->bounds || 
            abs(curr[2]) >= scene->bounds) 
        {
            return Vector3f(0.0,0.0,0.0);
        }

        // can be cleaned up
        auto nearest = scene->nearest_shape(curr);
        float nearest_dist = nearest->wdist(curr);
        
        // increment t
        t += nearest_dist;
        curr = light->position + t*dir;

        if (nearest_dist < eps) {
            // really hardcore smoothing
            if ((curr - surface_point).norm() < 16.0*eps) {
                Vector3f n = shape->wnormal(surface_point);
                return Vector3f(1.0, 1.0, 1.0) * ((abs(dir.dot(n)) * light->s)/(4.0*M_PI*distance));
            } else {
                //std::cout << " interference " << std::endl;
                return Vector3f(0.0,0.0,0.0);
            }
        }

        
    }
    return Vector3f(0.0,0.0,0.0);
}

Vector3i Simulation::raymarch(Vector3f& dir) {
    const int max_steps = 256;
    Vector3f curr = scene->cam.location;

    for (int step = 0; step < max_steps; step++) {
        
        if (abs(curr[0]) >= scene->bounds || 
            abs(curr[1]) >= scene->bounds || 
            abs(curr[2]) >= scene->bounds) 
        {
            return scene->background->color;
        }

        auto nearest = scene->nearest_shape(curr);

        if (nearest == nullptr) {
            /* TODO handle this */
            return scene->background->color;
        }

        float nearest_dist = nearest->wdist(curr);

        if (nearest_dist < eps) {
            Vector3f normal = nearest->wnormal(curr);
            return nearest->material->shade(curr, normal, *scene); // TODO: clean up pointers
        }

        /* move the ray forward by the length of the nearest shape */
        curr = curr + nearest_dist*dir;
    }
    return this->scene->background->color;
}


void Simulation::raymarch_worker_thread(int i, int work) {

    const int height = scene->cam.frame_height;
    const int width = scene->cam.frame_width;

    for (int idx = i; idx < i+work && idx < height*width; idx++) {
        int yidx = idx / width;
        int xidx = idx % width;

        Vector3f dir = scene->cam.pixel_ray(xidx, yidx);
        Vector3i pixel_color = raymarch(dir);

        /* write to the frame buffer */
        int fb_idx = 3*(yidx*width + xidx);
        frame_buffer[fb_idx] = pixel_color[0];
        frame_buffer[fb_idx+1] = pixel_color[1];
        frame_buffer[fb_idx+2] = pixel_color[2];
    }
}

void Simulation::render_step() {

    const int height = scene->cam.frame_height;
    const int width = scene->cam.frame_width;

    int work_per_thread = ceil((height*width) / options.num_threads);
    /* Nearest upper multiple of cache line size */
    work_per_thread = ((work_per_thread-1)/64) * 64 + 64;

    for (int t = 0; t < options.num_threads; t++) {
        workers.push_back(std::thread(&Simulation::raymarch_worker_thread, this, t*work_per_thread, work_per_thread));
    }

    for (auto& t : workers) {
        if (t.joinable())
            t.join();
    }

    workers.clear();
    this->scene->step_time();
}