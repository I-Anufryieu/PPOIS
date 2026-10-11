#ifndef POSTMACHINE_H
#define POSTMACHINE_H

#include <iostream>
#include <list>
#include <iterator>
#include <sstream>
#include <string>
#include <map>

/**
 * @brief Класс, реализующий ленту машины Поста.
 * 
 * Лента представляет собой двусвязный список целых чисел с возможностью 
 * перемещения каретки, ввода/вывода и сравнения состояний.
 */
class Tape{
private:
    std::list<int> tape; ///< Контейнер для хранения символов ленты.
    std::list<int>::iterator cur; ///< Итератор, указывающий на текущую позицию каретки.

public:
    /**
     * @brief Конструктор по умолчанию. Инициализирует пустую ленту.
     */
    Tape();

    /**
     * @brief Конструктор копирования.
     * @param to_copy Лента для копирования.
     */
    Tape(const Tape &to_copy);

    /**
     * @brief Установка начального состояния ленты.
     * @param initial_state Строка символов начального состояния.
     * @param start_pos Позиция каретки по умолчанию.
     */
    void set_tape(const std::string& initial_state, int start_pos = 0);

    /**
     * @brief Получить символ в текущей позиции каретки.
     * @return Значение символа.
     */
    int get_symbol() const;

    /**
     * @brief Установить символ в текущей позиции каретки.
     * @param val Новое значение символа.
     */
    void set_symbol(int val);

    /**
     * @brief Передвинуть каретку вправо.
     */
    void move_right();

    /**
     * @brief Передвинуть каретку влево.
     */
    void move_left();

    /**
     * @brief Оператор присваивания копированием.
     * @param to_copy Лента для копирования.
     * @return Ссылка на текущую ленту.
     */
    Tape& operator=(const Tape& to_copy);

    /**
     * @brief Оператор вывода ленты в поток.
     */
    friend std::ostream& operator<<(std::ostream& os, const Tape& tape);

    /**
     * @brief Оператор ввода ленты из потока.
     */
    friend std::istream& operator>>(std::istream& is, Tape& tape);

    /**
     * @brief Оператор проверки на равенство.
     */
    bool operator==(const Tape& other) const{
        return (this->tape == other.tape) && 
               (std::distance(this->tape.cbegin(), std::list<int>::const_iterator(this->cur)) == 
                std::distance(other.tape.cbegin(), std::list<int>::const_iterator(other.cur)));
    }

    /**
     * @brief Оператор проверки на неравенство.
     */
    bool operator!=(const Tape& other) const{
        return !(*this == other);
    }
};

/**
 * @brief Класс, представляющий команду машины Поста.
 */
class Command{
private:
    int number;     ///< Номер команды.
    char symbol;    ///< Символ операции.
    int fdigit;     ///< Первый операнд / номер следующей команды.
    int sdigit;     ///< Второй операнд (при необходимости).
public:
    /**
     * @brief Конструктор команды.
     * @param number Номер команды.
     * @param symbol Тип команды (символ).
     * @param fdigit Первый целевой номер.
     * @param sdigit Второй целевой номер.
     */
    Command(int number = -1, char symbol = '!', int fdigit = -1, int sdigit = -1)
        : number(number), symbol(symbol), fdigit(fdigit), sdigit(sdigit) {}

    /**
     * @brief Статический метод парсинга команды из строки.
     * @param line Строка с текстом команды.
     * @return Объект Command.
     */
    static Command parser(const std::string& line);

    /** @brief Получить номер команды. */
    int get_number() const { return number; }

    /** @brief Получить символ команды. */
    char get_symbol() const { return symbol; }

    /** @brief Получить первый числовой параметр. */
    int get_fdigit() const { return fdigit; }

    /** @brief Получить второй числовой параметр. */
    int get_sdigit() const { return sdigit; } 
};

/**
 * @brief Класс, моделирующий работу машины Поста.
 */
class PostMachine{
private:
    std::map<int, Command> command; ///< Набор команд, проиндексированный по их номерам.
    Tape tape;                      ///< Рабочая лента машины.
    int current_line = 1;           ///< Текущая выполняемая строка (команда).
public:
    /** @brief Получить текущую строку выполнения. */
    int get_current_line() const { return current_line; }

    /** @brief Получить константную ссылку на ленту. */
    const Tape& get_tape() { return tape; }

    /**
     * @brief Добавить команду в набор команд машины.
     * @param cmd Команда для добавления.
     */
    void add_command(const Command& cmd){
        command[cmd.get_number()] = cmd;
    }

    /**
     * @brief Выполнить один шаг машины Поста.
     * @return true, если выполнение продолжается, false при завершении.
     */
    bool step();

    /**
     * @brief Установка ленты для машины.
     */
    void set_tape(const std::string& initial_state, int start_pos = 0) {
        tape.set_tape(initial_state, start_pos);
    }

    /**
     * @brief Сброс состояния машины.
     */
    void reset(){
        command.clear();
        current_line = 1;
    }
};

#endif