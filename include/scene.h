#ifndef SCENE_H
#define SCENE_H

/*  Stores shapes in a 3d scene and provides (soon-to-be efficient) 
    accessors to various objects
*/

#include "shape.h"
#include <vector>
#include <Eigen/Core>

using Eigen::Vector3f, Eigen::Vector3i;

class Scene {
    protected:
        // animation variables
        unsigned int scene_time_steps;
        float dt;
        float curr_t;
  
        // internal representation of group of objects, jumping point to 
        // implement spatial binning
        std::vector<std::shared_ptr<Shape>> scene_shapes;
        
    public:
        virtual ~Scene() {}

        const float bounds = 6.0;
        Texture background = Texture(222,222,222);
        
        std::shared_ptr<Shape> nearest_shape(Vector3f p) {
            std::shared_ptr<Shape> ret = nullptr;
            float min_distance = std::numeric_limits<float>::infinity();

            for (auto& s : scene_shapes) {
                float d = s->wdist(p);
                if (d < min_distance) {
                    ret = s;
                    min_distance = d;
                }
            }

            return ret;
        }

        void add_shape(std::shared_ptr<Shape> shape) {
            this->scene_shapes.push_back(shape);
        }

        // continue one step in the animation
        virtual void step_time() = 0;

        int num_steps() {
            return scene_time_steps;
        }

};

#endif