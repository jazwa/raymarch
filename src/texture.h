#ifndef TEXTURE_H
#define TEXTURE_H

/* Wrapper class for different types of of textures, right now, 
   all textures are just solid colors, with a canonical 
   representation of a 3 vector of integer rgb values 
*/

#include <Eigen/Core>

using Eigen::Vector3i;

class Texture {
    private:
        Vector3i color;

    public:
        float albedo = 0.18;
        
        Texture(int r, int g, int b) {
            color = Vector3i(r,g,b);
        }
        Texture(Vector3i& cvec) {
            color = cvec;
        }
        Texture() {
            color = Vector3i(0,0,0); // black
        }

        Vector3i get_pixel_color() {
            return color;
        }
};

#endif