#include "world.hpp"
#include <string>

int main() {
//    logger.Logger.initialize("default_log.txt")
    std::string map_file = "sample_maps/4walls_1table_1ball_nested.json";
    std::string vis_filename = "output_vis.json";
    rosa::World world(map_file, vis_filename);
    world.run();
    //logger.Logger.finalize()
    return 0;
}