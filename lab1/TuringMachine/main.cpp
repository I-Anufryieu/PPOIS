#ifdef _WIN32
#include <windows.h>
#endif

#include "TuringMachine.h"

int main(){

    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    #endif


    int choice = 0;
    TuringMachine turing_machine;
    Parser parser;

    while(true){
        std::cout<<"1 - Задать таблицу переходов\n2 - Задать начальное состояние ленты и позицию каретки\n3 - Выполнить 1 итерацию алгоритма\n"
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
                std::cout<<"Напишите алфавит машины Тьюринга (через пробел): ";
                std::string input_alphabet;
                getline(std::cin,input_alphabet);
                std::cout<<"Напишите возможные состояния машины Тьюринга (через пробел): ";
                std::string input_states;
                getline(std::cin,input_states);

                parser.parse_tape_alphabet(input_alphabet);
                parser.parse_tape_states(input_states);

                std::cout << "Доступный алфавит: ";
                for (char c : parser.get_tape_alphabet()) {
                    std::cout << c << " ";
                }
                std::cout << "\nДоступные состояния: ";
                for (const std::string state : parser.get_tape_states()) {
                    std::cout << state << " ";
                }
                std::cout<<"\nЗадайте правила переходов(вида state symbol new_symbol direction new_state), для остановки ввода введите 0"<<std::endl;
                std::string rule;

                while(true){
                    std::getline(std::cin, rule);
                    if (rule == "0") break;
                    std::string message = parser.parse_and_add_transition(rule);
                    if(message.empty()) std::cout << "Правило добавлено. Вводите следующее :\n";
                    else std::cout << message<<std::endl;
                }

                std::string initial_state;
                while (true) {
                    std::cout << "Введите начальное состояние машины Тьюринга: ";
                    getline(std::cin, initial_state);
                    const auto& states = parser.get_tape_states();
                    if (states.contains(initial_state)) break;
                    std::cout << "Ошибка: такого состояния нет в списке возможных состояний. Попробуйте снова.\n";
                }

                turing_machine.set_program(parser.get_program());
                turing_machine.set_alphabet(parser.get_tape_alphabet());
                turing_machine.set_initial_state(initial_state);
                std::cout << "Начальное состояние установлено\n";
                break;
             }
             case 2:{
                if (!turing_machine.is_configured()) {std::cout << "Ошибка: сначала выполните пункт 1\n"; break; }
                std::string line;
                while (true) {
                    std::cout<<"Введите начальное состояние ленты(без пробела): "<<std::endl; 
                    std::cin>>line;
                    std::string message = turing_machine.set_tape(line);
                    if(message.empty()){ std::cout << "Лента успешно задана\n"; break;}
                    std::cout << message << ", попробуйте снова.\n";
                }
                    int tape_size=line.size();
                    std::cout<<"Задайте начальную позицию каретки(от 0 до " << tape_size - 1<<" )"<<std::endl;
                    int choice;
                    while(true){
                        if(std::cin>>choice){
                            if(choice >= 0 && choice <= tape_size - 1){
                                turing_machine.set_head_position(choice);
                                std::cout << "Каретка успешно установлена на позицию " << choice<<std::endl;
                                break;
                            }
                            else std::cout << "Ошибка, Введите позицию от 0 до " << tape_size - 1 << ": ";
                        }
                        else{
                            std::cout << "Ошибка! Введите целое число: ";
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }
                    }
                break;
             }
            case 3:{
                try{
                    bool success=turing_machine.step();
                    if(!success) std::cout<<"Работа алгоритма завершена за неимением правила перехода"<<std::endl;
                    else std::cout<<"Итерация успешна. Состояние ленты: "<< turing_machine.get_tape() <<std::endl;
                }
                catch (const std::exception& e) {
                std::cout << "Ошибка выполнения: " << e.what() << std::endl;
                }
                break; 
            }
            case 4:{
                int step=0;
                std::cout << "Начальное состояние: " << turing_machine.get_current_state()
                          << " Лента: " << turing_machine.get_tape() << std::endl;
                try{
                    while (step < MAX_STEPS) {
                        bool success = turing_machine.step();
                        if (!success){std::cout << "Работа алгоритма завершена."<<std::endl; break;}
                        std::cout << "Итерация: " << step++ << " Состояние: " << turing_machine.get_current_state()
                         << " Лента: " << turing_machine.get_tape() << std::endl;
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Ошибка выполнения: " << e.what() << std::endl;
                }
                break;
            }
            case 0: return 0;
        }
    }
}