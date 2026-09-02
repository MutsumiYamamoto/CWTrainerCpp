#include "MorseDecoder.h"

MorseDecoder::MorseDecoder()
{
    table[".-"] = "A";
    table["-..."] = "B";
    table["-.-."] = "C";
    table["-.."] = "D";
    table["."] = "E";
    table["..-."] = "F";
    table["--."] = "G";
    table["...."] = "H";
    table[".."] = "I";
    table[".---"] = "J";
    table["-.-"] = "K";
    table[".-.."] = "L";
    table["--"] = "M";
    table["-."] = "N";
    table["---"] = "O";
    table[".--."] = "P";
    table["--.-"] = "Q";
    table[".-."] = "R";
    table["..."] = "S";
    table["-"] = "T";
    table["..-"] = "U";
    table["...-"] = "V";
    table[".--"] = "W";
    table["-..-"] = "X";
    table["-.--"] = "Y";
    table["--.."] = "Z";
}

std::string MorseDecoder::Decode(
    const std::string& code)
{
    auto it = table.find(code);

    if (it != table.end())
        return it->second;

    return "?";
}