#ifndef MATERIAL_TEST_H
#define MATERIAL_TEST_H

#include "scene.h"
#include "shape.h"
#include <iostream>
#include <map>
#include "materials/phong_material.h"
#include "materials/solid_material.h"

using std::make_shared;

class MaterialTestScene : public Scene {
    private:
        const int width = 480;
        const int height = 360;
        const float fov = 90.0;

        std::shared_ptr<SolidMaterial> solidMat = make_shared<SolidMaterial>(170, 120, 230);
        //std::shared_ptr<CelMaterial> celMat = make_shared<CelMaterial>(170, 120, 230);
        std::shared_ptr<PhongMaterial> phongMat = make_shared<PhongMaterial>();


    public:
    
        MaterialTestScene() {
            this->cam = Camera(Vector3f(0,0,0), this->width, this->height, this->fov);
            this->curr_t = 0.0;
            this->dt = 0.0;
            this->scene_time_steps = 1;

            this->add_shape(make_shared<Sphere>(Vector3f(-0.3, 0.0, 1.0), 0.2, solidMat));

        }

        void step_time() {}
};

#endif