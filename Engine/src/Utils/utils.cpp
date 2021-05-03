#include "utils.hpp"

#include <iostream>
#include <algorithm>

namespace Utils
{
    bool hasPrefix(const std::string& str, const std::string& prefix)
    {
        // Check if the current string as the input prefix
        return str.size() >= prefix.size() &&
            str.compare(0, prefix.size(), prefix) == 0;
    }

    bool hasSuffix(const std::string& str, const std::string& suffix)
    {
        // Check if the current string as the input suffix
        return str.size() >= suffix.size() &&
            str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    bool isAnInteger(const std::string& str)
    {
        // Check if every char of the string is a digit
        return std::all_of(str.begin(), str.end(), [](char c)
            {
                return isdigit(c);
            });
    }

    bool isADecimal(const std::string& str)
    {
        // Return true if the string is decimal, check if it has one or less dot and sign, 

        int digitCount = 0, dotCount = 0, signCount = 0;

        // Check if every char of the string is a digit or a mathematical symbol
        for (char c : str)
        {
            if (c == ' ' || c == '\r') continue;

            if (isdigit(c))
            {
                digitCount++;
                continue;
            }

            if (c == '.')
            {
                dotCount++;
                continue;
            }

            if (c == '-' || c == '+')
            {
                signCount++;
                continue;
            }

            return false;
        }

        return digitCount > 0
            && signCount <= 1
            && dotCount <= 1;
    }

    void splitString(const std::string& input, const std::string& delimiter,
        std::string& output, const std::function<void()>& lambda)
    {
        // Loop over the sub-strings, and put them in the output string (to get access it after)
        size_t begin = 0;
        for (size_t end = 0; (end = input.find(delimiter, end)) != std::string::npos; end++)
        {
            output = input.substr(begin, end - begin);

            lambda();

            begin = end + 1;
        }

        output = input.substr(begin);

        lambda();
    }

    std::string getFileNameFromPath(const std::string& filePath)
    {
        return filePath.substr(filePath.find_last_of("/\\") + 1, filePath.size());
    }

    std::string getDirectory(const std::string& filePath)
    {
        std::string fileName = getFileNameFromPath(filePath);

        return filePath.substr(0, filePath.size() - fileName.size());
    }
}