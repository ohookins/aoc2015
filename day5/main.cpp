#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <set>
#include <vector>

const std::string INPUT_FILENAME = "input.txt";
const std::set<std::string> CURSED_PAIRS = {"ab", "cd", "pq", "xy"};

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

bool Part1LineIsNice(std::string Line)
{
    char LastChar = '\0';
    int VowelsSeen = 0;
    bool HaveSeenDouble = false;
    bool IsCursed = false;

    for (auto c : Line)
    {
        // Vowel check
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') VowelsSeen++;

        // Double check
        if (LastChar == c) HaveSeenDouble = true;

        // Cursed pair check
        std::string CurrentPair = {LastChar, c};
        if (CURSED_PAIRS.count(CurrentPair) == 1) IsCursed = true;

        LastChar = c;
    }

    return (VowelsSeen >= 3 && HaveSeenDouble && !IsCursed);
}

bool Part2LineIsNice(std::string Line)
{
    // Two sets of 2- and 3-letter groupings - without any further
    // checks on them (yet).
    std::set<std::string> Pairs;
    std::set<std::string> Triples;

    // Extract pairs and triples
    for (int i = 0; i < Line.size(); i++)
    {
        // Extract the next pair of characters
        if ((i+1) < Line.size())
        {
            Pairs.insert(std::string({Line[i], Line[i+1]}));
        }

        // Extract the next triple of characters
        if ((i+2) < Line.size())
        {
            Triples.insert(std::string({Line[i], Line[i+1], Line[i+2]}));
        }
    }

    // Check for triples first as they are easier
    bool HaveSeenValidTriple = false;
    for (auto Triple : Triples)
    {
        // Validity is fairly simple - first and last letters need to match
        // with no further requirements on middle letter.
        if (Triple[0] == Triple[2])
        {
            HaveSeenValidTriple = true;
            break;
        }
    }

    // Check for pairs - this just requires finding the same
    // pair of letters multiple times that are starting at least
    // two letters apart.
    bool HaveSeenValidPair = false;
    for (auto Pair : Pairs)
    {
        size_t SearchPosition = 0;
        std::vector<int> FoundPositions;

        while (true)
        {
            size_t Position = Line.find(Pair, SearchPosition);
            if (Position == std::string::npos) break;

            // Add the found position to the vector for later
            // checking. I could have done this in the first pass
            // of the line :(
            FoundPositions.push_back(Position);

            // Start the next search at the next character within this
            // match in case there are overlaps.
            SearchPosition = Position + 1;

            if (SearchPosition >= Line.size()) break;
        }

        // Could do an exhaustive N^2 search here but it's already fairly
        // awful, and if we have at least two pairs of characters that match
        // we may as well use their positions - they are the most likely to be
        // far apart and meet the criteria. If the first and last are only
        // a single character apart (i.e. overlap) then it's not a nice string
        // anyway. This probably could have been done in the first pass.
        if (FoundPositions.back() >= FoundPositions.front() + 2)
        {
            HaveSeenValidPair = true;
            break;
        }
    }

    return (HaveSeenValidTriple && HaveSeenValidPair);
}

void Part1(const std::vector<std::string>& Input)
{
    int NiceStringCount = 0;

    for (auto Line : Input)
    {
        if (Part1LineIsNice(Line)) NiceStringCount++;
    }

    std::cout << "(Part 1) Input contains " << NiceStringCount << " nice strings" << std::endl;
}

void Part2(const std::vector<std::string>& Input)
{
    int NiceStringCount = 0;

    for (auto Line : Input)
    {
        if (Part2LineIsNice(Line)) NiceStringCount++;
    }

    std::cout << "(Part 2) Input contains " << NiceStringCount << " nice strings" << std::endl;
}

int main(int argc, char** argv)
{
    std::vector<std::string> Input;
    ReadInput([&Input](std::string Line) { Input.push_back(Line); });

    Part1(Input);
    Part2(Input);
}