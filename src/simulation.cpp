
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
                return Vector3f(1.0, 1.0, 1.0) * ((abs(dir.dot(n)) * light->luminosity)/(4.0*M_PI*distance));
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

            Vector3i pixel_color = scene->shade(curr, normal, nearest->get_material());

            Vector3i pixel_color = nearest->get_texture().get_pixel_color();

            if (this->scene->lighting) {
                // calculate the light contributions 
                Vector3f light_modifier = Vector3f(0.0,0.0,0.0);

                for (std::shared_ptr<PointLight> light: scene->lights) {
                    light_modifier += light_contribution(light, curr, nearest); 
                }

                light_modifier = vclamp(light_modifier + scene->ambient_light, 1.0, 0.0);
                //std::cout << light_modifier << std::endl;

                return calc_color(pixel_color, light_modifier); 
            } else {
                // no lighting, return pixel color as is
                return pixel_color;
            }
        }

        /* move the ray forward by the length of the nearest shape */
        curr = curr + nearest_dist*dir;
    }
    return this->scene->background.get_pixel_color();
}


void Simulation::raymarch_worker_thread(int i, int work) {

    for (int idx = i; idx < i+work && idx < options.height*options.width; idx++) {
        int yidx = idx / options.width;
        int xidx = idx % options.width;

        Vector3f dir = cam.pixel_ray(xidx, yidx);
        Vector3i pixel_color = raymarch(dir);

        /* write to the frame buffer */
        int fb_idx = 3*(yidx*options.width + xidx);
        frame_buffer[fb_idx] = pixel_color[0];
        frame_buffer[fb_idx+1] = pixel_color[1];
        frame_buffer[fb_idx+2] = pixel_color[2];
    }
}

void Simulation::render_step() {

    int work_per_thread = ceil((options.height*options.width) / options.num_threads);
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