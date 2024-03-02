#include "utils.hpp"

void   printCharVector(std::vector<char>& vector_char)
{
    for (size_t i = 0; i < vector_char.size(); i++)
        std::cout << vector_char[i];
}

std::vector<char>   stringToCharVector(std::string& str)
{
    std::vector<char> vector_char;
    for (size_t i = 0; i < str.size(); i++)
        vector_char.push_back(str[i]);
    return (vector_char);
}

std::string charVectorToString(std::vector<char>& vector_char)
{
    std::string str;

    for (size_t i = 0; i < vector_char.size(); i++)
        str += vector_char[i];
    return (str);
}