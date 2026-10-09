#include <list>
#include <iterator>

class Tape{
    
private:
    std::list<int> tape;
    std::list<int>::iterator cur;

public:
    Tape();
    int get_symbol() const;
    void set_symbol(int val);
    void move_right();
    void move_left();
    
};