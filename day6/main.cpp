#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <regex>
#include <bitset>

const std::string INPUT_FILENAME = "input.txt";

void ReadInput(std::function<void(std::string)> function)
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

struct Instruction
{
    std::string Command;
    int FromX;
    int FromY;
    int ToX;
    int ToY;
};

Instruction ParseLine(std::string Line)
{
    std::regex InstructionRE("^(turn on|turn off|toggle) (\\d+),(\\d+) through (\\d+),(\\d+)$");
    std::smatch Match;
    std::regex_match(Line, Match, InstructionRE);

    Instruction instruction;

    instruction.Command = Match[1].str();
    instruction.FromX = std::stoi(Match[2].str());
    instruction.FromY = std::stoi(Match[3].str());
    instruction.ToX = std::stoi(Match[4].str());
    instruction.ToY = std::stoi(Match[5].str());

    return instruction;
}

void Part1(const std::vector<Instruction>& Instructions)
{
    // Not as wasteful on memory as a million bools, but
    // a sparse matrix would be better if I knew how to make it work.
    std::bitset<1000000> Lights;

    for (auto instruction : Instructions)
    {
        // Standard n^2 loop. There's probably a much better way.
        for (int y = instruction.FromY; y <= instruction.ToY; y++)
        {
            for (int x = instruction.FromX; x <= instruction.ToX; x++)
            {
                if (instruction.Command == "turn on")
                {
                    Lights.set(y*1000+x);
                    continue;
                }

                if (instruction.Command == "turn off")
                {
                    Lights.reset(y*1000+x);
                    continue;
                }

                // assume this is toggle
                Lights.flip(y*1000+x);
            }
        }
    }

    std::cout << "(Part 1) Total of switched on lights is " << Lights.count() << std::endl;
}

void Part2(const std::vector<Instruction>& Instructions)
{
    // Sadly very wasteful on memory
    uint8_t Lights[1000000] = {};

    for (auto instruction : Instructions)
    {
        // Standard n^2 loop. There's probably a much better way.
        for (int y = instruction.FromY; y <= instruction.ToY; y++)
        {
            for (int x = instruction.FromX; x <= instruction.ToX; x++)
            {
                if (instruction.Command == "turn on")
                {
                    Lights[y*1000+x]++;
                    continue;
                }

                if (instruction.Command == "turn off")
                {
                    if (Lights[y*1000+x] > 0) Lights[y*1000+x]--;
                    continue;
                }

                // assume this is toggle
                Lights[y*1000+x] += (uint8_t)2;
            }
        }
    }

    // count the "brightness" values
    int sum = 0;
    for (auto light : Lights)
    {
        sum += light;
    }

    std::cout << "(Part 2) Total brightness is " << sum << std::endl;
}

int main(int argc, char** argv)
{
    std::vector<Instruction> Instructions;
    ReadInput([&Instructions](std::string Line) { Instructions.push_back(ParseLine(Line)); });

    Part1(Instructions);
    Part2(Instructions);
}