#ifndef MATERIAL_TEST_H
#define MATERIAL_TEST_H

#include "scene.h"
#include "shape.h"
#include "light.h"
#include <iostream>
#include <map>
#include "materials/phong_material.h"
#include "materials/solid_material.h"

using std::make_shared;

class MaterialTestScene : public Scene {
    private:
        const int width = 1280;
        const int height = 720;
        const float fov = 90.0;

        std::shared_ptr<SolidMaterial> solidMat = make_shared<SolidMaterial>(170, 120, 230);
        std::shared_ptr<PhongMaterial> phongMat = make_shared<PhongMaterial>(Vector3f(0.8, 0.6, 0.1), 1.0, 0.8, 0.2, 12.0);

    public:
    
        MaterialTestScene() {
            this->cam = Camera(Vector3f(0,0,0), this->width, this->height, this->fov);
            this->curr_t = 0.0;
            this->dt = 0.0;
            this->scene_time_steps = 1;
            this->background = make_shared<SolidMaterial>(222,222,222);

            add_shape(make_shared<Sphere>(Vector3f(-0.3, 0.0, 1.0), 0.2, phongMat));
            
            auto torus = make_shared<Torus>(0.2, 0.1, phongMat);
            torus->apply_translate(Vector3f(0.3, 0.0, 1.0));
            torus->apply_rotate(M_PI/3.0,0,0);
            add_shape(torus);

            add_light(make_shared<PointLight>(Vector3f(0.0,0.2, 0.5), 3.0, Vector3f(255,255,255)));
        }

        void step_time() {}
};

#endif