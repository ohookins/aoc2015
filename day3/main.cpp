#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <set>

const std::string INPUT_FILENAME = "input.txt";

std::string Directions;
std::set<std::pair<int, int>> DeliveryMap;

void ReadInput(void (*function)(std::string))
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

void MoveCharacter(char Code, int &X, int &Y)
{
    switch (Code)
        {
        case '^':
            Y += 1;
            break;
        
        case 'v':
            Y -= 1;
            break;

        case '<':
            X -= 1;
            break;

        case '>':
            X += 1;
            break;
    }
}

void DeliverPresents()
{
    // relative coordinates to starting location
    int SantaX = 0, SantaY = 0;

    // deliver first present
    DeliveryMap.emplace(std::pair(0, 0));
    for (auto Char : Directions)
    {
        MoveCharacter(Char, SantaX, SantaY);
        DeliveryMap.emplace(std::pair(SantaX, SantaY));
    }
    std::cout << "Santa has delivered to " << DeliveryMap.size() << " separate houses." << std::endl;

    // Now deliver by both Santa and Robo-Santa
    SantaX = 0, SantaY = 0;
    int RoboX = 0, RoboY = 0;
    DeliveryMap.clear();

    // deliver first presents
    DeliveryMap.emplace(std::pair(0, 0));
    for (int i = 0; i < Directions.size(); i++)
    {
        // Santa delivers on each even instruction.
        if (i % 2 == 0)
        {
            MoveCharacter(Directions[i], SantaX, SantaY);
            DeliveryMap.emplace(std::pair(SantaX, SantaY));
        // Robo-Santa delivers on each odd instruction.
        } else {
            MoveCharacter(Directions[i], RoboX, RoboY);
            DeliveryMap.emplace(std::pair(RoboX, RoboY));
        }
    }

    std::cout << "Santa and Robo-Santa have delivered to " << DeliveryMap.size() << " separate houses." << std::endl;
}

int main(int argc, char** argv)
{
    // super simple input handling as there's only a single line of input
    ReadInput([](std::string Line) { Directions = Line; });
    DeliverPresents();
}