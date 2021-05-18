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

        std::shared_ptr<SolidMaterial> solidMat = make_shared<SolidMaterial>(170, 120, 230);
        //std::shared_ptr<CelMaterial> celMat = make_shared<CelMaterial>(170, 120, 230);
        std::shared_ptr<PhongMaterial> phongMat = make_shared<PhongMaterial>(170, 120, 230);


    public:
    
        MaterialTestScene() {
            this->curr_t = 0.0;
            this->dt = 0.0;
            this->scene_time_steps = 1;

            this->add_shape(make_shared<Sphere>(Vector3f(-0.3, 0.0, 1.0), 0.2, solidMat));

        }

        void step_time() {}
};

#endif