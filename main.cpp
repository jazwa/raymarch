#include <iostream>
#include <Eigen/Dense>
#include <simulation.h>
#include <fstream>
#include <chrono>

using namespace Eigen;
using namespace std;

int main(void) {

    Simulation sim;
    cout << "Simulation info: " << sim.width  << ", " << sim.height << endl;

    for (int step = 0; step < sim.num_frames; step++) {

        cout << "rendering: " << step+1 << " out of " << sim.num_frames << endl;
        /* render frame */
        sim.render_step();

        /* write to ppm file */
        int* frame_buffer = sim.get_current_frame();
        
        ofstream ofs("first.ppm", ios_base::out | ios_base::binary);
        ofs << "P6" << endl << sim.width << ' ' << sim.height << endl << "255" << endl;
        
        /* TODO parallelize this */
        for (int j = 0; j < sim.height; j++) {
            for (int i = 0; i < sim.width; i++) {
                int idx = 3*(j*sim.width + i);
                
                ofs << (char) frame_buffer[idx] 
                    << (char) frame_buffer[idx+1]
                    << (char) frame_buffer[idx+2];
            }
        }
        ofs.close();

    }

}