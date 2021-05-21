#ifndef LIGHT_SCENE_H
#define LIGHT_SCENE_H

#include "scene.h"
#include "shape.h"
#include "materials/material.h"
#include "materials/phong_material.h"
#include <Eigen/Core>

using Eigen::Vector3f;
using std::make_shared, std::shared_ptr;

class LightingScene : public Scene {
    private:
        const int width = 1280;
        const int height = 720;
        const float fov = 90.0;

        shared_ptr<Sphere> sphere;
        shared_ptr<PhongMaterial> sphere_mat = make_shared<PhongMaterial>(Vector3f(0.8, 0.6, 0.1), 1.0, 2.0, 0.4, 16.0);
        shared_ptr<Torus> torus;
        shared_ptr<PhongMaterial> torus_mat = make_shared<PhongMaterial>(Vector3f(0.3, 0.6, 0.8), 1.0, 1.0, 0.2, 16.0);
        
        int current_step;

        float torus_frame_rotate;
        float sphere_frame_z_disp;

        shared_ptr <PointLight> light;
    
    public:
        LightingScene() {
            this->cam = Camera(Vector3f(0,0,0), this->width, this->height, this->fov);
            this->scene_time_steps = 32;
            this->current_step = 0;

            Vector3f sphere_start = Vector3f(-1.0, 0.0, 2.0);
            Vector3f sphere_mid = Vector3f(-1.0, 0.0, 1.5);

            torus_frame_rotate = (2.0*M_PI) / ((float) this->scene_time_steps);
            sphere_frame_z_disp = (2.0*(sphere_start(2)-sphere_mid(2))) / ((float) this->scene_time_steps);

            torus = make_shared<Torus>(0.3, 0.1, torus_mat);
            torus->apply_translate(Vector3f(0.5, -0.5, 1.2));
            add_shape(torus);

            sphere = make_shared<Sphere>(sphere_start, 0.4, sphere_mat);
            add_shape(sphere);

            light = make_shared<PointLight>(Vector3f(0.0,0.2, 1.0), 3.0, Vector3f(255,255,255));
            add_light(light);

        }

        void step_time() {
            torus->apply_rotate_x(torus_frame_rotate);

            Vector3f sphere_direction;
            if (current_step >= (int)(this->scene_time_steps/2)) {
                sphere_direction = Vector3f(0.0, 0.0, 1.0);
            } else {
                sphere_direction = Vector3f(0.0, 0.0, -1.0);
            }
            sphere->apply_translate(sphere_direction, sphere_frame_z_disp);

            current_step++;
        }
};

#endif