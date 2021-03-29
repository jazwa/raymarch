#include <iostream>
#include <Eigen/Dense>
#include <simulation.h>
#include <fstream>
#include <chrono>

using namespace Eigen;

using std::cout, std::endl, std::ofstream, std::ios_base;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

int main(void) {

    Simulation sim;
    cout << "Simulation info: " << sim.width  << ", " << sim.height << endl;

    for (int step = 0; step < sim.num_frames; step++) {
        cout << "Rendering: " << step+1 << " out of " << sim.num_frames << endl;

        auto start_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        
        /* render frame */
        sim.render_step();
        
        auto end_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        cout << "Time: "<< end_time - start_time << "msec" << endl;

        /* write to ppm file */
        int* frame_buffer = sim.get_current_frame();

        std::string filename = "assets/img" + std::to_string(step) + ".ppm";
        ofstream ofs(filename, ios_base::out | ios_base::binary);
        ofs << "P6" << endl << sim.width << ' ' << sim.height << endl << "255" << endl;

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