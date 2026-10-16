#ifndef POSTMACHINE_H
#define POSTMACHINE_H

#include <iostream>
#include <list>
#include <iterator>
#include <sstream>
#include <string>
#include <map>

/**
 * @file PostMachine.h
 * @brief Заголовочный файл, содержащий объявления классов Tape, Command и PostMachine.
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
     * @@param os Поток вывода.
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
    bool operator==(const Tape& other) const{
        return (this->tape == other.tape) && 
               (std::distance(this->tape.cbegin(), std::list<int>::const_iterator(this->cur)) == 
                std::distance(other.tape.cbegin(), std::list<int>::const_iterator(other.cur)));
    }

    /**
     * @brief Оператор проверки лент на неравенство.
     * @param other Другая лента для сравнения.
     * @return true, если ленты различаются, иначе false.
     */
    bool operator!=(const Tape& other) const{
        return !(*this == other);
    }
};

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
    Command(int number = INVALID_NUM, char symbol = STOP, int fdigit = INVALID_NUM, int sdigit = INVALID_NUM)
        : number(number), symbol(symbol), fdigit(fdigit), sdigit(sdigit) {}

    /** @brief Получить порядковый номер команды. */
    int get_number() const { return number; }
    
    /** @brief Получить тип операции (символ). */
    char get_symbol() const { return symbol; }
    
    /** @brief Получить первый числовой параметр. */
    int get_fdigit() const { return fdigit; }
    
    /** @brief Получить второй числовой параметр. */
    int get_sdigit() const { return sdigit; } 

    /**
     * @brief Оператор проверки равенства двух команд.
     * @param other Другая команда для сравнения.
     * @return true, если все поля команд идентичны, иначе false.
     */
    bool operator==(const Command& other) const {
        return number == other.number && 
               symbol == other.symbol && 
               fdigit == other.fdigit && 
               sdigit == other.sdigit;
    }
    
    /**
     * @brief Оператор проверки неравенства двух команд.
     * @param other Другая команда для сравнения.
     * @return true, если команды отличаются, иначе false.
     */
    bool operator!=(const Command& other) const {
        return !(*this == other);
    }
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
    void reset(){
        command.clear();
        current_line = 1;
    }

    /**
     * @brief Оператор проверки равенства двух машин Поста.
     * @param other Другая машина Поста для сравнения.
     * @return true, если ленты, наборы команд и текущие позиции совпадают, иначе false.
     */
    bool operator==(const PostMachine& other) const{
        return tape == other.tape && 
               command == other.command && 
               current_line == other.current_line;
    }

    /**
     * @brief Оператор проверки неравенства двух машин Поста.
     * @param other Другая машина Поста для сравнения.
     * @return true, если машины различаются, иначе false.
     */
    bool operator!=(const PostMachine& other) const{
        return !(*this == other);
    }
};

#endif // POSTMACHINE_H