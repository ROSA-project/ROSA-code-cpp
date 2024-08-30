#include "common/arg_parser.hpp"
#include "common/logger.hpp"
#include "world.hpp"
#include <string>

int main(int argc, char* argv[]) {
    set_logger();
    ArgumentParser arg_parser(argc, argv);

    std::string default_map = "sample_maps/4walls_1table_1ball_nested.json";
    std::string map_file = arg_parser.getArgument<std::string>("-m", default_map);
    std::string vis_file_prefix = arg_parser.getArgument<std::string>("-v", "output_vis");

    rosa::WorldConfig w_config;
    w_config.simDurationMsec = 10 * 1000; // 10 secs
    w_config.visFrameIntervalMsec = 250;  // 25 msec
    rosa::World world(map_file, vis_file_prefix, w_config);
    world.run();
    return 0;
}