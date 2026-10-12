#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

std::string fileToString(const std::string &filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);
    if (!fileStream.is_open()) {
        std::cerr << "File " << filePath << " does not exist." << std::endl;
        return "";
    }
    std::stringstream sstr;
    sstr << fileStream.rdbuf();
    content = sstr.str();
    fileStream.close();
    return content;
}