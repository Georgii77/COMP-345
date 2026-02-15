#include <iostream>
#include <vector>
#include <string>
#include "Map.cpp"

int main(int argc, char* argv[])
{
    MapLoader loader;

    for (int i = 1; i < argc; i++)
    {
        std::string filename = argv[i];

        Map* m = loader.loadMap(filename);

        if (m == nullptr)
        {
            std::cout << filename << " FAILED\n";
            return 0;
        }

        std::cout << filename << " LOADED\n";

        if (m->validate())
        {
            std::cout << filename << " VALID\n";
        }
        
    }

    return 0;
}
