#include "../include/Program.h"

bool Program::get_transition(const std::string& state, char symbol, Transition& next_transition) const {
    auto find_state = state_table.find({state, symbol});
    if (find_state != state_table.end()) {
        next_transition = find_state->second;
        return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const Program& program) {
    for (const auto& [key, transition] : program.state_table) {
        os << key.first << " " << key.second << " " 
           << transition.next_symbol << " " << transition.move_direction << " " 
           << transition.next_state << std::endl;
    }
    return os;
}

std::istream& operator>>(std::istream& is, Program& program) {
    program.state_table.clear();
    std::string state, next_state;
    char symbol, next_symbol, move_dir;
    while (is >> state >> symbol >> next_symbol >> move_dir >> next_state) {
        program.state_table[{state, symbol}] = Transition{next_symbol, move_dir, next_state};
    }
    return is;
}