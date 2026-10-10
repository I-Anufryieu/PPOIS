#include "PostMachine.h"

Tape::Tape(){
    tape.push_back(0);
    cur=tape.begin();
}

int Tape::get_symbol() const{
    return *cur;
}

void Tape::set_symbol(int symbol){
    *cur=symbol;
}

void Tape::move_right(){
    if(cur==std::prev(tape.end())){
        tape.push_back(0);
    }
    ++cur;
}

void Tape::move_left(){
    if(cur==tape.begin()){
        tape.push_front(0);
    }
    --cur;
}

Command::Command(int number, char symbol, int fdigit, int sdigit)
            : number(number),symbol(symbol),fdigit(fdigit),sdigit(sdigit){}

Command Command::parser(const std::string& str){
   std::stringstream stream(str);
   char symbol='\0';
   int number = -1,fdigit = -1,sdigit = -1;
   if (!(stream >> number >> symbol)) {
        return Command(-1);
    }
   if(symbol=='V' || symbol=='X' || symbol=='>' || symbol=='<' ) stream>>fdigit;
   else if(symbol=='?') stream>>fdigit>>sdigit;
   else if(symbol=='!'){
    return Command(number,symbol);
   }
   else{
    return Command(-1);
   }
   return Command(number,symbol,fdigit,sdigit);
}