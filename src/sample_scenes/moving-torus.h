#ifndef MOVING_TORUS_SCENE_H
#define MOVING_TORUS_SCENE_H
/* 
#include "scene.h"
#include "shape.h"
#include <Eigen/Core>

using Eigen::Vector3f;
using std::make_shared;

class MovingTorusScene : public Scene {
    private:        
        std::shared_ptr<Torus> tor;
        Vector3f start;
        Vector3f end;

    public:
    
        Moving_torus_scene() {
            this->scene_time_steps = 32;

            start = Vector3f(-1.5, -1.5, 2.0);
            end = Vector3f(0.0, 0.1, 2.5);

            this->dt = (start-end).norm()/((float)this->num_steps());
            this->curr_t = 0.0;

            tor = make_shared<Torus>(0.5, 0.15, Texture(128, 30, 128));
            tor->apply_translate(start);
            add_shape(tor);
        }

        void step_time() {
            tor->reset_transform();
            float t = this->curr_t;
            Vector3f current_pos = start + t*(end-start);
            tor->apply_translate(current_pos);
            tor->apply_rotate_x((t*M_PI)/4.0);
            tor->apply_rotate_y((t*M_PI)/4.0);
            
            this->curr_t += this->dt;
        }
};
 */
#endif