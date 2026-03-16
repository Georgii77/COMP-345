#ifndef LOGGINGOBSERVER_H
#define LOGGINGOBSERVER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class ILoggable {
public:
    virtual ~ILoggable();
    virtual std::string stringToLog() = 0;
};

class Observer {
public:
    virtual ~Observer();
    virtual void update(ILoggable* loggable) = 0;
};

class Subject {
private:
    std::vector<Observer*>* observers;
public:
    Subject();
    virtual ~Subject();
    Subject(const Subject& other);
    Subject& operator=(const Subject& other);

    void attach(Observer* observer);
    void detach(Observer* observer);
    void notify(ILoggable* loggable);
};

class LogObserver : public Observer {
private:
    std::ofstream* logFile;
public:
    LogObserver();
    LogObserver(const LogObserver& other);
    LogObserver& operator=(const LogObserver& other);
    ~LogObserver();

    void update(ILoggable* loggable) override;
    friend std::ostream& operator<<(std::ostream& os, const LogObserver& lo);
};

#endif
