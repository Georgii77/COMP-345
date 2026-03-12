#include "CommandProcessing.h"

#include <cctype>
#include <sstream>

#include "GameEngine.h"

using namespace std;
static string_trim(const string& value){
    size_t_start = 0;

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
    command = new string("");
    effect = new string("");
}

Command::Command(const string& commandStr) {
    command = new string(commandStr);
    effect = new string("");
}

Command::Command(const Command& other) {
    command = new string(*other.command);
    effect = new string(*other.effect);
}

Command& Command::operator=(const Command& other) {
    if (this != &other) {
        *command = *other.command;
        *effect = *other.effect;
    }
    return *this;
}

Command::~Command() {
    delete command;
    delete effect;
}

void Command::saveEffect(const string& effectStr) {
    *effect = effectStr;
}

string Command::getCommand() const {
    return *command;
}

string Command::getEffect() const {
    return *effect;
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
    return command;
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
    return gameEngine.isValidCommand(command->getCommand());
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
    filename = new string("");
    inputFile = new ifstream();
}

FileLineReader::FileLineReader(const string& file) {
    filename = new string(file);
    inputFile = new ifstream(file.c_str());
}

FileLineReader::FileLineReader(const FileLineReader& other) {
    filename = new string(*other.filename);
    inputFile = new ifstream(filename->c_str());
}

FileLineReader& FileLineReader::operator=(const FileLineReader& other) {
    if (this != &other) {
        if (inputFile->is_open()) {
            inputFile->close();
        }
        *filename = *other.filename;
        inputFile->open(filename->c_str());
    }
    return *this;
}

FileLineReader::~FileLineReader() {
    if (inputFile->is_open()) {
        inputFile->close();
    }
    delete inputFile;
    delete filename;
}

string FileLineReader::readLineFromFile() {
    if (!inputFile->is_open()) {
        return "";
    }

    string line;
    if (!getline(*inputFile, line)) {
        return "";
    }

    return trim(line);
}

string FileLineReader::getFilename() const {
    return *filename;
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