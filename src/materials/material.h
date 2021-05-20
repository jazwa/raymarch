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
        /* Can be seen as a very very very limited pixel shader
           location: location in worldspace of pixel
           normal  : normal of surface at location
           cam     : camera location
           lights  : lights which affect the surface location 
           shapes  : other shapes in the simulation */
        virtual Vector3i shade(
            Vector3f location,
            Vector3f normal, 
            Vector3f cam, 
            std::vector<std::shared_ptr<PointLight>> lights
            //,
            //std::vector<std::shared_ptr<Shape>> shapes
        ) = 0;


        virtual ~Material() {}
};

#endif