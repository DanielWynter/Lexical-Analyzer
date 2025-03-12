#ifndef AUTOMATON_HPP
#define AUTOMATON_HPP

#include "State.hpp"
#include <iostream>
#include <string>

class Automaton {
public:
    State* S;

    Automaton(State* S) : S(S) {}

    bool validate(const std::string& word) {
        if (word.empty()) {
            return S->isFinal;
        }

        State* currentState = S;
        bool result = false;

        for (char ch : word) {
            std::string symbol(1, ch);
            currentState = currentState->getNextState(symbol);

            if (currentState == nullptr) {
                return false;
            }

            result = currentState->isFinal;
        }

        return result;
    }
};

#endif
