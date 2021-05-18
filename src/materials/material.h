#ifndef MATERIAL_H
#define MATERIAL_H

/* Material class for surface properties.
*/

#include <Eigen/Core>
#include <memory>
#include <light.h> // TODO refactor so this does not depend on light.h

using Eigen::Vector3i, Eigen::Vector3f;

class Material {

    public:
    
        Vector3i color;
        float albedo = 0.18;

        virtual Vector3i shade(Vector3f location, Vector3f normal, std::vector<std::shared_ptr<PointLight>> lights) = 0;

        Material(int r, int g, int b) {
            color = Vector3i(r,g,b);
        }
        Material(Vector3i& cvec) {
            color = cvec;
        }
        Material() {
            color = Vector3i(0,0,0); // black
        }

        virtual ~Material() {}
};

#endif