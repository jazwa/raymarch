#ifndef SOLID_MATERIAL_H
#define SOLID_MATERIAL_H

/* No shading, just a color
*/

using std::vector;

#include <Eigen/Core>

using Eigen::Vector3i;

class SolidMaterial {
    public:
        Vector3i shade(Vector3f location, Vector3f normal, vector<PointLight> lights) {
            return this->color;
        }

};

#endif