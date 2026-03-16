#include "LoggingObserver.h"
#include <algorithm>

ILoggable::~ILoggable() {}
Observer::~Observer() {}

// Default Constructor
Subject::Subject() {
    observers = new std::vector<Observer*>();
}

// Destructor
Subject::~Subject() {
    delete observers;
}

// Copy Constructor
Subject::Subject(const Subject& other) {
    observers = new std::vector<Observer*>();
}

// Assignment Operator
Subject& Subject::operator=(const Subject& other) {
    return *this;
}

// Attach an observer
void Subject::attach(Observer* observer) {
    observers->push_back(observer);
}

// Detach an observer
void Subject::detach(Observer* observer) {
    auto it = std::find(observers->begin(), observers->end(), observer);
    if (it != observers->end()) {
        observers->erase(it);
    }
}

void Subject::notify(ILoggable* loggable) {
    for (Observer* obs : *observers) {
        obs->update(loggable);
    }
}

// Default Constructor
LogObserver::LogObserver() {
    logFile = new std::ofstream("gamelog.txt", std::ios::app);
}

// Copy Constructor
LogObserver::LogObserver(const LogObserver& other) {
    logFile = new std::ofstream("gamelog.txt", std::ios::app);
}

LogObserver& LogObserver::operator=(const LogObserver& other) {
    if (this != &other) {
        if (logFile->is_open()) {
            logFile->close();
        }
        delete logFile;
        logFile = new std::ofstream("gamelog.txt", std::ios::app);
    }
    return *this;
}

// Destructor
LogObserver::~LogObserver() {
    if (logFile->is_open()) {
        logFile->close();
    }
    delete logFile;
}

// Update
void LogObserver::update(ILoggable* loggable) {
    *logFile << loggable->stringToLog() << std::endl;
}

std::ostream& operator<<(std::ostream& os, const LogObserver& lo) {
    os << "LogObserver writing to gamelog.txt";
    return os;
}
