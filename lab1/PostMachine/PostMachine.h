#include <list>
#include <iterator>
#include <sstream>
#include <string>
#include <map>

class Tape{
private:
    std::list<int> tape;
    std::list<int>::iterator cur;

public:
    Tape();
    Tape(const Tape &to_copy);
    void set_tape(const std::string& initial_state, int start_pos = 0);
    int get_symbol() const;
    void set_symbol(int val);
    void move_right();
    void move_left();

    Tape& operator=(const Tape& to_copy);
    friend std::ostream& operator<<(std::ostream& os,const Tape& tape);
    friend std::istream& operator>>(std::istream& is,Tape& tape);
    bool operator==(const Tape& other) const{
        return (this->tape == other.tape) && 
               (std::distance(this->tape.cbegin(), std::list<int>::const_iterator(this->cur)) == 
                std::distance(other.tape.cbegin(), std::list<int>::const_iterator(other.cur)));
    }
    bool operator!=(const Tape& other) const{
        return !(*this==other);
    }
};

class Command{
private:
    int number;
    char symbol;
    int fdigit;
    int sdigit;
public:
    Command(int number = -1, char symbol = '!', int fdigit = -1, int sdigit = -1)
        : number(number), symbol(symbol), fdigit(fdigit), sdigit(sdigit) {}
    static Command parser(const std::string& line);
    int get_number() const { return number; }
    char get_symbol() const { return symbol;}
    int get_fdigit() const {return fdigit;}
    int get_sdigit() const { return sdigit;} 
};

class PostMachine{
private:
    std::map<int, Command> command; 
    Tape tape;
    int current_line = 1;
public:
    void add_command(const Command& cmd){
    command[cmd.get_number()]=cmd;
    }
    int get_current_line() const{ return current_line; }
    const Tape& get_tape(){ return tape;}
};
