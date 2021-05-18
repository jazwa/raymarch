#ifndef MATERIAL_H
#define MATERIAL_H

/* Material class for surface properties.
*/

#include <Eigen/Core>

using Eigen::Vector3i;

class Material {

    protected:
        virtual ~Material() {}
    public:
        Vector3i color;
        float albedo = 0.18;

        virtual Vector3i shade = 0;

        Material(int r, int g, int b) {
            color = Vector3i(r,g,b);
        }
        Material(Vector3i& cvec) {
            color = cvec;
        }
        Material() {
            color = Vector3i(0,0,0); // black
        }
};

#endif