#pragma once

#include <unordered_map>
#include <cstring> //std::strlen
#include <string>
#include <type_traits>
#include <sstream> // std::stringstream

class ArgumentParser {
private:
    std::unordered_map<std::string, std::string> options_;

public:
    ArgumentParser(int argc, char* const argv[]) {
        int i = 0;
        while (i < argc) {
            if (argv[i][0] == '-') {
                if (i < argc - 1 && argv[i + 1][0] != '-') {
                    options_[std::string(argv[i])] = std::string(argv[i + 1]);
                    i++;
                } else options_[std::string(argv[i])] = std::string("");
            } else {
                options_[std::string(argv[i])] = std::string("");
            }
            i++;
        }
    }

    bool hasOption(const std::string &option) const {
        return options_.find(option) != options_.end();
    }

    template <typename T> 
    T getArgument(const std::string &option, const T& default_value) {
        if (!hasOption(option)) {
            return default_value;
        }
        std::stringstream tmp(options_.at(option));
        T value;        
        tmp >> value;
        return value;

    }
};