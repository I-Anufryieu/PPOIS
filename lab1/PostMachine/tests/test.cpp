#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "PostMachine.h"
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

TEST_CASE("2. Полное покрытие парсера команд через потоки", "[Command]") {
    SECTION("Все типы команд и ветвления ввода") {
        Command cmd1;
        std::istringstream("1 V 2") >> cmd1;
        REQUIRE(cmd1.get_symbol() == Command::SET1);
        REQUIRE(cmd1.get_number() == 1);
        REQUIRE(cmd1.get_fdigit() == 2);

        Command cmd2;
        std::istringstream("2 X 3") >> cmd2;
        REQUIRE(cmd2.get_symbol() == Command::SET0);

        Command cmd3;
        std::istringstream("3 > 4") >> cmd3;
        REQUIRE(cmd3.get_symbol() == Command::RIGHT);

        Command cmd4;
        std::istringstream("4 < 1") >> cmd4;
        REQUIRE(cmd4.get_symbol() == Command::LEFT);

        Command cmd5;
        std::istringstream("5 ? 2 3") >> cmd5;
        REQUIRE(cmd5.get_symbol() == Command::BRANCH);
        REQUIRE(cmd5.get_fdigit() == 2);
        REQUIRE(cmd5.get_sdigit() == 3);

        Command cmd6;
        std::istringstream("6 !") >> cmd6;
        REQUIRE(cmd6.get_symbol() == Command::STOP);

        Command cmd7;
        std::istringstream("7 bad_syntax") >> cmd7;
        REQUIRE(cmd7.get_number() == Command::INVALID_NUM);
    }
}

TEST_CASE("3. Полное покрытие машины Поста", "[PostMachine]") {
    PostMachine pm;

    SECTION("Выполнение всех ветвлений команд") {
        pm.set_tape("0", 0);
        
        Command c1, c2, c3, c4, c5, c6;
        std::istringstream("1 ? 2 3") >> c1; pm.add_command(c1);
        std::istringstream("2 V 4") >> c2; pm.add_command(c2);
        std::istringstream("3 X 4") >> c3; pm.add_command(c3);
        std::istringstream("4 > 5") >> c4; pm.add_command(c4);
        std::istringstream("5 < 6") >> c5; pm.add_command(c5);
        std::istringstream("6 !") >> c6; pm.add_command(c6);

        while(pm.step());
    }

    SECTION("Исключение при выходе за диапазон строки") {
        pm.reset();
        pm.set_tape("0", 0);
        
        Command c(1, Command::SET1, 10);
        pm.add_command(c);
        
        pm.step();
        REQUIRE_THROWS_AS(pm.step(), std::out_of_range);
    }
}

TEST_CASE("4. Расширенные тесты Tape", "[Tape]") {
    SECTION("Автоматическое расширение ленты по краям") {
        Tape tape;
        tape.set_tape("1", 0);
        
        tape.move_left(); 
        REQUIRE(tape.get_symbol() == 0);

        tape.move_right();
        tape.move_right(); 
        REQUIRE(tape.get_symbol() == 0);
    }

    SECTION("Операторы сравнения Tape") {
        Tape t1, t2;
        t1.set_tape("101", 1);
        t2.set_tape("101", 1);
        REQUIRE(t1 == t2);

        t2.set_tape("100", 1);
        REQUIRE(t1 != t2);
    }
}

TEST_CASE("5. Расширенные тесты Command", "[Command]") {
    SECTION("Операторы сравнения Command") {
        Command c1(1, Command::SET1, 2);
        Command c2(1, Command::SET1, 2);
        Command c3(2, Command::SET0, 3);

        REQUIRE(c1 == c2);
        REQUIRE(c1 != c3);
    }

    SECTION("Потоковый ввод некорректной команды") {
        Command cmd;
        std::istringstream("invalid_command_format") >> cmd;
        REQUIRE(cmd.get_number() == Command::INVALID_NUM);
    }
}

TEST_CASE("6. Расширенные тесты PostMachine", "[PostMachine]") {
    PostMachine pm;

    SECTION("Проверка исключения при дублировании номера команды") {
        Command c1(1, Command::SET1, 2);
        Command c2(1, Command::SET0, 3);
        
        pm.add_command(c1);
        REQUIRE_THROWS_AS(pm.add_command(c2), std::invalid_argument);
    }

    SECTION("Выполнение короткого алгоритма") {
        pm.reset();
        pm.set_tape("0", 0);

        Command c1(1, Command::SET1, 2);
        Command c2(2, Command::STOP, 0);

        pm.add_command(c1);
        pm.add_command(c2);

        while (pm.step());
        REQUIRE(pm.get_tape().get_symbol() == 1);
    }

    SECTION("Сравнение машин Поста и метод reset") {
        PostMachine pm1, pm2;
        REQUIRE(pm1 == pm2);

        Command c1(1, Command::STOP, 0);
        pm1.add_command(c1);
        REQUIRE(pm1 != pm2);

        pm1.reset();
        REQUIRE(pm1 == pm2);
    }
}

TEST_CASE("7. Покрытие оператора вывода Command", "[CommandOutput]") {
    Command c1(1, Command::SET1, 2);
    std::ostringstream oss1;
    oss1 << c1;
    REQUIRE(!oss1.str().empty());

    Command c2(2, Command::BRANCH, 3, 4);
    std::ostringstream oss2;
    oss2 << c2;
    REQUIRE(!oss2.str().empty());

    Command c3(3, Command::STOP, 0);
    std::ostringstream oss3;
    oss3 << c3;
    REQUIRE(!oss3.str().empty());
}

TEST_CASE("8. Жесткое покрытие всех веток и ошибок", "[CoverageFinal]") {
    Command c;
    std::istringstream("1 V 2") >> c;
    std::istringstream("2 X 3") >> c;
    std::istringstream("3 > 4") >> c;
    std::istringstream("4 < 5") >> c;
    std::istringstream("5 ? 6 7") >> c;
    std::istringstream("6 !") >> c;
    std::istringstream("7 BAD_CHAR 8") >> c;
}