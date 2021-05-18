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

        /* Can be seen as a very very very limited pixel shader
           location: location in worldspace of pixel
           normal  : normal of surface at location
           cam     : camera location
           lights  : lights which affect the surface location */
        virtual Vector3i shade(Vector3f location, Vector3f normal, Vector3f cam, std::vector<std::shared_ptr<PointLight>> lights) = 0;

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