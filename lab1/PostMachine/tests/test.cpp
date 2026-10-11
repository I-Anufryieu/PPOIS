#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../PostMachine.h"
#include <sstream>

TEST_CASE("1. Полное покрытие ленты", "[Tape]") {
    Tape tape;

    SECTION("Установка символа и навигация") {
        tape.set_tape("0", 0);
        tape.set_symbol(1);
        REQUIRE(tape.get_symbol() == 1);

        tape.move_left();
        REQUIRE(tape.get_symbol() == 0);

        tape.move_right();
        tape.move_right();
        REQUIRE(tape.get_symbol() == 0);
    }

    SECTION("Конструктор копирования и присваивание") {
        Tape tape1;
        tape1.set_tape("101", 1);

        Tape tape2(tape1); 
        REQUIRE(tape1.get_symbol() == tape2.get_symbol());

        Tape tape3;
        tape3 = tape1;
        REQUIRE(tape3.get_symbol() == tape1.get_symbol());

        Tape self_assign;
        self_assign = self_assign;
        REQUIRE(self_assign.get_symbol() == 0);
    }

    SECTION("Потоки и пустое состояние") {
        Tape empty_tape;
        empty_tape.set_tape("", 0);

        std::stringstream ss;
        ss << tape; 

        std::stringstream input_stream("101");
        Tape tape_stream;
        input_stream >> tape_stream; 
        REQUIRE(tape_stream.get_symbol() == 1);
    }
}

TEST_CASE("2. Полное покрытие парсера команд", "[Command]") {
    SECTION("Все типы команд и ветвления парсинга") {
        REQUIRE(Command::parser("1 V 2").get_symbol() == 'V');
        REQUIRE(Command::parser("2 X 3").get_symbol() == 'X');
        REQUIRE(Command::parser("3 > 4").get_symbol() == '>');
        REQUIRE(Command::parser("4 < 1").get_symbol() == '<');
        REQUIRE(Command::parser("5 ? 2 3").get_symbol() == '?');
        REQUIRE(Command::parser("6 !").get_symbol() == '!');
        REQUIRE(Command::parser("7 bad_syntax").get_number() == -1);
    }
}

TEST_CASE("3. Полное покрытие машины Поста", "[PostMachine]") {
    PostMachine pm;

    SECTION("Выполнение всех ветвлений команд") {
        pm.set_tape("0", 0);
        pm.add_command(Command::parser("1 ? 2 3"));
        pm.add_command(Command::parser("2 V 4"));
        pm.add_command(Command::parser("3 X 4"));
        pm.add_command(Command::parser("4 > 5"));
        pm.add_command(Command::parser("5 < 6"));
        pm.add_command(Command::parser("6 !"));

        while(pm.step());
    }

    SECTION("Исключение при выходе за диапазон строки") {
        pm.reset();
        pm.set_tape("1", 0);
        pm.add_command(Command::parser("10 !"));
        REQUIRE_THROWS_AS(pm.step(), std::out_of_range);
    }
}