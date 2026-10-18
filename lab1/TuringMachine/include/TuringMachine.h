#ifndef TURINGMACHINE_H
#define TURINGMACHINE_H

#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include "Tape.h"
#include "Program.h"
#include "Transition.h"

/**
 * @file TuringMachine.h
 * @brief Заголовочный файл, содержащий объявление класса TuringMachine.
 */

/**
 * @brief Класс, реализующий исполнительную логику машины Тьюринга.
 *
 * Управляет лентой, текущим состоянием, алфавитом и таблицей переходов.
 * Метод step() выполняет один такт работы согласно программе.
 */
class TuringMachine {
private:
    Tape tape;                     ///< Рабочая лента машины
    Program program;               ///< Таблица переходов (алгоритм)
    std::set<char> alphabet;       ///< Допустимый алфавит символов ленты
    std::string current_state;     ///< Текущее активное состояние машины
public:
    /**
     * @brief Выполнить один шаг машины Тьюринга.
     *
     * Ищет переход для пары (current_state, tape.get_symbol()).
     * Если переход найден: записывает new_symbol, двигает голову, меняет состояние.
     * Если не найден: возвращает false (остановка).
     * 
     * @return true, если шаг выполнен успешно и можно продолжать, false если остановка.
     */
    bool step();
    
    /**
     * @brief Получить константную ссылку на текущее состояние ленты.
     * @return Ссылка на объект Tape.
     */
    const Tape& get_tape() const { return tape; }
    
    /**
     * @brief Получить имя текущего состояния машины.
     * @return Строка с названием состояния (например, "q0").
     */
    std::string get_current_state() const { return current_state; }
    
    /**
     * @brief Установить программу переходов.
     * @param p Объект Program.
     */
    void set_program(const Program& p) { program = p; }
    
    /**
     * @brief Установить алфавит ленты.
     * @param alph Множество символов.
     */
    void set_alphabet(const std::set<char>& alph) { alphabet = alph; }
    
    /**
     * @brief Установить начальное (текущее) состояние машины.
     * @param state Имя состояния.
     */
    void set_initial_state(const std::string& state) { current_state = state; }

    /**
     * @brief Проверить, настроена ли машина (заполнен ли алфавит).
     * @return true, если alphabet не пуст, иначе false.
     */
    bool is_configured() const { return !alphabet.empty(); }
    
    /**
     * @brief Безопасно установить содержимое ленты, проверяя символы по алфавиту.
     * @param line Строка с символами ленты.
     * @return Пустую строку при успехе или сообщение об ошибке, если есть недопустимые символы.
     */
    std::string set_tape(const std::string& line);
    
    /**
     * @brief Установить позицию головки ленты.
     * @param position Индекс ячейки.
     */
    void set_head_position(int position) { tape.set_head_position(position); }

    /**
     * @brief Оператор проверки машин Тьюринга на равенство.
     * @param other Другая машина для сравнения.
     * @return true, если лента, программа, алфавит и состояние совпадают, иначе false.
     */
    bool operator==(const TuringMachine& other) const {
        return tape == other.tape &&
               program == other.program &&
               alphabet == other.alphabet &&
               current_state == other.current_state;
    }

    /**
     * @brief Оператор проверки машин Тьюринга на неравенство.
     * @param other Другая машина для сравнения.
     * @return true, если машины различаются, иначе false.
     */
    bool operator!=(const TuringMachine& other) const {
        return !(*this == other);
    }

    /**
     * @brief Перегрузка оператора вывода машины в поток.
     * Выводит текущее состояние, алфавит, программу и визуальное представление ленты.
     * @param os Поток вывода.
     * @param turing_machine Машина для вывода.
     * @return Ссылка на поток вывода.
     */
    friend std::ostream& operator<<(std::ostream& os, const TuringMachine& turing_machine);
    
    /**
     * @brief Перегрузка оператора ввода машины из потока.
     * Читает полную конфигурацию: состояние -> алфавит -> правила -> лента.
     * Примечание: данная реализация делегирует чтение правил и ленты отдельным операторам >>.
     * @param is Поток ввода.
     * @param turing_machine Объект машины для заполнения.
     * @return Ссылка на поток ввода.
     */
    friend std::istream& operator>>(std::istream& is, TuringMachine& turing_machine);
};

#endif