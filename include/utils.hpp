#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <vector>

void                printCharVector(std::vector<char>& vector_char);
std::vector<char>   stringToCharVector(std::string& str);
std::string         charVectorToString(std::vector<char>& vector_char);

#endif