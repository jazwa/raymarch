#ifndef MATERIAL_H
#define MATERIAL_H

/* Material class for surface properties.
*/

#include <Eigen/Core>
#include <memory>
//#include <scene.h>
//#include <light.h> // TODO refactor so this does not depend on light.h

class Scene;
using Eigen::Vector3i, Eigen::Vector3f;

class Material {

    public:
        // Can be seen as a very very very limited pixel shader 
        virtual Vector3i shade(
            Vector3f location,
            Vector3f normal , 
            const Scene& scene
        ) = 0;

        virtual ~Material() {}
};

#endif