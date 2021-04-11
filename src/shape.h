#ifndef SHAPE_H
#define SHAPE_H

#include <Eigen/Dense>
#include <algorithm>
#include <texture.h>
#include <object3d.h>
#include <cmath>

using namespace Eigen;

class Shape : public Object3D {
    protected:
        Texture texture;
        virtual float object_sdf(Vector3f p) = 0;
        // can either be analytical or approximate, prefer analytical
        virtual Vector3f object_normal(Vector3f p) = 0;

    public:

        // worldspace functions get the distance field and surface normals
        float wdist(Vector3f p) {
            return this->object_sdf(this->inv_affine(p));
        }
        Vector3f wnormal(Vector3f p) {
            return this->object_normal(this->inv_affine(p));
        }

        virtual ~Shape() {}
        
        Texture get_texture() {
            return texture;
        };
};

class Sphere: public Shape {
    private:
        float radius;

        float object_sdf(Vector3f p) {
            return p.norm() - radius;
        }
        
        Vector3f object_normal(Vector3f p) {
            return p.normalized(); // not needed
        }

    public:
        // initialize the sphere with center at worldspace origin
        Sphere(float radius, Texture texture) {
            this->radius = radius;
            this->texture = texture;
        }

        Sphere(Vector3f center, float radius, Texture texture) {
            this->radius = radius;
            this->texture = texture;
            this->apply_translate(center);
        }

};

class Plane: public Shape {
    private:
        Vector3f point;
        Vector3f normal;

        float object_sdf(Vector3f p) {
            return p[1]; // y value
        }

        Vector3f object_normal(Vector3f p) {
            (void) p;
            return Vector3f(0,1,0);
        }

    public:
        // initialize plane on x-z axis
        Plane(Texture texture) {
            this->texture = texture;
        }
        Plane(Vector3f point, Vector3f normal, Texture texture) {

            this->apply_translate(point);
            // TODO check correctness
            float rz = M_PI - atan(normal[2]/normal[1]);
            float rx = M_PI - atan(normal[2]/normal[0]);
            this->apply_rotate_x(rx);
            this->apply_rotate_z(rz);

            this->texture = texture;
        }

        Plane(Vector3f rotation, Texture texture) {
            this->texture = texture;
            this->apply_rotate(rotation(0), 0 , rotation(2));
        }

        Plane(float rx, float rz, Texture texture) {
            this->texture = texture;
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

        Vector3f object_normal(Vector3f p) {
            Vector3f slice_center = Vector3f(p(0), 0, p(2)).normalized() * r_dist;
            return (p-slice_center).normalized();
        }

    public:
        Torus(float r_dist, float radius, Texture texture) {
            this->r_dist = r_dist;
            this->radius = radius;
            this->texture = texture;
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
            (void)p;
            assert(false); // TODO
        }

    public:
        Capsule(Vector3f a, Vector3f b, float radius, Texture texture) {
            this->a = a;
            this->b = b;
            this->radius = radius;
            this->texture = texture;
        }
};


#endif