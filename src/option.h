#ifndef OPTION_H
#define OPTION_H

#include <scene.h>
#include <sample_scenes/helix.h>
#include <sample_scenes/moving-torus.h>
#include <sample_scenes/lighting.h>

struct Option {
    const int width = 480;
    const int height = 360;
    const float fov = 90.0;

    bool lighting = false;

    enum SceneType {
        MOVING_TORUS,
        DNA_HELIX,
        LIGHTING
    };

    SceneType scene_type = MOVING_TORUS;

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