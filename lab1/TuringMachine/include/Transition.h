#ifndef TRANSITION_H
#define TRANSITION_H

#include <string>

/**
 * @file Transition.h
 * @brief Заголовочный файл, содержащий объявление структуры Transition.
 */

/**
 * @brief Структура, описывающая один переход в машине Тьюринга.
 *
 * Определяет действие, которое машина выполняет при обнаружении определённого символа
 * в текущем состоянии: какой символ записать на ленту, куда сдвинуть каретку и в какое
 * состояние перейти.
 */
class Transition {
public:
    char next_symbol;          ///< Символ, который будет записан на ленту вместо текущего
    char move_direction;       ///< Направление движения головки ('R' - вправо, 'L' - влево, 'S' - остаться)
    std::string next_state;    ///< Имя состояния, в которое переходит машина после выполнения действия

    /**
     * @brief Оператор проверки равенства двух переходов.
     * @param other Другой переход для сравнения.
     * @return true, если все поля переходов идентичны, иначе false.
     */
    bool operator==(const Transition& other) const {
        return next_symbol == other.next_symbol &&
               move_direction == other.move_direction &&
               next_state == other.next_state;
    }

    /**
     * @brief Оператор проверки неравенства двух переходов.
     * @param other Другой переход для сравнения.
     * @return true, если переходы различаются, иначе false.
     */
    bool operator!=(const Transition& other) const {
        return !(*this == other);
    }
};

#endif