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
            
            // sample sdf near p, construct a normal from the closest 2 points
            // has to be an even number!
            const int sbox = 16;
            const int sbox2 = sbox*sbox;
            const int sbox3 = sbox*sbox2;
            const float eps = 0.002;
            
            const Vector3f corner_sample = p.array() - ((sbox/2)*eps);

            float fst_closest_dist = 1000.0;
            float snd_closest_dist = 1000.0;
            float sample_distances[sbox3];
            int fst_closest_idx, snd_closest_idx;

            auto indexVec = [eps,&corner_sample](int i) {
                const int xi = i/sbox2;
                const int yi = (i%sbox2)/sbox;
                const int zi = i%sbox;
                return Vector3f(corner_sample + Vector3f(xi, yi, zi)*eps);
            };

            // first pass to get the closest point to surface
            for (int i = 0; i < sbox3; i++) {
                sample_distances[i] = fabs(this->object_sdf(indexVec(i)));

                if (sample_distances[i] < fst_closest_dist) {
                    fst_closest_dist = sample_distances[i];
                    fst_closest_idx = i;
                }
            }

            // second pass to get the second closest point
            for (int i = 0; i < sbox3; i++) {
                if (sample_distances[i] < snd_closest_dist && i != fst_closest_idx) {
                    snd_closest_dist = sample_distances[i];
                    snd_closest_idx = i;
                }
            }

            // get normal (might be negated)
            Vector3f pf = indexVec(fst_closest_idx) - p;
            Vector3f ps = indexVec(snd_closest_idx) - p;
            Vector3f norm = pf.cross(ps).normalized();
            // use sdf to determine orientation of normal
            if (object_sdf(p+(norm*eps)) < object_sdf(p-(norm*eps))) {
                return -norm;
            } else {
                return norm;
            }
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