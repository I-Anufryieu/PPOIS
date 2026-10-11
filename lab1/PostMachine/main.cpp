#include "PostMachine.h"

int main(){
    int choice = 0;
    PostMachine post_machine;
    while(true){
        std::cout<<"1 - Задать начальное состояние ленты и позицию каретки\n2 - Написать алгоритм для машины Поста\n3 - Выполнить 1 итерацию алгоритма\n"
               "4 - Начать выполнение алгоритма\n0 - Завершить работу\nВаш выбор: "<<std::endl;
               
        if (!(std::cin >> choice)) { 
            std::cout << "Неверный ввод. Введите неотрицательное число.\n"; 
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch(choice){
             default: std::cout<<"Неверный ввод, попробуйте снова"<<std::endl; break;
             case 1:{
                std::string line;
                while(true){
                    std::cout<<"Введите начальное состояние ленты (вида 10010110): "<<std::endl; 
                    std::cin>>line;
                    bool is_valid=true;
                    for(char c : line){
                        if(c!='0' && c!='1') { is_valid=false; break; }
                    }
                    if(is_valid) break;
                    std::cout<<"Неверный ввод, попробуйте снова"<<std::endl;
                }
                int tape_length=line.length();
                int pos_choice = 0;
                while(true){
                    std::cout<<"Введите начальную позицию каретки (от 0 до "<<tape_length - 1<<"): "<<std::endl;
                    std::string input;
                    std::cin >> input;
                    bool is_number = true;
                    for (char c : input) {
                        if (!isdigit(c)) {
                            is_number = false;
                            break;
                        }
                    }
                    if (!is_number) {
                        std::cout << "Неверный ввод. Нужно ввести целое число. Попробуйте снова." << std::endl;
                        continue;
                    }
                    pos_choice = std::stoi(input);
                    if (pos_choice >= 0 && pos_choice < tape_length) break;
                    std::cout << "Неверная позиция, такой ячейки нет. Попробуйте снова." << std::endl;
                }
                post_machine.set_tape(line, pos_choice);
                std::cout << "Начально состояние ленты и позиция каретки заданы" << std::endl;
                break;
            }
            case 2:{
                post_machine.reset();
                std::cout<<"Введите алгоритм: "<<std::endl;
                while(true){
                    std::string line;
                    getline(std::cin,line);
                    if (line.empty()) continue;
                    Command cmd=Command::parser(line);
                    if(cmd.get_number()==-1){
                        std::cout<<"Неправильная команда, попробуйте снова."<<std::endl;
                        continue;
                    }
                    post_machine.add_command(cmd);
                    if(cmd.get_symbol()=='!'){
                        std::cout << "Окончание написания алгоритма" << std::endl;
                        break;
                    }
                }
                break;
            }
            case 3:{
                try {
                    bool keep_work=post_machine.step();
                    if(!keep_work) std::cout<<"Работа алгоритма завершена"<<std::endl;
                    else std::cout<<post_machine.get_tape();
                }
                catch (const std::out_of_range& e) {
                    std::cout << "Ошибка выполнения: " << e.what() << std::endl;
                }
                    break;
            }   
            case 4:{
                int step=1;
                try {
                    while(true){
                        int cmd_number=post_machine.get_current_line();
                        bool keep_work=post_machine.step();
                        if(!keep_work){ std::cout<<"Работа алгоритма завершена"<<std::endl; break;}
                        else  std::cout<<"Номер итерации: "<<step++<<" Номер команды: "<< cmd_number<<" Лента: "<<post_machine.get_tape()<<std::endl;  
                    }
                } 
                catch (const std::out_of_range& e) {
                    std::cout << "Ошибка выполнения: " << e.what() << std::endl;
                }
                    break;
            }
            case 0: return 0;
        }
    }
}