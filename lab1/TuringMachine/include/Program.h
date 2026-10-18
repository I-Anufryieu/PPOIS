#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <string>
#include <map>
#include <utility>
#include "Transition.h"

/**
 * @file Program.h
 * @brief Заголовочный файл, содержащий объявление класса Program.
 */

/**
 * @brief Класс, хранящий таблицу переходов (программу) машины Тьюринга.
 *
 * Представляет собой ассоциативный массив, где ключом является пара (состояние, символ),
 * а значением — структура Transition, описывающая действие.
 */
class Program {
private:
    std::map<std::pair<std::string, char>, Transition> state_table; ///< Таблица переходов программы
public:
    /**
     * @brief Добавить или обновить переход в таблице.
     * @param state Текущее состояние.
     * @param symbol Символ, считанный с ленты.
     * @param transition Объект перехода (что писать, куда двигаться, куда идти).
     */
    void add_transition(const std::string& state, char symbol, const Transition& transition) {
        state_table[{state, symbol}] = transition;
    }
    
    /**
     * @brief Найти переход для заданного состояния и символа.
     * @param state Текущее состояние.
     * @param symbol Символ под головкой.
     * @param next_transition Выходной параметр: найденный переход.
     * @return true, если переход существует, иначе false.
     */
    bool get_transition(const std::string& state, char symbol, Transition& next_transition) const;

    /**
     * @brief Оператор проверки программ на равенство.
     * @param other Другая программа для сравнения.
     * @return true, если таблицы переходов идентичны, иначе false.
     */
    bool operator==(const Program& other) const {
        return state_table == other.state_table;
    }

    /**
     * @brief Оператор проверки программ на неравенство.
     * @param other Другая программа для сравнения.
     * @return true, если программы различаются, иначе false.
     */
    bool operator!=(const Program& other) const {
        return !(*this == other);
    }

    /**
     * @brief Перегрузка оператора вывода программы в поток.
     * @param os Поток вывода.
     * @param program Программа для вывода.
     * @return Ссылка на поток вывода.
     */
    friend std::ostream& operator<<(std::ostream& os, const Program& program);
    
    /**
     * @brief Перегрузка оператора ввода программы из потока.
     * Читает правила перехода до конца потока (жадно).
     * @param is Поток ввода.
     * @param program Объект программы для заполнения.
     * @return Ссылка на поток ввода.
     */
    friend std::istream& operator>>(std::istream& is, Program& program);
};

#endif