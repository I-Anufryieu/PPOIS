#ifndef TURINGMACHINE_H
#define TURINGMACHINE_H

#include <iostream>
#include <list>
#include <iterator>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <limits>

/**
 * @file TuringMachine.h
 * @brief Заголовочный файл, содержащий объявления структур Transition и классов Tape, Program, Parser и TuringMachine.
 */

constexpr char BLANK_SYMBOL = '_'; ///< Символ пустой ячейки ленты машины Тьюринга
constexpr int MAX_STEPS = 1000;    ///< Максимальное количество шагов для предотвращения бесконечного цикла

/**
 * @brief Структура, описывающая один переход в машине Тьюринга.
 *
 * Определяет действие, которое машина выполняет при обнаружении определённого символа
 * в текущем состоянии: какой символ записать на ленту, куда сдвинуть каретку и в какое
 * состояние перейти.
 */
struct Transition{
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

/**
 * @brief Класс, представляющий ленту машины Тьюринга.
 *
 * Лента реализована на базе двусвязного списка (std::list<char>) с итератором каретки.
 * Позволяет хранить произвольные символы алфавита, динамически расширяться при выходе
 * за границы и эффективно перемещаться в обе стороны.
 */
class Tape{
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
    char get_symbol() const{ return *cur;}
    
    /**
     * @brief Запись символа в текущую позицию каретки.
     * @param symbol Символ для записи.
     */
    void set_symbol(char symbol){ *cur = symbol; }
    
    /**
     * @brief Установка каретки на конкретную позицию по индексу.
     * @param position Индекс ячейки, куда нужно установить каретку (начиная с 0).
     */
    void set_head_position(int position){
        cur=tape.begin();
        std::advance(cur,position);
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
     * @return true, если содержимое лент и позиции кареток совпадают, иначе false.
     */
    bool operator==(const Tape& other) const{
        return (this->tape == other.tape) && 
               (std::distance(this->tape.cbegin(), std::list<char>::const_iterator(this->cur)) == 
                std::distance(other.tape.cbegin(), std::list<char>::const_iterator(other.cur)));
    }
    
    /**
     * @brief Оператор проверки лент на неравенство.
     * @param other Другая лента для сравнения.
     * @return true, если ленты различаются, иначе false.
     */
    bool operator!=(const Tape& other) const{ return !(*this==other); }

    /**
     * @brief Перегрузка оператора вывода ленты в выходной поток.
     * Выводит содержимое ленты, заключая текущий символ в квадратные скобки [X].
     * @param os Поток вывода.
     * @param tape Лента для вывода.
     * @return Ссылка на поток вывода.
     */
    friend std::ostream &operator<<(std::ostream& os,const Tape& tape);
    
    /**
     * @brief Перегрузка оператора ввода ленты из входного потока.
     * Читает строку без пробелов и заполняет ей ленту, ставя каретку в начало.
     * @param is Поток ввода.
     * @param tape Лента для записи прочитанных данных.
     * @return Ссылка на поток ввода.
     */
    friend std::istream &operator>>(std::istream& is,Tape& tape);
};

/**
 * @brief Класс, хранящий таблицу переходов (программу) машины Тьюринга.
 *
 * Представляет собой ассоциативный массив, где ключом является пара (состояние, символ),
 * а значением — структура Transition, описывающая действие.
 */
class Program{
private:
    std::map<std::pair<std::string,char>,Transition> state_table; ///< Таблица переходов программы
public:
    /**
     * @brief Добавить или обновить переход в таблице.
     * @param state Текущее состояние.
     * @param symbol Символ, считанный с ленты.
     * @param transition Объект перехода (что писать, куда двигаться, куда идти).
     */
    void add_transition(const std::string& state, char symbol, const Transition& transition){
        state_table[{state,symbol}] = transition;
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

/**
 * @brief Класс-парсер для чтения конфигурации машины Тьюринга из текстовых строк.
 *
 * Отвечает за валидацию формата правил, проверку наличия состояний и символов в алфавите
 * перед добавлением их в программу. Используется в интерактивном режиме main.cpp.
 */
class Parser{
private:
    std::set<char> tape_alphabet;      ///< Множество допустимых символов алфавита ленты
    std::set<std::string> tape_states; ///< Множество объявленных состояний машины
    Program program;                   ///< Собираемая программа (таблица переходов)

    
public:
    /**
     * @brief Получить собранную программу.
     * @return Копия объекта Program.
     */
    Program get_program() const{ return program;}
    
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
    const std::set<char>& get_tape_alphabet() const{ return tape_alphabet;}
    
    /**
     * @brief Получить константную ссылку на разобранные состояния.
     * @return Ссылка на std::set<std::string>.
     */
    const std::set<std::string>& get_tape_states() const{ return tape_states;}
    
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

/**
 * @brief Класс, реализующий исполнительную логику машины Тьюринга.
 *
 * Управляет лентой, текущим состоянием, алфавитом и таблицей переходов.
 * Метод step() выполняет один такт работы согласно программе.
 */
class TuringMachine{
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
    const Tape& get_tape() const{ return tape; }
    
    /**
     * @brief Получить имя текущего состояния машины.
     * @return Строка с названием состояния (например, "q0").
     */
    std::string get_current_state() const{ return current_state; }
    
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
    void set_head_position(int position) {tape.set_head_position(position);}

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