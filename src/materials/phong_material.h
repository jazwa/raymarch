#ifndef PHONG_MATERIAL_H
#define PHONG_MATERIAL_H

/* Basic Phong shading
*/

using std::vector;

#include <Eigen/Core>
#include "material.h"

using Eigen::Vector3i, Eigen::Vector3f;

class PhongMaterial: public Material {
    public:
        PhongMaterial(int r, int g, int b) {
            color = Vector3i(r,g,b);
        }

        Vector3i shade(Vector3f location, Vector3f normal, std::vector<std::shared_ptr<PointLight>> lights) {
            (void) location;
            (void) normal;
            (void) lights;

            return this->color;
        }

};

#endif