#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cctype> //We included this for isdigit and isalpha
#include "Automaton.hpp"
#include "State.hpp"

//Function to read the file content
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string content, line;

    if (!file.is_open()) {
        std::cerr << "Could not open the file - '" << filename << "'" << std::endl;
        return "";
    }

    while (std::getline(file, line)) {
        content += line + " ";
    }

    file.close();
    return content;
}

//Function to map state names to token types
std::string getTokenType(State* state) {
    static std::unordered_map<std::string, std::string> tokenMap = {
        {"Integer", "Integer"},
        {"Float", "Float"},
        {"Variable", "Variable"},
        {"Sum", "Sum"},
        {"Subtract", "Subtract"},
        {"Product", "Product"},
        {"Division", "Division"},
        {"Assignment", "Assignment"},
        {"LeftParenthesis", "Left Parenthesis"},
        {"RightParenthesis", "Right Parenthesis"}
    };

    auto it = tokenMap.find(state->name);
    if (it != tokenMap.end()) {
        return it->second;
    }
    return "Rejected";
}

//Function to process a token using the automaton
void processToken(Automaton* automaton, std::string& token) {
    if (!token.empty()) {
        bool result = automaton->validate(token);
        if (result) {
            State* finalState = automaton->S;
            for (char c : token) {
                std::string symbol(1, c);
                finalState = finalState->getNextState(symbol);
            }
            std::cout << token << " -> " << getTokenType(finalState) << std::endl;
        } else {
            std::cout << token << " -> Rejected" << std::endl;
        }
        token = "";
    }
}

//The lexer function which reads a file and processes the content
void lexer(const std::string& filepath) {
    //Create the states
    State* InitialState = new State("InitialState", false, false);
    State* Integer = new State("Integer", true, false);
    State* _Float = new State("Float", true, false);
    State* Variable = new State("Variable", true, false);
    State* Sum = new State("Sum", true, false);
    State* Subtract = new State("Subtract", true, false);
    State* Product = new State("Product", true, false);
    State* Division = new State("Division", true, false);
    State* Assignment = new State("Assignment", true, false);
    State* LeftParenthesis = new State("LeftParenthesis", true, false);
    State* RightParenthesis = new State("RightParenthesis", true, false);
    State* DeadState = new State("DeadState", false, true);

    //Transitions for integers
    for (char i = '0'; i <= '9'; i++) {
        std::string digit(1, i);
        InitialState->addTransition(digit, Integer);
        Integer->addTransition(digit, Integer);
    }

    //Transitions for variables
    for (char i = 'a'; i <= 'z'; i++) {
        std::string letter(1, i);
        InitialState->addTransition(letter, Variable);
        Variable->addTransition(letter, Variable);

        for (char j = '0'; j <= '9'; j++) {
            std::string digit(1, j);
            Variable->addTransition(digit, Variable);
        }
    }

    //Transitions for floats
    Integer->addTransition(".", _Float);
    for (char i = '0'; i <= '9'; i++) {
        std::string digit(1, i);
        _Float->addTransition(digit, _Float);
    }

    //Transitions for the other symbols
    InitialState->addTransition("+", Sum);
    InitialState->addTransition("-", Subtract);
    InitialState->addTransition("*", Product);
    InitialState->addTransition("/", Division);
    InitialState->addTransition("=", Assignment);
    InitialState->addTransition("(", LeftParenthesis);
    InitialState->addTransition(")", RightParenthesis);

    //Read the content of the file
    std::string content = readFile(filepath);

    if (content.empty()) {
        std::cerr << "File is empty or could not be read." << std::endl;
        delete InitialState;
        delete Integer;
        delete _Float;
        delete Variable;
        delete Sum;
        delete Subtract;
        delete Product;
        delete Division;
        delete Assignment;
        delete LeftParenthesis;
        delete RightParenthesis;
        return;
    }

    //Create the automaton with the initial state
    Automaton* automaton = new Automaton(InitialState);

    //Process the content and tokenize
    std::string token = "";
    for (char ch : content) {
        if (std::isspace(ch)) {
            processToken(automaton, token);
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '(' || ch == ')') {
            processToken(automaton, token);
            std::string symbol(1, ch);
            processToken(automaton, symbol);
        } else {
            token += ch;
        }
    }
    processToken(automaton, token);

    //Cleanup memory
    delete InitialState;
    delete Integer;
    delete _Float;
    delete Variable;
    delete Sum;
    delete Subtract;
    delete Product;
    delete Division;
    delete Assignment;
    delete LeftParenthesis;
    delete RightParenthesis;
    delete automaton;
}

int main() {
    std::string filepath = "expressions.txt";
    lexer(filepath);
    return 0;
}
