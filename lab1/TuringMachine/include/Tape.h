#ifndef TAPE_H
#define TAPE_H

#include <iostream>
#include <list>
#include <iterator>
#include <string>

constexpr char BLANK_SYMBOL = '_'; ///< Символ пустой ячейки ленты машины Тьюринга
constexpr int MAX_STEPS = 1000;    ///< Максимальное количество шагов для предотвращения бесконечного цикла

/**
 * @file Tape.h
 * @brief Заголовочный файл, содержащий объявление класса Tape.
 */

/**
 * @brief Класс, представляющий ленту машины Тьюринга.
 *
 * Лента реализована на базе двусвязного списка (std::list<char>) с итератором каретки.
 * Позволяет хранить произвольные символы алфавита, динамически расширяться при выходе
 * за границы и эффективно перемещаться в обе стороны.
 */
class Tape {
private:
    std::list<char> tape;          ///< Содержимое ленты (последовательность символов)
    std::list<char>::iterator cur; ///< Текущая позиция каретки (головки) на ленте
public:
    /**
     * @brief Конструктор по умолчанию.
     * Инициализирует ленту одной пустой ячейкой (BLANK_SYMBOL) и устанавливает каретку на неё.
     */
    Tape();
    
    /**
     * @brief Конструктор копирования.
     * @param to_copy Лента, которую необходимо скопировать.
     */
    Tape(const Tape &to_copy);
    
    ~Tape() = default; ///< Деструктор по умолчанию
    
    /**
     * @brief Перемещение каретки на одну ячейку вправо.
     * Если каретка находится на последней ячейке, лента автоматически расширяется справа пустым символом.
     */
    void move_right();
    
    /**
     * @brief Перемещение каретки на одну ячейку влево.
     * Если каретка находится на первой ячейке, лента автоматически расширяется слева пустым символом.
     */
    void move_left();

    /**
     * @brief Получение символа в текущей позиции каретки.
     * @return Символ в текущей ячейке.
     */
    char get_symbol() const { return *cur; }
    
    /**
     * @brief Запись символа в текущую позицию каретки.
     * @param symbol Символ для записи.
     */
    void set_symbol(char symbol) { *cur = symbol; }
    
    /**
     * @brief Установка каретки на конкретную позицию по индексу.
     * @param position Индекс ячейки, куда нужно установить каретку (начиная с 0).
     */
    void set_head_position(int position) {
        cur = tape.begin();
        std::advance(cur, position);
    }

    /**
     * @brief Оператор присваивания копированием.
     * @param to_copy Лента для копирования.
     * @return Ссылка на текущий объект.
     */
    Tape& operator=(const Tape& to_copy);

    /**
     * @brief Оператор проверки лент на равенство.
     * @param other Другая лента для сравнения.
     * @return true, если содержимое ленты и позиции кареток совпадают, иначе false.
     */
    bool operator==(const Tape& other) const {
        return (this->tape == other.tape) && 
               (std::distance(this->tape.cbegin(), std::list<char>::const_iterator(this->cur)) == 
                std::distance(other.tape.cbegin(), std::list<char>::const_iterator(other.cur)));
    }
    
    /**
     * @brief Оператор проверки лент на неравенство.
     * @param other Другая лента для сравнения.
     * @return true, если ленты различаются, иначе false.
     */
    bool operator!=(const Tape& other) const { return !(*this == other); }

    /**
     * @brief Перегрузка оператора вывода ленты в выходной поток.
     * Выводит содержимое ленты, заключая текущий символ в квадратные скобки [X].
     * @param os Поток вывода.
     * @param tape Лента для вывода.
     * @return Ссылка на поток вывода.
     */
    friend std::ostream &operator<<(std::ostream& os, const Tape& tape);
    
    /**
     * @brief Перегрузка оператора ввода ленты из входного потока.
     * Читает строку без пробелов и заполняет ей ленту, ставя каретку в начало.
     * @param is Поток ввода.
     * @param tape Лента для записи прочитанных данных.
     * @return Ссылка на поток ввода.
     */
    friend std::istream &operator>>(std::istream& is, Tape& tape);
};

#endif