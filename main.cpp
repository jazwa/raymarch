#include <iostream>
#include <Eigen/Dense>
#include <simulation.h>
#include <fstream>
#include <chrono>
#include "lodepng.h"

using namespace Eigen;

using std::cout, std::endl, std::ofstream, std::ios_base, std::string;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

inline std::string render_loading_string(int curr, int total);

int main(void) {

    Simulation sim;
    cout << "Frame size: " << sim.width  << ", " << sim.height << endl;
    cout << "Num frames: " << sim.scene->num_steps() << endl;
    cout << "Starting render...🚀 " << endl;

    auto start_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    for (int step = 0; step < (sim.scene)->num_steps(); step++) {
        
        cout << render_loading_string(step+1, sim.scene->num_steps()) << "\r" << std::flush;

        // render frame
        sim.render_step();

        /* write to ppm file */
        int* frame_buffer = sim.get_current_frame();

        std::string filename = "img" + std::to_string(step) + ".ppm";
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
    
    auto end_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    cout << "\n🛎️  Done! Total Time: "<< end_time - start_time << "ms" << endl;

    return 0;
}


inline std::string render_loading_string(int curr, int total) {
    
    const int loading_bar_len = 64;
    float percent_done = static_cast<float>(curr) / static_cast<float>(total);
    const int num_done = static_cast<int>(percent_done*loading_bar_len);


    return "(Frame: " + std::to_string(curr) + "/" + std::to_string(total) + ") " 
            + '[' + string(num_done, '#') + ">" + string(loading_bar_len-num_done, '.') + ']' + string(8, ' ');
}