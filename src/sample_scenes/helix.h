
#include "scene.h"
#include "shape.h"
#include <iostream>
#include <map>

using std::make_shared;

class DNA_helix_scene : public Scene {
    private:
        std::string dna_seq = "ggggaacccgcatgaaggtccgagtgagggcatgaacaagtcctactccacggtacggacattggctctgaacgcggaggtcatgtgttgttatgggggc";
        std::map<char, char> comp_base = {
            {'a', 't'},
            {'t', 'a'},
            {'c', 'g'},
            {'g', 'c'}
        };
        std::map<char, Texture> base_color = {
            {'a', Texture(0, 127, 255)}, // adenine -> azure
            {'t', Texture(253, 245, 1)}, // thymine -> tweety bird
            {'c', Texture(20, 253, 20)}, // guanine -> green
            {'g', Texture(150, 10, 24)}  // cytosine -> carmine
        };
        // this might not be the right name
        Texture pentose_color = Texture(170, 120, 230);

    public:
    
        DNA_helix_scene() {
            this->curr_t = 0.0;
            this->dt = 0.0;
            this->scene_time_steps = 1;

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
