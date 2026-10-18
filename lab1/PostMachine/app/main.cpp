#include "PostMachine.h"
#include <limits>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {

    int get_menu_choice(){
        int choice = 0;
            while (true) {
                std::cout << "1 - Задать начальное состояние ленты и позицию каретки\n"
                        << "2 - Написать алгоритм для машины Поста\n"
                        << "3 - Выполнить 1 итерацию алгоритма\n"
                        << "4 - Начать выполнение алгоритма\n"
                        << "0 - Завершить работу\nВаш выбор: ";
                        
                if (std::cin >> choice) { 
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    return choice;
                } 
                
                std::cout << "Неверный ввод. Введите число.\n"; 
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

    std::string read_tape_string() {
        std::string line;
        while (true) {
            std::cout << "Введите начальное состояние ленты (вида 10010110): " << std::endl; 
            std::cin >> line;
            bool is_valid = true;
            for (char c : line) {
                if (c != '0' && c != '1') { is_valid = false; break; }
            }
            if (is_valid) return line;
            std::cout << "Неверный ввод, попробуйте снова" << std::endl;
        }
    }

    int read_carriage_position(int tape_length) {
        while (true) {
            std::cout << "Введите начальную позицию каретки (от 0 до " << tape_length - 1 << "): " << std::endl;
            std::string input;
            std::cin >> input;
            
            bool is_number = true;
            for (char c : input) {
                if (!isdigit(c)) { is_number = false; break; }
            }
            if (!is_number) {
                std::cout << "Неверный ввод. Нужно ввести целое число. Попробуйте снова." << std::endl;
                continue;
            }
            
            int pos = std::stoi(input);
            if (pos >= 0 && pos < tape_length) return pos;
            std::cout << "Неверная позиция, такой ячейки нет. Попробуйте снова." << std::endl;
        }
    }

    void initialize_tape(PostMachine& post_machine) {
        std::string line = read_tape_string();
        int pos_choice = read_carriage_position(line.length());
        
        post_machine.set_tape(line, pos_choice);
        std::cout << "Начальное состояние ленты и позиция каретки заданы" << std::endl;
    }

    bool process_command_line(PostMachine& post_machine) {
        std::string line;
        if (!std::getline(std::cin, line) || line.empty()) return true;

        Command cmd;
        if (!(std::istringstream(line) >> cmd) || cmd.get_number() == Command::INVALID_NUM) {
            std::cout << "Неправильная команда, попробуйте снова." << std::endl;
            return true;
        }

        try { post_machine.add_command(cmd); }
        catch (const std::invalid_argument& e) {
            std::cout << e.what() << " Попробуйте ввести другой номер." << std::endl;
            return true;
        }

        if (cmd.get_symbol() == Command::STOP) {
            std::cout << "Окончание написания алгоритма" << std::endl;
            return false;
        }
        return true; 
    }

    void write_algorithm(PostMachine& post_machine) {
        post_machine.reset();
        std::cout << "Введите алгоритм: " << std::endl;
        while (process_command_line(post_machine));
    }

    void run_single_step(PostMachine& post_machine) {
        try {
            bool keep_work = post_machine.step();
            if (!keep_work) std::cout << "Работа алгоритма завершена" << std::endl;
            else std::cout << post_machine.get_tape() << std::endl;
        } 
        catch (const std::out_of_range& e) { std::cout << "Ошибка выполнения: " << e.what() << std::endl;}
    }

    void run_full_algorithm(PostMachine& post_machine) {
        int step = 1;
        try {
            while (true) {
                int cmd_number = post_machine.get_current_line();
                bool keep_work = post_machine.step();
                if (!keep_work) { 
                    std::cout << "Работа алгоритма завершена" << std::endl; 
                    break;
                } else {
                    std::cout << "Номер итерации: " << step++ 
                                << " Номер команды: " << cmd_number 
                                << " Лента: " << post_machine.get_tape() << std::endl;  
                }
            }
        } 
        catch (const std::out_of_range& e) { 
            std::cout << "Ошибка выполнения: " << e.what() << std::endl; 
        }
    }

}

int main(){
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    #endif
    PostMachine post_machine;
    while(true){
        switch(get_menu_choice()){
            default: std::cout<<"Неверный ввод, попробуйте снова"<<std::endl; break;
            case 1: initialize_tape(post_machine); break;
            case 2: write_algorithm(post_machine); break;
            case 3: run_single_step(post_machine); break;
            case 4: run_full_algorithm(post_machine); break;
            case 0: return 0;
        }
    }
}