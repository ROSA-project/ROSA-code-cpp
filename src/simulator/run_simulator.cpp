#include "common/arg_parser.hpp"
#include "common/logger.hpp"
#include "world.hpp"
#include <string>

int main(int argc, char *argv[]) {
    set_logger();
    ArgumentParser arg_parser(argc, argv);
    LOG_TRACE("log with source info"); // Console: "[source example.cpp] [function source_info_example] [line 10] log with source info"
    LOG_INFO("log with source info"); // Console: "[source example.cpp] [function source_info_example] [line 10] log with source info"
    LOG_ERROR("log with source info"); // Console: "[source example.cpp] [function source_info_example] [line 10] log with source info"

    std::string default_map = "sample_maps/4walls_1table_1ball_nested.json";
    std::string map_file = arg_parser.getArgument<std::string>("-m", default_map);
    std::string vis_file = arg_parser.getArgument<std::string>("-v", "output_vis.json");

    rosa::World world(map_file, vis_file);
    world.run();
    return 0;
}