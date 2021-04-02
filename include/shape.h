#ifndef SHAPE_H
#define SHAPE_H
#include <Eigen/Dense>
#include <algorithm>
#include <texture.h>

using namespace Eigen;

class Shape {
    protected:
        Texture texture;
    public:
        virtual float sdf(Vector3f p) = 0;

        virtual ~Shape() {}
        
        Texture get_texture() {
            return texture;
        };

        //virtual void transform(Matrix4f t);
};

class Sphere: public Shape {
    private:
        Vector3f center;
        float radius;
    public:
        Sphere(Vector3f center, float radius, Texture texture) {
            this->center = center;
            this->radius = radius;
            this->texture = texture;
        }

        /* TODO change to support worldspace to reference space */
        float sdf(Vector3f p) {
            return (p - center).norm() - radius;
        }

        Vector3f get_center() {
            return center;
        }
};

class Plane: public Shape {
    private:
        Vector3f point;
        Vector3f normal;

    public:
        Plane(Vector3f point, Vector3f normal, Texture texture) {
            this->point = point;
            this->normal = normal.normalized();
            this->texture = texture;
        }

        /* TODO change to support worldspace to reference space */
        float sdf(Vector3f p) {
            return normal.dot(p - this->point);
        }

};

class Capsule: public Shape {
    private:
        Vector3f a;
        Vector3f b;
        float radius;

    public:
        Capsule(Vector3f a, Vector3f b, float radius, Texture texture) {
            this->a = a;
            this->b = b;
            this->radius = radius;
            this->texture = texture;
        }

        float sdf(Vector3f p) {
            float t = (p-a).dot(b-a) / (b-a).dot(b-a);
            t = std::clamp<float>(t, 0.0, 1.0);
            Vector3f c = a + t*(b-a);
            return (p-c).norm() - radius;
        }

};

#endif