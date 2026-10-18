#ifndef POSTMACHINE_H
#define POSTMACHINE_H

#include <iostream>
#include <map>
#include <string>
#include <stdexcept>
#include "Tape.h"
#include "Command.h"

/**
 * @file PostMachine.h
 * @brief Заголовочный файл, содержащий объявление класса PostMachine.
 */

/**
 * @brief Класс, реализующий исполнительную логику машины Поста.
 * 
 * Управляет таблицей команд, текущим состоянием ленты и счётчиком команд.
 */
class PostMachine {
private:
    std::map<int, Command> command; ///< Набор команд программы (ассоциативный массив: номер команды -> команда)
    Tape tape;                      ///< Лента машины Поста
    int current_line = 1;           ///< Номер текущей выполняемой команды (счётчик команд)

public:
    /** @brief Получить номер текущей команды. */
    int get_current_line() const { return current_line; }
    
    /** @brief Получить константную ссылку на ленту машины. */
    const Tape& get_tape() const { return tape; }

    /**
     * @brief Добавить новую команду в программу машины.
     * @param cmd Объект команды.
     * @throws std::invalid_argument если команда с таким номером уже существует в программе.
     */
    void add_command(const Command& cmd);

    /**
     * @brief Выполнить один шаг (одну итерацию) выполнения алгоритма.
     * @return true, если выполнение должно быть продолжено, false при достижении команды останова (!).
     * @throws std::out_of_range если запрашиваемый номер команды отсутствует в программе.
     */
    bool step();

    /**
     * @brief Установка начального состояния ленты и каретки.
     * @param initial_state Строка с начальным содержимым ленты.
     * @param start_pos Начальная позиция каретки.
     */
    void set_tape(const std::string& initial_state, int start_pos = 0) {
        tape.set_tape(initial_state, start_pos);
    }

    /**
     * @brief Сброс состояния машины Поста (очистка набора команд и сброс счётчика команд на 1).
     */
    void reset() {
        command.clear();
        current_line = 1;
    }

    /**
     * @brief Оператор проверки равенства двух машин Поста.
     * @param other Другая машина Поста для сравнения.
     * @return true, если ленты, наборы команд и текущие позиции совпадают, иначе false.
     */
    bool operator==(const PostMachine& other) const {
        return tape == other.tape && 
               command == other.command && 
               current_line == other.current_line;
    }

    /**
     * @brief Оператор проверки неравенства двух машин Поста.
     * @param other Другая машина Поста для сравнения.
     * @return true, если машины различаются, иначе false.
     */
    bool operator!=(const PostMachine& other) const {
        return !(*this == other);
    }
};

#endif // POSTMACHINE_H