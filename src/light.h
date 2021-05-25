#ifndef LIGHT_H
#define LIGHT_H

#include <Eigen/Core>
#include <cmath>

using Eigen::Vector3i, Eigen::Vector3f;


class Light {
    public:
        virtual Vector3f intensity(Vector3f p) = 0;
        virtual ~Light() {}
};

/* TODO: directional light - this might require a bit of refactoring */

class PointLight : public Light {

    public:
        Vector3f tint;
        float s;
        Vector3f position;
    
        PointLight(Vector3f position, float s, Vector3f tint) {
            this->position = position;
            this->s = s;
            this->tint = tint;
        }

        Vector3f intensity(Vector3f p) {
            float d2 = (this->position - p).squaredNorm();
            return (this->s * this->tint) / d2;

        }
};

#endif