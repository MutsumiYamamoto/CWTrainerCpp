#pragma once

#include <string>
#include <unordered_map>

class MorseDecoder
{
public:
    MorseDecoder();

    std::string Decode(
        const std::string& code);

private:
    std::unordered_map<
        std::string,
        std::string> table;
};