#ifndef PHONG_MATERIAL_H
#define PHONG_MATERIAL_H

/* Basic Phong shading
*/

using std::vector;

#include <Eigen/Core>
#include "material.h"

using Eigen::Vector3i, Eigen::Vector3f;

class PhongMaterial: public Material {
    private:
        float Ka = 0.0; // ambient reflect coeff
        float Kd = 0.0; // diffuse reflect coeff
        float Ks = 0.0; // specular reflect coeff



    public:
        PhongMaterial(int r, int g, int b) {
            color = Vector3i(r,g,b);
        }

        Vector3i shade(Vector3f location, Vector3f normal, Vector3f cam, std::vector<std::shared_ptr<PointLight>> lights) {
            Vector3f N = normal.normalized();
            Vector3f L, R;

            for (auto& light: lights) {
                L = (light->position - location).normalized();
                R = 2.0 * (L.dot(N))*N - L; // reflect L on plane defined by N
                
            }

};

#endif