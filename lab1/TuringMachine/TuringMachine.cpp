#include "TuringMachine.h"

Tape::Tape() {
    tape.push_back(BLANK_SYMBOL);
    cur = tape.begin();
}

Tape& Tape::operator=(const Tape& to_copy){    
    if(this==&to_copy) return *this;
    tape=to_copy.tape;
    int dist=std::distance(to_copy.tape.cbegin(),std::list<char>::const_iterator(to_copy.cur));

    cur = tape.begin();
    std::advance(cur, dist);
    
    return *this;
}

Tape::Tape(const Tape& to_copy) : tape(to_copy.tape){
    int dist=std::distance(to_copy.tape.cbegin(),std::list<char>::const_iterator(to_copy.cur));
    cur=tape.begin();
    std::advance(cur, dist);
}

void Tape::move_right(){
    if(cur==std::prev(tape.end())) tape.push_back('_');
    ++cur;
}

void Tape::move_left(){
    if(cur==tape.begin()) tape.push_front('_');
    --cur;
}

std::ostream& operator<<(std::ostream& os,const Tape& tape){
    for(auto iter=tape.tape.begin();iter!=tape.tape.end();++iter){
        if(iter==tape.cur) os <<'['<< *iter <<']';
        else os << ' '<< *iter <<' ';
    }
    return os;
}

std::istream &operator>>(std::istream& is,Tape& tape){
    tape.tape.clear();
    std::string initial_state;
    is>>initial_state;
    for(char c : initial_state){
        tape.tape.push_back(c);
    }
    tape.cur = tape.tape.begin();
    return is;
}

bool Program::get_transition(const std::string& state,char symbol,Transition& next_transition) const{
    auto find_state = state_table.find({state,symbol});
    if(find_state!=state_table.end()){
        next_transition=find_state->second;
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

void Parser::parse_tape_alphabet(const std::string& line){
    std::istringstream ss(line);
    char symbol;
    while(ss>>symbol){
        tape_alphabet.insert(symbol);
    }
}

void Parser::parse_tape_states(const std::string& line){
    std::istringstream ss(line);
    std::string state;
    while(ss>>state){
        tape_states.insert(state);
    }
}

std::string Parser::parse_and_add_transition(const std::string& rule){
    std::istringstream ss(rule);
    std::string current_state, next_state;
    char read_symbol, write_symbol, move_direction;
    if (!(ss >> current_state >> read_symbol >> write_symbol >> move_direction >> next_state)) return "Ошибка: неверный формат правила";

    if (tape_states.find(current_state) == tape_states.end() || tape_states.find(next_state) == tape_states.end())
        return "Ошибка: одно из состояний не найдено";
    
    if (tape_alphabet.find(read_symbol) == tape_alphabet.end() || tape_alphabet.find(write_symbol) == tape_alphabet.end())
       return "Ошибка: один из символов отсутствует в алфавите";  

    if(move_direction!='R' && move_direction!='L' && move_direction != 'S') return "Ошибка: написано неверное направление (R, L, S)";

    Transition transition{write_symbol, move_direction, next_state};
    program.add_transition(current_state, read_symbol, transition);

    return "";
}

std::ostream& operator<<(std::ostream& os, const Parser& parser) {
    os << "Алфавит ленты: ";
    for (char c : parser.tape_alphabet) {
         os << c << " ";
    }
    os << "\nСостояния: ";
    for (const std::string& state : parser.tape_states) {
         os << state << " ";
      }
      os << "\n" << parser.program;
      return os;
}

std::string TuringMachine::set_tape(const std::string& line) {
    if (line.empty()) return "Ошибка: строка ленты не может быть пустой";
    for (char c : line) {
        if (!alphabet.contains(c)) return "Ошибка: символ '" + std::string(1, c) + "' отсутствует в алфавите ленты";
    }
    std::istringstream ss(line);
    ss >> tape;
    return "";
}

bool TuringMachine::step(){
    Transition transition;
    if(program.get_transition(current_state,tape.get_symbol(),transition) == false) return false; 
    tape.set_symbol(transition.next_symbol);

    if(transition.move_direction=='R') tape.move_right();
    else if(transition.move_direction=='L') tape.move_left();

    current_state=transition.next_state;
    
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