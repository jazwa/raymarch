#ifndef SOLID_MATERIAL_H
#define SOLID_MATERIAL_H

/* No shading, just a color
*/

using std::vector;

#include <Eigen/Core>
#include "material.h"

using Eigen::Vector3i, Eigen::Vector3f;

class SolidMaterial: public Material {
    public:
        SolidMaterial(int r, int g, int b) {
            color = Vector3i(r,g,b);
        }

        Vector3i shade(Vector3f location, Vector3f normal, std::vector<std::shared_ptr<PointLight>> lights) {
            (void) location;
            (void) normal;
            (void) lights;

            return this->color;
        }

        Vector3i get_color() {
            return color;
        }

};

#endif