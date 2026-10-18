#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include "Program.h"
#include "Transition.h"

/**
 * @file Parser.h
 * @brief Заголовочный файл, содержащий объявление класса Parser.
 */

/**
 * @brief Класс-парсер для чтения конфигурации машины Тьюринга из текстовых строк.
 *
 * Отвечает за валидацию формата правил, проверку наличия состояний и символов в алфавите
 * перед добавлением их в программу. Используется в интерактивном режиме main.cpp.
 */
class Parser {
private:
    std::set<char> tape_alphabet;      ///< Множество допустимых символов алфавита ленты
    std::set<std::string> tape_states; ///< Множество объявленных состояний машины
    Program program;                   ///< Собираемая программа (таблица переходов)

public:
    /**
     * @brief Получить собранную программу.
     * @return Копия объекта Program.
     */
    Program get_program() const { return program; }
    
    /**
     * @brief Разобрать строку с алфавитом и сохранить символы во внутреннее множество.
     * @param line Строка вида "A B _".
     */
    void parse_tape_alphabet(const std::string& line);
    
    /**
     * @brief Разобрать строку с состояниями и сохранить имена во внутреннее множество.
     * @param line Строка вида "q0 q1 qf".
     */
    void parse_tape_states(const std::string& line);
    
    /**
     * @brief Получить константную ссылку на разобранный алфавит.
     * @return Ссылка на std::set<char>.
     */
    const std::set<char>& get_tape_alphabet() const { return tape_alphabet; }
    
    /**
     * @brief Получить константную ссылку на разобранные состояния.
     * @return Ссылка на std::set<std::string>.
     */
    const std::set<std::string>& get_tape_states() const { return tape_states; }
    
    /**
     * @brief Проверить формат правила, его семантическую корректность и добавить в программу.
     * @param rule Строка правила вида "q0 A B R q1".
     * @return Пустую строку при успехе или сообщение об ошибке при неудаче.
     */
    std::string parse_and_add_transition(const std::string& rule);

    /**
     * @brief Оператор проверки парсеров на равенство.
     * @param other Другой парсер для сравнения.
     * @return true, если алфавиты, состояния и программы совпадают, иначе false.
     */
    bool operator==(const Parser& other) const {
        return tape_alphabet == other.tape_alphabet &&
               tape_states == other.tape_states &&
               program == other.program;
    }

    /**
     * @brief Оператор проверки парсеров на неравенство.
     * @param other Другой парсер для сравнения.
     * @return true, если парсеры различаются, иначе false.
     */
    bool operator!=(const Parser& other) const {
        return !(*this == other);
    }

    /**
     * @brief Перегрузка оператора вывода информации о парсере в поток.
     * @param os Поток вывода.
     * @param parser Парсер для вывода.
     * @return Ссылка на поток вывода.
     */
    friend std::ostream& operator<<(std::ostream& os, const Parser& parser);
};

#endif