#ifndef PHONG_MATERIAL_H
#define PHONG_MATERIAL_H

/* Basic Phong shading
*/

using std::vector;

#include <Eigen/Core>

using Eigen::Vector3i;

class PhongMaterial {
    public:
        Vector3i shade(Vector3f location, Vector3f normal, vector<PointLight> lights) {
            return this->color;
        }

};

#endif