#include "TuringMachine.h"

std::string TuringMachine::set_tape(const std::string& line) {
    if (line.empty()) return "Ошибка: строка ленты не может быть пустой";
    for (char c : line) {
        if (!alphabet.contains(c)) return "Ошибка: символ '" + std::string(1, c) + "' отсутствует в алфавите ленты";
    }
    std::istringstream ss(line);
    ss >> tape;
    return "";
}

bool TuringMachine::step() {
    Transition transition;
    if (program.get_transition(current_state, tape.get_symbol(), transition) == false) return false; 
    tape.set_symbol(transition.next_symbol);

    if (transition.move_direction == 'R') tape.move_right();
    else if (transition.move_direction == 'L') tape.move_left();

    current_state = transition.next_state;
    
    return true;
}

std::ostream& operator<<(std::ostream& os, const TuringMachine& turing_machine) {
    os << turing_machine.current_state << std::endl;
    for (char c : turing_machine.alphabet) {
        os << c << " ";
    }
    os << std::endl;
    os << turing_machine.program;
    os << turing_machine.tape << std::endl;
    return os;
}

std::istream& operator>>(std::istream& is, TuringMachine& turing_machine) {
    std::string state;
    if (!(is >> state)) return is;
    turing_machine.current_state = state;
    
    std::string alphabet_line;
    is >> std::ws;
    std::getline(is, alphabet_line);
    turing_machine.alphabet.clear();
    for (char c : alphabet_line) {
        if (c != ' ' && c != '\r') turing_machine.alphabet.insert(c);
    }
    if (!(is >> turing_machine.program)) return is;
    if (!(is >> turing_machine.tape)) return is;
    
    return is;
}