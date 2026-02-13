#include <iostream>
#include <vector>
#include <string>
#include "Map.cpp"

int main(int argc, char* argv[]) {
    MapLoader loader;

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <map1> <map2> ...\n";
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        std::string filename = argv[i];

        std::cout << "----------------------------------\n";
        std::cout << "Loading: " << filename << "\n";

        Map* m = loader.loadMap(filename);

        if (m == nullptr) {
            std::cout << "Result: REJECTED (invalid or unreadable map)\n";
            continue;
        }

        std::cout << "Result: ACCEPTED (valid map)\n";

        // Demonstrate validations
        if (m->validate()) {
            std::cout << "validate(): PASSED\n";

        }
        else {
            std::cout << "validate(): FAILED \n";
        }

        delete m;
    }

    return 0;
}