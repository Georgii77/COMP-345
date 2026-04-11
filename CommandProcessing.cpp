#include "CommandProcessing.h"

#include <cctype>
#include <sstream>

#include "GameEngine.h"

using namespace std;

static string trim(const string& value){
    size_t start = 0;

    while (start < value.size() && isspace(static_cast<unsigned char>(value[start]))) {
        ++start;
    }

    size_t end = value.size();

    while (end > start && isspace(static_cast<unsigned char>(value[end - 1]))) {
        --end;
    }

    return value.substr(start, end - start);
}

Command::Command() {
    command = "";
    effect = "";
}

Command::Command(const string& commandStr) {
    command = commandStr;
    effect = "";
}

Command::Command(const Command& other) {
    command = other.command;
    effect = other.effect;
}

Command& Command::operator=(const Command& other) {
    if (this != &other) {
        command = other.command;
        effect = other.effect;
    }
    return *this;
}

Command::~Command() {
    // No manual deletion needed
}

void Command::saveEffect(const string& effectStr) {
    effect = effectStr;
    notify(this);
}

string Command::stringToLog() {
    return "Command: " + command + " | Effect: " + effect;
}

string Command::getCommand() const {
    return command;
}

string Command::getEffect() const {
    return effect;
}

ostream& operator<<(ostream& out, const Command& commandObj) {
    out << "Command: " << commandObj.getCommand() << " | Effect: " << commandObj.getEffect();
    return out;
}

CommandProcessor::CommandProcessor() {
    commands = new vector<Command*>();
}

CommandProcessor::CommandProcessor(const CommandProcessor& other) {
    commands = new vector<Command*>();
    for (Command* command : *other.commands) {
        commands->push_back(new Command(*command));
    }
}

CommandProcessor& CommandProcessor::operator=(const CommandProcessor& other) {
    if (this != &other) {
        for (Command* command : *commands) {
            delete command;
        }
        commands->clear();

        for (Command* command : *other.commands) {
            commands->push_back(new Command(*command));
        }
    }
    return *this;
}

CommandProcessor::~CommandProcessor() {
    for (Command* command : *commands) {
        delete command;
    }
    delete commands;
}

string CommandProcessor::readCommand() {
    string commandStr;
    getline(cin, commandStr);
    return trim(commandStr);
}

Command* CommandProcessor::saveCommand(const string& commandStr) {
    Command* command = new Command(commandStr);
    commands->push_back(command);
    notify(this);
    return command;
}

string CommandProcessor::stringToLog() {
    return "Command saved: " + commands->back()->getCommand();
}

Command* CommandProcessor::getCommand() {
    while (true) {
        string commandStr = readCommand();

        if (!cin && commandStr.empty()) {
            return nullptr;
        }

        if (commandStr.empty()) {
            if (cin.eof()) {
                return nullptr;
            }
            continue;
        }

        return saveCommand(commandStr);
    }
}

bool CommandProcessor::validate(Command* command, const GameEngine& gameEngine) const {
    if (command == nullptr) {
        return false;
    }

    std::istringstream iss(command->getCommand());
    std::string commandName;
    iss >> commandName;

    if (!gameEngine.isValidTransition(commandName)) {
        std::cout << "Invalid command '" << command->getCommand()
                  << "' for state '" << gameEngine.getCurrentState() << "'" << std::endl;

        command->saveEffect("ERROR: command '" + command->getCommand()
            + "' is invalid in state '" + gameEngine.getCurrentState() + "'");
        return false;
    }

    return true;
}


const vector<Command*>* CommandProcessor::getCommands() const {
    return commands;
}

ostream& operator<<(ostream& out, const CommandProcessor& commandProcessor) {
    out << "Command history:" << endl;
    for (Command* command : *commandProcessor.getCommands()) {
        out << "  " << *command << endl;
    }
    return out;
}

FileLineReader::FileLineReader() {
    filename = "";
    inputFile = ifstream();
}

FileLineReader::FileLineReader(const string& file) {
    filename = file;
    inputFile.open(file);
}

FileLineReader::FileLineReader(const FileLineReader& other) {
    filename = other.filename;
    inputFile.open(filename);
}

FileLineReader& FileLineReader::operator=(const FileLineReader& other) {
    if (this != &other) {
        if (inputFile.is_open()) {
            inputFile.close();
        }
        filename = other.filename;
        inputFile.open(filename);
    }
    return *this;
}

FileLineReader::~FileLineReader() {
    if (inputFile.is_open()) {
        inputFile.close();
    }
    // No manual deletion needed
}

string FileLineReader::readLineFromFile() {
    if (!inputFile.is_open()) {
        return "";
    }

    string line;
    if (!getline(inputFile, line)) {
        return "";
    }

    return trim(line);
}

string FileLineReader::getFilename() const {
    return filename;
}

ostream& operator<<(ostream& out, const FileLineReader& fileLineReader) {
    out << "FileLineReader(" << fileLineReader.getFilename() << ")";
    return out;
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter() {
    fileLineReader = new FileLineReader();
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const string& filename) {
    fileLineReader = new FileLineReader(filename);
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other)
    : CommandProcessor(other) {
    fileLineReader = new FileLineReader(*other.fileLineReader);
}

FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter& other) {
    if (this != &other) {
        CommandProcessor::operator=(other);
        delete fileLineReader;
        fileLineReader = new FileLineReader(*other.fileLineReader);
    }
    return *this;
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete fileLineReader;
}

string FileCommandProcessorAdapter::readCommand() {
    return fileLineReader->readLineFromFile();
}

ostream& operator<<(ostream& out, const FileCommandProcessorAdapter& adapter) {
    out << "FileCommandProcessorAdapter using " << *adapter.fileLineReader;
    return out;
}


// Returns true if the command is well-formed and all constraints are met
bool CommandProcessor::parseTournamentCommand(const string& cmdStr, TournamentParams& params) {
    params.maps.clear();
    params.strategies.clear();
    params.numGames = 0;
    params.maxTurns = 0;

    istringstream iss(cmdStr);
    string token;
    iss >> token; // consume "tournament"

    while (iss >> token) {
        if (token == "-M") {
            string mapsStr;
            if (!(iss >> mapsStr)) {
                cout << "Tournament: missing argument after -M.\n";
                return false;
            }
            istringstream ms(mapsStr);
            string entry;
            while (getline(ms, entry, ',')) {
                // trim
                size_t s = entry.find_first_not_of(" \t");
                size_t e = entry.find_last_not_of(" \t");
                if (s != string::npos)
                    params.maps.push_back(entry.substr(s, e - s + 1));
            }
        } else if (token == "-P") {
            string stratStr;
            if (!(iss >> stratStr)) {
                cout << "Tournament: missing argument after -P.\n";
                return false;
            }
            istringstream ss(stratStr);
            string entry;
            while (getline(ss, entry, ',')) {
                size_t s = entry.find_first_not_of(" \t");
                size_t e = entry.find_last_not_of(" \t");
                if (s != string::npos)
                    params.strategies.push_back(entry.substr(s, e - s + 1));
            }
        } else if (token == "-G") {
            if (!(iss >> params.numGames)) {
                cout << "Tournament: missing or invalid argument after -G.\n";
                return false;
            }
        } else if (token == "-D") {
            if (!(iss >> params.maxTurns)) {
                cout << "Tournament: missing or invalid argument after -D.\n";
                return false;
            }
        } else {
            cout << "Tournament: unknown flag '" << token << "'.\n";
            return false;
        }
    }

    // check number of maps is between 1 and 5
    if (params.maps.empty() || params.maps.size() > 5) {
        cout << "Tournament: -M must specify 1–5 map filenames (got "
             << params.maps.size() << ").\n";
        return false;
    }

    // check number of strategies is between 2 and 4
    if (params.strategies.size() < 2 || params.strategies.size() > 4) {
        cout << "Tournament: -P must specify 2–4 strategies (got "
             << params.strategies.size() << ").\n";
        return false;
    }

    const vector<string> validStrategies = {
        "Aggressive", "Benevolent", "Neutral", "Cheater"
    };
    for (const string& strat : params.strategies) {
        bool found = false;
        for (const string& valid : validStrategies) {
            if (strat == valid) { found = true; break; }
        }
        if (!found) {
            cout << "Tournament: invalid strategy '" << strat
                 << "'. Valid options: Aggressive, Benevolent, Neutral, Cheater.\n";
            return false;
        }
    }
    // check that number of games is between 1 and 5
    if (params.numGames < 1 || params.numGames > 5) {
        cout << "Tournament: -G must be between 1 and 5 (got "
             << params.numGames << ").\n";
        return false;
    }
    // check that number of turns is between 10 and 50
    if (params.maxTurns < 10 || params.maxTurns > 50) {
        cout << "Tournament: -D must be between 10 and 50 (got "
             << params.maxTurns << ").\n";
        return false;
    }

    return true;
}