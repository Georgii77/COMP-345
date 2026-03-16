#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "GameEngine.h"
#include "LoggingObserver.h"

using namespace std;

class Command : public Subject, public ILoggable {

    public:
        Command();
        Command(const string &commandStr);
        Command(const Command &c);
        Command& operator=(const Command &c);
        ~Command();

        void saveEffect(const string &effectStr);
        string getCommand() const;
        string getEffect() const;
        string stringToLog() override;

        friend ostream& operator<<(ostream &out, const Command  &command);

    private:
        string command;
        string effect;
};

class CommandProcessor : public Subject, public ILoggable {

    public:
        CommandProcessor();
        CommandProcessor(const CommandProcessor &cp);
        CommandProcessor& operator=(const CommandProcessor &cp);
        virtual ~CommandProcessor();

        Command* getCommand();
        virtual bool validate(Command *command, const GameEngine &ge) const;
        const vector<Command*>* getCommands() const;
        string stringToLog() override;

        friend ostream& operator<<(ostream& out, const CommandProcessor& cp);
        
    protected:
        virtual string readCommand();
        Command* saveCommand(const string &commandStr);

    private:
        vector<Command*>* commands;

};

class FileLineReader {
public:
    FileLineReader();
    FileLineReader(const string& filename);
    FileLineReader(const FileLineReader& other);
    FileLineReader& operator=(const FileLineReader& other);
    ~FileLineReader();

    string readLineFromFile();
    string getFilename() const;

    friend ostream& operator<<(ostream& out, const FileLineReader& fileLineReader);

private:
    string filename;
    ifstream inputFile;
};

class FileCommandProcessorAdapter : public CommandProcessor {
public:
    FileCommandProcessorAdapter();
    FileCommandProcessorAdapter(const string& filename);
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other);
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other);
    ~FileCommandProcessorAdapter() override;

    friend ostream& operator<<(ostream& out, const FileCommandProcessorAdapter& adapter);

protected:
    string readCommand() override;

private:
    FileLineReader* fileLineReader;
};


#endif