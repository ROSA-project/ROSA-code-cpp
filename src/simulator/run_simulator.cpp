#include "common/arg_parser.hpp"
#include "world.hpp"
#include <string>

int main(int argc, char *argv[]) {
    ArgumentParser arg_parser(argc, argv);
    
    std::string default_map = "sample_maps/4walls_1table_1ball_nested.json";
    std::string map_file = arg_parser.getArgument<std::string>("-m", default_map);

    std::string vis_file = arg_parser.getArgument<std::string>("-v", "output_vis.json");

    // logger.Logger.initialize("default_log.txt")
    rosa::World world(map_file, vis_file);
    world.run();
    // logger.Logger.finalize()
    return 0;
}