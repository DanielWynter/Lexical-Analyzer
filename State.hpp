#ifndef STATE_HPP
#define STATE_HPP

#include <iostream>
#include <unordered_map>
#include <string>

class State {
public:
    std::string name;
    bool isFinal;
    bool isDeathState;
    std::unordered_map<std::string, State*> transitions;

    State(std::string name, bool isFinal, bool isDeathState) 
        : name(name), isFinal(isFinal), isDeathState(isDeathState) {}

    void addTransition(const std::string& symbol, State* state) {
        transitions[symbol] = state;
    }

    State* getNextState(const std::string& symbol) {
        if (transitions.find(symbol) != transitions.end()) {
            return transitions[symbol];
        }
        return nullptr;
    }

    std::string toString() const {
        return name + "," + (isFinal ? "true" : "false") + "," + (isDeathState ? "true" : "false");
    }
};

#endif