#ifndef LIGHT_H
#define LIGHT_H

#include <Eigen/Core>

using Eigen::Vector3i, Eigen::Vector3f;

class PointLight {

    public:
        Vector3i tint;
        float luminosity;
        Vector3f position;
    
        PointLight(Vector3f position, float luminosity, Vector3i tint) {
            this->position = position;
            this->luminosity = luminosity;
            this->tint = tint;
        }
};

#endif