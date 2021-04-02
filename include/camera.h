#ifndef CAMERA_H
#define CAMERA_H

#include <stdint.h>
#include <Eigen/Dense>
#include <iostream>

#define PI 3.14159265

/* TODO: support arbitrary camera transforms */

class Camera {
    private:
        Vector3f location;
        float frame_width;
        float frame_height;
        float aspect_ratio;
        float fov; // radians

    public:
        Camera(){}

        Camera(Vector3f location, int width, int height, float fov){
            this->location = location;
            this->frame_width = static_cast<float>(width);
            this->frame_height = static_cast<float>(height);
            this->aspect_ratio = frame_width / frame_height;
            this->fov = (fov * PI) / 180.0;
        }

        /* return the direction of a ray in worldspace that corresponds to given screen space values */
        Vector3f pixel_ray(int width_idx, int height_idx) { 
            float screen_x = static_cast<float>(width_idx);
            float screen_y = static_cast<float>(height_idx);
            /* screen space to camera space */
            float cs_x = (2*(screen_x/frame_width) - 1) * tan(fov/2.0) * aspect_ratio;
            float cs_y = (1 - 2*(screen_y/frame_height)) * tan(fov/2.0);

            /* camera space to world space TODO: support transforms */
            return Vector3f(cs_x, cs_y, 1.0).normalized();
        }

        Vector3f origin() {
            return this->location;
        }

};

#endif