#include <iostream>
#include <string>
#include "CommandProcessing.h"
#include "GameEngine.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ./program -console OR ./program -file <filename>" << endl;
        return 1;
    }

    GameEngine engine;
    CommandProcessor* processor = nullptr;

    if (string(argv[1]) == "-console") {
        processor = new CommandProcessor();
    } else if (string(argv[1]) == "-file" && argc >= 3) {
        processor = new FileCommandProcessorAdapter(argv[2]);
    } else {
        cout << "Usage: ./program -console OR ./program -file <filename>" << endl;
        return 1;
    }

    while (true) {
        cout << "Current state: " << engine.getCurrentState() << endl;
        Command* cmd = processor->getCommand();
        if (cmd == nullptr) break;

        if (processor->validate(cmd, engine)) {
            engine.processCommand(cmd->getCommand());
            cmd->saveEffect("Command '" + cmd->getCommand() + "' executed successfully.");
        }
        cout << *cmd << endl;
    }

    delete processor;
    return 0;
}