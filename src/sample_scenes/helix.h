#ifndef DNA_HELIX_SCENE_H
#define DNA_HELIX_SCENE_H

#include "scene.h"
#include "shape.h"
#include "materials/solid_material.h"
#include <camera.h>
#include <iostream>
#include <map>

using std::make_shared;

class DNAHelixScene : public Scene {
    private:

        const int width = 480;
        const int height = 360;
        const float fov = 90.0;

        std::string dna_seq = "ggggaacccgcatgaaggtccgagtgagggcatgaacaagtcctactccacggtacggacattggctctgaacgcggaggtcatgtgttgttatgggggc";
        std::map<char, char> comp_base = {
            {'a', 't'},
            {'t', 'a'},
            {'c', 'g'},
            {'g', 'c'}
        };
        std::map<char, std::shared_ptr<SolidMaterial> > base_color = {
            {'a', make_shared<SolidMaterial>(0, 127, 255)}, // adenine -> azure
            {'t', make_shared<SolidMaterial>(253, 245, 1)}, // thymine -> tweety bird
            {'c', make_shared<SolidMaterial>(20, 253, 20)}, // guanine -> green
            {'g', make_shared<SolidMaterial>(150, 10, 24)}  // cytosine -> carmine
        };
        // this might not be the right name
        std::shared_ptr<SolidMaterial> pentose_color = make_shared<SolidMaterial>(170, 120, 230);

    public:
    
        DNAHelixScene() {

            this->curr_t = 0.0;
            this->dt = 0.0;
            this->scene_time_steps = 1;
            this->background = make_shared<SolidMaterial>(222,222,222);

            this->cam = Camera(Vector3f(0,0,0), this->width, this->height, this->fov);

            for (int y_idx = 0; y_idx <= 50; y_idx++) {
                float y = -2.5 + 0.1*((float)y_idx);
                float cir_rad = 0.115;
                float snd_point_bias = 0.62;
                float z_bias = 3.0;
                float cyl_rad = 0.03;
                Vector3f pointA, pointB, midpoint;

                pointA << 0.5*sin(4.0*y),
                          y,
                          z_bias + 0.5*cos(4.0*y);
                
                pointB << 0.5*sin(4.0*(y+snd_point_bias)),
                          y,
                          z_bias + 0.5*cos(4.0*(y+snd_point_bias));
                
                midpoint = pointA + 0.5*(pointB - pointA);

                char base_letter = dna_seq[y_idx];
                char comp_letter = comp_base[base_letter];

                this->add_shape(make_shared<Sphere>(pointA, cir_rad, pentose_color));
                this->add_shape(make_shared<Sphere>(pointB, cir_rad, pentose_color));
                this->add_shape(make_shared<Capsule>(pointA, midpoint, cyl_rad, base_color[base_letter]));
                this->add_shape(make_shared<Capsule>(midpoint, pointB, cyl_rad, base_color[comp_letter]));
            }
        }

        void step_time() {}
};

#endif