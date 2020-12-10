#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>

const std::string INPUT_FILENAME = "input.txt";

void readInput(void (*function)(std::string))
{
    std::ifstream ifs;
    ifs.open(INPUT_FILENAME);

    if (ifs.fail())
    {
        std::cout << std::filesystem::current_path() << std::endl;
        throw("Failed to open input file");
    }

    while (!ifs.eof())
    {
        std::string Line;
        std::getline(ifs, Line);
        function(Line);
    }
}

void countParentheses(std::string Line)
{
    int Balance = 0;
    bool haveEnteredBasement = false;
    for (int i = 0; i < Line.size(); i++)
    {
        if (Line[i] == '(') Balance++;
        if (Line[i] == ')') Balance--;

        // check to see if we've entered the basement for the first time
        if (!haveEnteredBasement && Balance == -1)
        {
            haveEnteredBasement = true;
            // positions are indexed from 1 in the file
            std::cout << "Santa first entered the basement at position " << i + 1 << std::endl;
        }
    }

    std::cout << "Santa is going to floor " << Balance << std::endl;
}

int main(int argc, char** argv)
{
    readInput(&countParentheses);
}