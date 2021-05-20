#ifndef LIGHT_H
#define LIGHT_H

#include <Eigen/Core>
#include <cmath>

using Eigen::Vector3i, Eigen::Vector3f;

class PointLight {

    public:
        Vector3f tint;
        float luminosity;
        Vector3f position;
    
        PointLight(Vector3f position, float luminosity, Vector3f tint) {
            this->position = position;
            this->luminosity = luminosity;
            this->tint = tint;
        }


        Vector3f intensity(Vector3f p) {
            float d2 = (this->position - p).squaredNorm();
            return (this->luminosity * this->tint) / (4 * M_PI * d2);

        }
};

#endif