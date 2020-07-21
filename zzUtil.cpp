//
// Created by lorenzo on 7/20/20.
//

#include "zzUtil.hpp"

std::vector<std::string> zzUtil::SplitString(std::string str, char delim) {
    std::string buffer;
    std::stringstream ss(str);
    std::vector<std::string> tokens;

    while(getline(ss, buffer, delim)) tokens.push_back(buffer);

    return tokens;
}