#ifndef TAPE_H
#define TAPE_H

#include <iostream>
#include <list>
#include <iterator>
#include <string>

/**
 * @file Tape.h
 * @brief Заголовочный файл, содержащий объявление класса Tape.
 */

/**
 * @brief Класс, представляющий ленту машины Поста.
 * 
 * Лента реализована на базе двусвязного списка (std::list) с итератором каретки,
 * что позволяет эффективно перемещаться в обе стороны и динамически расширять 
 * ленту в случае выхода за её текущие границы.
 */
class Tape {
private:
    std::list<int> tape;          ///< Содержимое ленты (последовательность ячеек, содержащих 0 или 1)
    std::list<int>::iterator cur; ///< Текущая позиция каретки на ленте

public:
    /**
     * @brief Конструктор по умолчанию. 
     * Инициализирует ленту одной ячейкой со значением 0 и устанавливает каретку на нее.
     */
    Tape();

    /**
     * @brief Конструктор копирования.
     * @param to_copy Лента, которую необходимо скопировать.
     */
    Tape(const Tape &to_copy);

    /**
     * @brief Установка начального состояния ленты и позиции каретки.
     * @param initial_state Строка, содержащая символы '0' и '1'.
     * @param start_pos Начальная позиция каретки (индекс ячейки).
     */
    void set_tape(const std::string& initial_state, int start_pos = 0);

    /**
     * @brief Получение символа в текущей позиции каретки.
     * @return Значение символа в текущей ячейке (0 или 1).
     */
    int get_symbol() const;

    /**
     * @brief Запись символа в текущую позицию каретки.
     * @param val Значение для записи (0 или 1).
     */
    void set_symbol(int val);

    /**
     * @brief Перемещение каретки на одну ячейку вправо.
     * Если каретка находится на последней ячейке, лента автоматически расширяется справа нулем.
     */
    void move_right();

    /**
     * @brief Перемещение каретки на одну ячейку влево.
     * Если каретка находится на первой ячейке, лента автоматически расширяется слева нулем.
     */
    void move_left();

    /**
     * @brief Оператор присваивания копированием.
     * @param to_copy Лента для копирования.
     * @return Ссылка на текущий объект.
     */
    Tape& operator=(const Tape& to_copy);

    /**
     * @brief Перегрузка оператора вывода ленты в выходной поток.
     * @param os Поток вывода.
     * @param tape Лента для вывода.
     * @return Ссылка на поток вывода.
     */
    friend std::ostream& operator<<(std::ostream& os, const Tape& tape);

    /**
     * @brief Перегрузка оператора ввода ленты из входного потока.
     * @param is Поток ввода.
     * @param tape Лента для записи прочитанных данных.
     * @return Ссылка на поток ввода.
     */
    friend std::istream& operator>>(std::istream& is, Tape& tape);

    /**
     * @brief Оператор проверки лент на равенство.
     * @param other Другая лента для сравнения.
     * @return true, если содержимое лент и позиции кареток совпадают, иначе false.
     */
    bool operator==(const Tape& other) const {
        return (this->tape == other.tape) && 
               (std::distance(this->tape.cbegin(), std::list<int>::const_iterator(this->cur)) == 
                std::distance(other.tape.cbegin(), std::list<int>::const_iterator(other.cur)));
    }

    /**
     * @brief Оператор проверки лент на неравенство.
     * @param other Другая лента для сравнения.
     * @return true, если ленты различаются, иначе false.
     */
    bool operator!=(const Tape& other) const {
        return !(*this == other);
    }
};

#endif // TAPE_H