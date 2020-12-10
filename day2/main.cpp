#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <regex>

const std::string INPUT_FILENAME = "input.txt";

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

int TotalWrappingPaper = 0;
int TotalRibbon = 0;

void TallyWrappingPaper(std::string Line)
{
    std::regex DimensionsRE("^(\\d+)x(\\d+)x(\\d+)$");
    std::cmatch Match;
    bool DidMatch = std::regex_match(Line.c_str(), Match, DimensionsRE);

    if (!DidMatch)
    {
        std::cout << "BAD: " << Line << std::endl;
        return;
    }

    int L = std::stoi(Match[1].str());
    int W = std::stoi(Match[2].str());
    int H = std::stoi(Match[3].str());

    int MinArea = std::min({L*W, W*H, H*L});
    int MinPerimeter = std::min({2*L+2*W, 2*W+2*H, 2*H+2*L});

    int WrappingNeeded = 2*L*W + 2*W*H + 2*H*L + MinArea;

    TotalWrappingPaper += WrappingNeeded;
    TotalRibbon += (MinPerimeter + L*W*H);
}

int main(int argc, char** argv)
{
    ReadInput(&TallyWrappingPaper);
    std::cout << "The elves will need " << TotalWrappingPaper << " sqft of wrapping paper and " << TotalRibbon << " ft of ribbon." << std::endl;
}