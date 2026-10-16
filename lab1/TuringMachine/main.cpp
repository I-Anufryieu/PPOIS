#ifdef _WIN32
#include <windows.h>
#endif

#include "TuringMachine.h"
namespace{

    int get_menu_choice(){
        int choice = 0;
        while (true) {
            std::cout<<"1 - Задать таблицу переходов\n2 - Задать начальное состояние ленты и позицию каретки\n3 - Выполнить 1 итерацию алгоритма\n"
            "4 - Начать выполнение алгоритма\n0 - Завершить работу\nВаш выбор: "<<std::endl;
            
            if (std::cin >> choice) { 
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return choice;
            }
            std::cout << "Неверный ввод. Введите число.\n"; 
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    void read_alphabet_and_states(Parser& parser) {
        std::cout << "Напишите алфавит машины Тьюринга (через пробел): ";
        std::string input_alphabet;
        getline(std::cin, input_alphabet);
        parser.parse_tape_alphabet(input_alphabet);

        std::cout << "Напишите возможные состояния машины Тьюринга (через пробел): ";
        std::string input_states;
        getline(std::cin, input_states);
        parser.parse_tape_states(input_states);
    }

    void read_transitions(Parser& parser) {
        std::cout << "Задайте правила переходов (вида state symbol new_symbol direction new_state), для остановки введите 0" << std::endl;
        std::string rule;
        while (true) {
            std::getline(std::cin, rule);
            if (rule == "0") break;
            std::string message = parser.parse_and_add_transition(rule);
            if (message.empty()) std::cout << "Правило добавлено.\n";
            else std::cout << message << std::endl;
        }
    }

    std::string read_initial_state(const Parser& parser) {
        std::string initial_state;
        while (true) {
            std::cout << "Введите начальное состояние машины Тьюринга: ";
            getline(std::cin, initial_state);
            if (parser.get_tape_states().contains(initial_state)) return initial_state;
            std::cout << "Ошибка: такого состояния нет в списке. Попробуйте снова.\n";
        }
    }

    void initialize_program(Parser& parser, TuringMachine& turing_machine) {
        read_alphabet_and_states(parser);
        read_transitions(parser);
        std::string initial_state = read_initial_state(parser);

        turing_machine.set_program(parser.get_program());
        turing_machine.set_alphabet(parser.get_tape_alphabet());
        turing_machine.set_initial_state(initial_state);
        std::cout << "Начальное состояние установлено\n";
    }

    std::string read_tape_content(TuringMachine& turing_machine) {
        std::string line;
        while (true) {
            std::cout << "Введите начальное состояние ленты (без пробела): " << std::endl;
            std::cin >> line;
            std::string message = turing_machine.set_tape(line);
            if (message.empty()) return line;
            std::cout << message << ", попробуйте снова.\n";
        }
    }

    int read_carriage_position(int tape_size) {
        int position = 0;
        while (true) {
            std::cout << "Задайте начальную позицию каретки (от 0 до " << tape_size - 1 << "): " << std::endl;
            if (std::cin >> position && position >= 0 && position < tape_size) {
                return position;
            }
            std::cout << "Ошибка, введите корректную позицию.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    void initialize_tape(TuringMachine& turing_machine) {
        if (!turing_machine.is_configured()) {
            std::cout << "Ошибка: сначала выполните пункт 1\n";
            return;
        }
        std::string line = read_tape_content(turing_machine);
        int position = read_carriage_position(static_cast<int>(line.size()));
        turing_machine.set_head_position(position);
        std::cout << "Каретка успешно установлена на позицию " << position << std::endl;
    }

    void run_single_step(TuringMachine& turing_machine) {
        try {
            bool success = turing_machine.step();
            if (!success) {
                std::cout << "Работа алгоритма завершена за неимением правила перехода" << std::endl;
            } else {
                std::cout << "Итерация успешна. Состояние ленты: " << turing_machine.get_tape() << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Ошибка выполнения: " << e.what() << std::endl;
        }
    }

    void run_full_algorithm(TuringMachine& turing_machine) {
        int step = 1;
        std::cout << "Начальное состояние: " << turing_machine.get_current_state()
                  << " Лента: " << turing_machine.get_tape() << std::endl;
        try {
            while (step <= MAX_STEPS) {
                bool success = turing_machine.step();
                if (!success) {
                    std::cout << "Работа алгоритма завершена." << std::endl;
                    break;
                }
                std::cout << "Итерация: " << step++ 
                          << " Состояние: " << turing_machine.get_current_state()
                          << " Лента: " << turing_machine.get_tape() << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Ошибка выполнения: " << e.what() << std::endl;
        }
    }

}  

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    TuringMachine turing_machine;
    Parser parser;

    while (true) {
        switch (get_menu_choice()) {
            default: std::cout << "Неверный ввод, попробуйте снова" << std::endl; break;
            case 1: initialize_program(parser, turing_machine); break;
            case 2: initialize_tape(turing_machine); break;
            case 3:  run_single_step(turing_machine); break;
            case 4: run_full_algorithm(turing_machine); break;
            case 0:  return 0;
        }
    }
}