#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <map>
#include <regex>

const std::string INPUT_FILENAME = "input.txt";

std::vector<std::string> ReadInput()
{
    std::vector<std::string> Input;
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
        Input.push_back(Line);
    }

    return Input;
}

struct Relation{};

struct CopyRelation : public Relation
{
    Node* Source;
};

struct NotRelation : public Relation
{
    Node* Source;
};

struct BoolRelation : public Relation
{
    std::string Operation;
    Node* LHS;
    Node* RHS;
};

struct ShiftRelation : public Relation
{
    std::string Direction;
    int Count;
};

struct Node
{
    std::string Name;
    int Value;
    Relation In;
};

std::optional<Node*> CreateNumericNode(std::string NodeName)
{
    // check if the source is a number and if so, just create
    // a node not in the map for it
    try
    {
        int Value = std::stoi(NodeName);
        Node* NumericNode = new Node();
        NumericNode->Value = Value;
        return NumericNode;
    }
    catch (...)
    {
        // do nothing
        return {};
    }
}

void CreateCopyRelation(std::string SourceName, std::string OutputName, std::map<std::string, Node*>& NodeMap, bool Invert = false)
{
    // Assume the destination node doesn't exist and add it to the node map.
    auto DestNode = new Node({Name: OutputName});
    NodeMap[OutputName] = DestNode;

    // check if the source node exists, and if not, create it
    // and put it in the map for later
    Node* SourceNode;
    if (NodeMap.count(SourceName) == 1)
    {
        SourceNode = NodeMap[SourceName];
    }
    else
    {
        // if we don't have a node in the nodemap, this could be a
        // bare numeric value, in which case try to convert it to a number
        // first
        SourceNode = CreateNumericNode(SourceName).value_or(new Node({Name: SourceName}));

        // Only add it to the nodemap if it wasn't a numeric node.
        // This didn't quite work out as nicely as I thought.
        if (SourceNode->Name != "") NodeMap[SourceNode->Name] = SourceNode;
    }

    // Finally, set up the relationship to the source node in the current node.    
    DestNode->In = CopyRelation{Source: SourceNode};

    // add a copy relation to the current node referencing either the node from the
    // map or the newly created one.
    // We abuse this function to allow for inverting the input.
    if (Invert)
    {
        DestNode->In = NotRelation{Source: SourceNode};    
    }
    else
    {
        DestNode->In = CopyRelation{Source: SourceNode};
    }
}

void CreateComplexRelation(std::string Param1, std::string Op, std::string Param2, std::string OutputName, std::map<std::string, Node*>& NodeMap)
{
    // Assume the destination node doesn't exist and add it to the node map.
    auto DestNode = new Node({Name: OutputName});
    NodeMap[OutputName] = DestNode;
}

std::map<std::string, Node*> ParseRelations(std::vector<std::string> Input)
{
    std::map<std::string, Node*> NodeMap;

    std::regex CopyRE("^(\\w+) -> (\\w+)$");
    std::regex InvertRE("^NOT (\\w+) -> (\\w+)$");
    std::regex OperationRE("^(\\w+) (\\w+) (\\w+) -> (\\w+)$");

    for (auto Line : Input)
    {
        std::smatch Match;

        // Copy operation, we'll assume we haven't yet seen the RHS
        // of this relation and just create it.
        if (std::regex_match(Line, Match, CopyRE))
        {
            CreateCopyRelation(Match[1].str(), Match[2].str(), NodeMap);
            continue;
        }

        // invert - logically the same as copy but with an inversion of the source node.
        if (std::regex_match(Line, Match, InvertRE))
        {
            CreateCopyRelation(Match[1].str(), Match[2].str(), NodeMap, true);
            continue;
        }

        // some kind of more complex operation with three parameters
        if (std::regex_match(Line, Match, OperationRE))
        {
            CreateComplexRelation(Match[1].str(), Match[2].str(), Match[3].str(), Match[4].str(), NodeMap);
            continue;
        }
    }

    return NodeMap;
}

int main(int argc, char** argv)
{
    std::vector<std::string> Input = ReadInput();

    // name to pointer mapping for nodes, from which we can
    // enter the graph at any point.
    std::map<std::string, Node*> NodeMap = ParseRelations(Input);
}