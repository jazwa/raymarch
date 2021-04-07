
#include "scene.h"
#include "shape.h"
#include <Eigen/Core>

using Eigen::Vector3f;
using std::make_shared;

class LightingScene : public Scene {
    private:
        
        std::shared_ptr<Plane> floor;
        std::shared_ptr<Plane> west_wall;
        std::shared_ptr<Plane> east_wall;
        std::shared_ptr<Plane> north_wall;
        std::shared_ptr<Plane> ceiling;

        std::shared_ptr<Sphere> sphere;
        std::shared_ptr<Torus> torus;
        
        int current_step;

        float torus_frame_rotate;
        float sphere_frame_y_disp;

        std::shared_ptr <PointLight> ceiling_light;
        std::shared_ptr <PointLight> side_light;

    public:
    
        LightingScene() {
            this->scene_time_steps = 64;
            this->current_step = 0;
            this->ambient_light = Vector3f(0.1, 0.1, 0.1);

            Vector3f sphere_start = Vector3f(-1.5, 1.0, 2.2);
            Vector3f sphere_mid = Vector3f(-1.5, -1.0, 2.2);

            torus_frame_rotate = (2.0*M_PI) / ((float) this->scene_time_steps);
            sphere_frame_y_disp = (2.0*(sphere_start(1)-sphere_mid(1))) / ((float) this->scene_time_steps);

            torus = make_shared<Torus>(0.5, 0.15, Texture(220, 100, 200));
            torus->apply_translate(Vector3f(0.5, -0.5, 2.0));
            add_shape(torus);

            sphere = make_shared<Sphere>(sphere_start, 0.5, Texture(100,255,255));
            add_shape(sphere);



            floor = make_shared<Plane>(0.0, 0.0, Texture(150,250,150));
            floor->apply_translate(Vector3f(0.0, -3.0, 0.0));
            add_shape(floor);

            ceiling = make_shared<Plane>(M_PI, 0.0, Texture(150,150,250));
            ceiling->apply_translate(Vector3f(0.0, -3.0, 0.0));
            add_shape(ceiling);

            east_wall = make_shared<Plane>(0.0, M_PI/2.0 , Texture(250,150,150));
            east_wall->apply_translate(Vector3f(0.0, 5.0, 0.0));
            add_shape(east_wall);

            west_wall = make_shared<Plane>(0.0, M_PI/2.0 , Texture(250,150,150));
            west_wall->apply_translate(Vector3f(0.0, -5.0, 0.0));
            add_shape(west_wall);
            

            north_wall = make_shared<Plane>(M_PI/2.0, 0.0, Texture(255,255,150));
            north_wall->apply_translate(Vector3f(0.0, -5.0, 0.0));
            add_shape(north_wall);

            ceiling_light = make_shared<PointLight>(Vector3f(0.0,0.2, 2.0), 10.0, Vector3i(255,255,255));
            add_light(ceiling_light);

            side_light = make_shared<PointLight>(Vector3f(2.0,-1.5, 1.0), 10.0, Vector3i(255,255,255));
            add_light(side_light);

        }

        void step_time() {
            //torus->reset_transform();
            torus->apply_rotate_x(torus_frame_rotate);

            //sphere->reset_transform();
            Vector3f sphere_direction;
            if (current_step >= (int)(this->scene_time_steps/2)) {
                sphere_direction = Vector3f(0.0, 1.0, 0.0);
            } else {
                sphere_direction = Vector3f(0.0, -1.0, 0.0);
            }
            sphere->apply_translate(sphere_direction, sphere_frame_y_disp);

            current_step++;
        }
};
