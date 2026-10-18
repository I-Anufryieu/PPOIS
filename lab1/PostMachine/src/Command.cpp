#include "Command.h"

Command::Command(int number, char symbol, int fdigit, int sdigit)
    : number(number), symbol(symbol), fdigit(fdigit), sdigit(sdigit) {}

int Command::get_number() const { return number; }
char Command::get_symbol() const { return symbol; }
int Command::get_fdigit() const { return fdigit; }
int Command::get_sdigit() const { return sdigit; }

bool Command::operator==(const Command& other) const {
    return number == other.number && 
           symbol == other.symbol && 
           fdigit == other.fdigit && 
           sdigit == other.sdigit;
}

bool Command::operator!=(const Command& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Command& cmd) {
    char symbol = cmd.get_symbol();
    os << cmd.get_number() << " " << symbol;
    
    if (symbol == Command::SET1 || symbol == Command::SET0 || 
        symbol == Command::RIGHT || symbol == Command::LEFT) {
        os << " " << cmd.get_fdigit();
    } 
    else if (symbol == Command::BRANCH) {
        os << " " << cmd.get_fdigit() << " " << cmd.get_sdigit();
    }
    
    return os;
}

std::istream& operator>>(std::istream& is, Command& cmd) {
    int number = Command::INVALID_NUM;
    int fdigit = Command::INVALID_NUM;
    int sdigit = Command::INVALID_NUM;
    char symbol = Command::STOP;

    if (!(is >> number >> symbol)) {
        cmd = Command(Command::INVALID_NUM);
        return is;
    }
    if (symbol == Command::SET1 || symbol == Command::SET0 || symbol == Command::RIGHT || symbol == Command::LEFT) is >> fdigit;
    else if (symbol == Command::BRANCH)  is >> fdigit >> sdigit;
    else if (symbol != Command::STOP) {
        cmd = Command(Command::INVALID_NUM);
        is.setstate(std::ios::failbit);
        return is;
    }
    cmd = Command(number, symbol, fdigit, sdigit);
    return is;
}