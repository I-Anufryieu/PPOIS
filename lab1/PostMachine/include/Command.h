#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>

/**
 * @file Command.h
 * @brief Заголовочный файл, содержащий объявление класса Command.
 */

/**
 * @brief Класс, описывающий отдельную команду машины Поста.
 */
class Command {
public:
    static constexpr char SET1  = 'V';    ///< Символ команды: записать 1
    static constexpr char SET0  = 'X';    ///< Символ команды: записать 0
    static constexpr char RIGHT = '>';    ///< Символ команды: сместить каретку вправо
    static constexpr char LEFT  = '<';    ///< Символ команды: сместить каретку влево
    static constexpr char BRANCH = '?';   ///< Символ команды: условный переход
    static constexpr char STOP   = '!';   ///< Символ команды: остановка алгоритма
    static constexpr int  INVALID_NUM = -1; ///< Маркер недопустимого номера команды

private:
    int number;     ///< Порядковый номер команды в программе
    char symbol;    ///< Символ (тип) операции
    int fdigit;     ///< Первый числовой параметр (номер следующей команды)
    int sdigit;     ///< Второй числовой параметр (номер команды при переходе по условию)

public:
    /**
     * @brief Конструктор команды.
     * @param number Порядковый номер команды.
     * @param symbol Тип операции (символ команды).
     * @param fdigit Первый числовой параметр.
     * @param sdigit Второй числовой параметр.
     */
    Command(int number = INVALID_NUM, char symbol = STOP, int fdigit = INVALID_NUM, int sdigit = INVALID_NUM);

    /** @brief Получить порядковый номер команды. */
    int get_number() const;
    
    /** @brief Получить тип операции (символ). */
    char get_symbol() const;
    
    /** @brief Получить первый числовой параметр. */
    int get_fdigit() const;
    
    /** @brief Получить второй числовой параметр. */
    int get_sdigit() const; 

    /**
     * @brief Оператор проверки равенства двух команд.
     * @param other Другая команда для сравнения.
     * @return true, если все поля команд идентичны, иначе false.
     */
    bool operator==(const Command& other) const;
    
    /**
     * @brief Оператор проверки неравенства двух команд.
     * @param other Другая команда для сравнения.
     * @return true, если команды отличаются, иначе false.
     */
    bool operator!=(const Command& other) const;
};

/**
 * @brief Перегрузка оператора вывода команды в поток.
 * @param os Поток вывода.
 * @param cmd Команда для вывода.
 * @return Ссылка на поток вывода.
 */
std::ostream& operator<<(std::ostream& os, const Command& cmd);

/**
 * @brief Перегрузка оператора ввода команды из потока.
 * @param is Поток ввода.
 * @param cmd Объект команды для заполнения.
 * @return Ссылка на поток ввода.
 */
std::istream& operator>>(std::istream& is, Command& cmd);

#endif // COMMAND_H