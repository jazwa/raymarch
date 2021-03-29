
#include <Eigen/Dense>
#include <algorithm>

using namespace Eigen;

class Shape {
    public:
        virtual float sdf(Vector3f p) = 0;
        virtual Vector3d get_color() = 0;
        virtual ~Shape() {}
        //virtual void transform(Matrix4f t);
};

class Sphere: public Shape {
    private:
        Vector3f center;
        float radius;
        Vector3d color;
    public:
        Sphere(Vector3f center, float radius, Vector3d color) {
            this->center = center;
            this->radius = radius;
            this->color = color;
        }

        /* TODO change to support worldspace to reference space */
        float sdf(Vector3f p) {
            return (p - center).norm() - radius;
        }

        Vector3d get_color() {
            return color;
        }

        Vector3f get_center() {
            return center;
        }
};

class Plane: public Shape {
    private:
        Vector3f point;
        Vector3f normal;
        Vector3d color;
        
    public:
        Plane(Vector3f point, Vector3f normal, Vector3d color) {
            this->point = point;
            this->normal = normal.normalized();
            this->color = color;
        }

        /* TODO change to support worldspace to reference space */
        float sdf(Vector3f p) {
            return normal.dot(p - this->point);
        }

        Vector3d get_color() {
            return color;
        }
};

class Capsule: public Shape {
    private:
        Vector3f a;
        Vector3f b;
        float radius;
        Vector3d color;

    public:
        Capsule(Vector3f a, Vector3f b, float radius, Vector3d color) {
            this->a = a;
            this->b = b;
            this->radius = radius;
            this->color = color;
        }

        float sdf(Vector3f p) {
            float t = (p-a).dot(b-a) / (b-a).dot(b-a);
            t = std::clamp<float>(t, 0.0, 1.0);
            Vector3f c = a + t*(b-a);
            return (p-c).norm() - radius;
        }

        Vector3d get_color() {
            return this->color;
        }
};

/* class Box: public Shape {
    private 
} */