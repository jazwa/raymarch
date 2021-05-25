#ifndef SHAPE_H
#define SHAPE_H

#include <Eigen/Dense>
#include <algorithm>
#include <materials/material.h>
#include <object3d.h>
#include <memory>
#include <cmath>

#include <iostream>

using namespace Eigen;

class Shape : public Object3D {
    protected:
        virtual float object_sdf(Vector3f p) = 0;

        // can either be analytical or approximate, prefer analytical
        virtual Vector3f object_normal(Vector3f p) {

            const float eps = 0.002;
            
            const Vector3f dx = Vector3f(1,0,0)*eps;
            const Vector3f dy = Vector3f(0,1,0)*eps;
            const Vector3f dz = Vector3f(0,0,1)*eps;
            float pdist = object_sdf(p);
            Vector3f n = Vector3f(object_sdf(p+dx) - pdist,
                                  object_sdf(p+dy) - pdist,
                                  object_sdf(p+dz) - pdist);
            n.normalize();

            return n;
        };

    public:
        std::shared_ptr<Material> material;
        // worldspace functions get the distance field and surface normals
        float wdist(Vector3f p) {
            return this->object_sdf(this->inv_affine(p));
        }
        Vector3f wnormal(Vector3f p) {
            return this->transform_dir(this->object_normal(this->inv_affine(p)));
        }

        virtual ~Shape() {}
};

class Sphere: public Shape {
    private:
        float radius;

        float object_sdf(Vector3f p) {
            return p.norm() - radius;
        }
        
        /* Vector3f object_normal(Vector3f p) {
            return p.normalized(); // not needed
        } */

    public:
        // initialize the sphere with center at worldspace origin
        Sphere(float radius, std::shared_ptr<Material> material) {
            this->radius = radius;
            this->material = material;
        }

        Sphere(Vector3f center, float radius, std::shared_ptr<Material> material) {
            this->radius = radius;
            this->material = material;
            this->apply_translate(center);
        }

};

class Plane: public Shape {
    private:
        float object_sdf(Vector3f p) {
            // we need the absolute value, otherwise this would define a halfspace
            return abs(p[1]);
        }

        Vector3f object_normal(Vector3f p) {
            (void) p;
            return Vector3f(0,1,0);
        }

    public:
        // initialize plane on x-z axis
        Plane(std::shared_ptr<Material> material) {
            this->material = material;
        }
        Plane(Vector3f point, Vector3f normal, std::shared_ptr<Material> material) {

            this->apply_translate(point);
            // TODO check correctness
            float rz = M_PI - atan(normal[2]/normal[1]);
            float rx = M_PI - atan(normal[2]/normal[0]);
            this->apply_rotate_x(rx);
            this->apply_rotate_z(rz);

            this->material = material;
        }

        Plane(Vector3f rotation, std::shared_ptr<Material> material) {
            this->material = material;
            this->apply_rotate(rotation(0), 0 , rotation(2));
        }

        Plane(float rx, float rz, std::shared_ptr<Material> material) {
            this->material = material;
            this->apply_rotate(rx, 0, rz);
        }

};


class Torus: public Shape {
    private:
        float r_dist;
        float radius;

        float object_sdf(Vector3f p) {
            Vector2f p_to_slice_center = Vector2f(Vector2f(p(0), p(2)).norm() - r_dist, p(1));
            return p_to_slice_center.norm() - radius;
        }

        /* Vector3f object_normal(Vector3f p) {
            Vector3f slice_center = Vector3f(p(0), 0, p(2)).normalized() * r_dist;
            return (p-slice_center).normalized();
        } */

    public:
        Torus(float r_dist, float radius, std::shared_ptr<Material> material) {
            this->r_dist = r_dist;
            this->radius = radius;
            this->material = material;
        }
};

class Capsule: public Shape {
    private:
        Vector3f a;
        Vector3f b;
        float radius;

        float object_sdf(Vector3f p) {
            float t = (p-a).dot(b-a) / (b-a).dot(b-a);
            t = std::clamp<float>(t, 0.0, 1.0);
            Vector3f c = a + t*(b-a);
            return (p-c).norm() - radius;
        }
        
        Vector3f object_normal(Vector3f p) {
            // TODO: INCORRECT!
            return p.normalized();
        }

    public:
        Capsule(Vector3f a, Vector3f b, float radius, std::shared_ptr<Material> material) {
            this->a = a;
            this->b = b;
            this->radius = radius;
            this->material = material;
        }
};

class Box: public Shape {
    private:
        Vector3f dim;

        float object_sdf(Vector3f p) {
            Vector3f q = p.cwiseAbs() - dim;
            return q.cwiseMax(0.0).squaredNorm() + fmin(fmax(q(0), fmax(q(1), q(2))), 0.0);
        }

    public:
        Box(Vector3f dimensions, std::shared_ptr<Material> material) {
            this->dim = dimensions;
            this->material = material;
        }

};


#endif