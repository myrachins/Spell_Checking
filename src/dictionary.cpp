#include <iostream>
#include <fstream>
#include <string>
#include "dictionary.h"


Dictionary::Dictionary(string filename) : HashSet()
{
    std::ifstream in(filename);

    if(!in.is_open())
        throw std::invalid_argument("File can't be open");

    std::string line;
    while (std::getline(in, line))
        insert(line);

    in.close();
}
