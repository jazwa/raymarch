#ifndef SIMULATION_OPTION_H
#define SIMULATION_OPTION_H

#include <scene.h>
#include <sample_scenes/helix.h>
#include <sample_scenes/moving-torus.h>
#include <sample_scenes/lighting.h>
#include <sample_scenes/material_test.h>


struct SimulationOption {
    const int width = 480;
    const int height = 360;
    const float fov = 90.0;
    const int num_threads = 32;

    enum SceneType {
        MOVING_TORUS,
        DNA_HELIX,
        LIGHTING,
        MATERIAL
    };

    SceneType scene_type = MATERIAL;

    std::unique_ptr<Scene> get_scene() {
        switch(scene_type) {
            //case MOVING_TORUS : return std::make_unique<MovingTorusScene>();
            //case DNA_HELIX    : return std::make_unique<DNAHelixScene>();
            //case LIGHTING     : return std::make_unique<LightingScene>();
            case MATERIAL       : return std::make_unique<MaterialTestScene>();
            
            default : return nullptr;
        }
    }
};

#endif