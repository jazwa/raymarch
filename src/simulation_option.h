#ifndef SIMULATION_OPTION_H
#define SIMULATION_OPTION_H

#include <scene.h>
#include <sample_scenes/helix.h>
#include <sample_scenes/moving-torus.h>
#include <sample_scenes/lighting.h>


struct SimulationOption {
    const int width = 480;
    const int height = 360;
    const float fov = 90.0;
    const int num_threads = 32;

    enum SceneType {
        MOVING_TORUS,
        DNA_HELIX,
        LIGHTING
    };

    SceneType scene_type = LIGHTING;

    std::unique_ptr<Scene> get_scene() {
        switch(scene_type) {
            case MOVING_TORUS : return std::make_unique<Moving_torus_scene>();
            case DNA_HELIX    : return std::make_unique<DNA_helix_scene>();
            case LIGHTING     : return std::make_unique<LightingScene>();
            
            default : return nullptr;
        }
    }
};

#endif