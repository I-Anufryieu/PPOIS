#include "PostMachine.h"

void PostMachine::add_command(const Command& cmd){
    auto unique_key = command.insert({cmd.get_number(), cmd});
    if (!unique_key.second) {
        throw std::invalid_argument("Ошибка: Команда с номером " + std::to_string(cmd.get_number()) + " уже существует!");
    }
}

bool PostMachine::step(){
    auto cmd_iterator = command.find(current_line);
    if(cmd_iterator == command.end()){
        throw std::out_of_range("Ошибка: Команды с номером " + std::to_string(current_line) + " не существует!");
    }
    const Command& cmd = cmd_iterator->second;
    char symbol = cmd.get_symbol();

    if(symbol == Command::STOP) return false;
    else if(symbol == Command::BRANCH){
        current_line = (tape.get_symbol() == 0) ? cmd.get_fdigit() : cmd.get_sdigit();
        return true;
    }
    else if (symbol == Command::SET1)  tape.set_symbol(1);
    else if (symbol == Command::SET0)  tape.set_symbol(0);
    else if (symbol == Command::RIGHT)  tape.move_right();
    else if (symbol == Command::LEFT)   tape.move_left();

    current_line = cmd.get_fdigit();
    
    return true;
}