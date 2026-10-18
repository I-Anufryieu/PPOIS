#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../include/TuringMachine.h"
#include "../include/Tape.h"
#include "../include/Program.h"
#include "../include/Parser.h"
#include <sstream>

namespace TapeTests {

TEST_CASE("Tape: default construction", "[Tape]") {
    Tape tape;
    REQUIRE(tape.get_symbol() == '_');
}

TEST_CASE("Tape: set and get symbol", "[Tape]") {
    Tape tape;
    tape.set_symbol('A');
    REQUIRE(tape.get_symbol() == 'A');
    tape.set_symbol('B');
    REQUIRE(tape.get_symbol() == 'B');
}

TEST_CASE("Tape: move right", "[Tape]") {
    Tape tape;
    tape.set_symbol('A');
    tape.move_right();
    REQUIRE(tape.get_symbol() == '_');
}

TEST_CASE("Tape: move left", "[Tape]") {
    Tape tape;
    tape.set_symbol('A');
    tape.move_right();
    tape.set_symbol('B');
    tape.move_left();
    REQUIRE(tape.get_symbol() == 'A');
}

TEST_CASE("Tape: move left from beginning adds blank", "[Tape]") {
    Tape tape;
    tape.move_left();
    REQUIRE(tape.get_symbol() == '_');
}

TEST_CASE("Tape: copy constructor", "[Tape]") {
    Tape tape1;
    tape1.set_symbol('X');
    tape1.move_right();
    tape1.set_symbol('Y');
    Tape tape2(tape1);
    REQUIRE(tape1 == tape2);
    REQUIRE(tape2.get_symbol() == 'Y');
}

TEST_CASE("Tape: assignment operator", "[Tape]") {
    Tape tape1;
    tape1.set_symbol('X');
    tape1.move_right();
    tape1.set_symbol('Y');
    Tape tape2;
    tape2 = tape1;
    REQUIRE(tape1 == tape2);
}

TEST_CASE("Tape: set head position", "[Tape]") {
    Tape tape;
    tape.set_symbol('A');
    tape.move_right();
    tape.set_symbol('B');
    tape.move_right();
    tape.set_symbol('C');
    tape.set_head_position(1);
    REQUIRE(tape.get_symbol() == 'B');
    tape.set_head_position(0);
    REQUIRE(tape.get_symbol() == 'A');
}

TEST_CASE("Tape: output operator", "[Tape][IO]") {
    Tape tape;
    tape.set_symbol('A');
    tape.move_right();
    tape.set_symbol('B');
    std::ostringstream oss;
    oss << tape;
    std::string output = oss.str();
    REQUIRE(output.find("[B]") != std::string::npos);
    REQUIRE(output.find("A") != std::string::npos);
}

TEST_CASE("Tape: input operator", "[Tape][IO]") {
    Tape tape;
    std::istringstream iss("ABC");
    iss >> tape;
    REQUIRE(tape.get_symbol() == 'A');
}

} 

namespace ProgramTests {

TEST_CASE("Program: add and get transition", "[Program]") {
    Program program;
    Transition t{'B', 'R', "q1"};
    program.add_transition("q0", 'A', t);
    Transition result;
    REQUIRE(program.get_transition("q0", 'A', result) == true);
    REQUIRE(result == t);
}

TEST_CASE("Program: get non-existent transition", "[Program]") {
    Program program;
    Transition result;
    REQUIRE(program.get_transition("q0", 'A', result) == false);
}

TEST_CASE("Program: overwrite transition", "[Program]") {
    Program program;
    Transition t1{'B', 'R', "q1"};
    Transition t2{'C', 'L', "q2"};
    program.add_transition("q0", 'A', t1);
    program.add_transition("q0", 'A', t2);
    Transition result;
    program.get_transition("q0", 'A', result);
    REQUIRE(result == t2);
}

TEST_CASE("Program: output operator", "[Program][IO]") {
    Program program;
    program.add_transition("q0", 'A', {'B', 'R', "q1"});
    program.add_transition("q1", 'B', {'C', 'L', "q0"});
    std::ostringstream oss;
    oss << program;
    std::string output = oss.str();
    REQUIRE(output.find("q0") != std::string::npos);
    REQUIRE(output.find("q1") != std::string::npos);
}

TEST_CASE("Program: input operator", "[Program][IO]") {
    Program program;
    std::istringstream iss("q0 A B R q1\nq1 B C L q0");
    iss >> program;
    Transition t;
    REQUIRE(program.get_transition("q0", 'A', t) == true);
    REQUIRE(t.next_symbol == 'B');
    REQUIRE(t.move_direction == 'R');
    REQUIRE(t.next_state == "q1");
}

} 

namespace ParserTests {

TEST_CASE("Parser: parse tape alphabet", "[Parser]") {
    Parser parser;
    parser.parse_tape_alphabet("A B _");
    REQUIRE(parser.get_tape_alphabet().count('A') == 1);
    REQUIRE(parser.get_tape_alphabet().count('B') == 1);
    REQUIRE(parser.get_tape_alphabet().count('_') == 1);
    REQUIRE(parser.get_tape_alphabet().count('C') == 0);
}

TEST_CASE("Parser: parse tape states", "[Parser]") {
    Parser parser;
    parser.parse_tape_states("q0 q1 qf");
    REQUIRE(parser.get_tape_states().count("q0") == 1);
    REQUIRE(parser.get_tape_states().count("q1") == 1);
    REQUIRE(parser.get_tape_states().count("qf") == 1);
    REQUIRE(parser.get_tape_states().count("q2") == 0);
}

TEST_CASE("Parser: parse valid transition", "[Parser]") {
    Parser parser;
    parser.parse_tape_alphabet("A B _");
    parser.parse_tape_states("q0 q1");
    std::string err = parser.parse_and_add_transition("q0 A B R q1");
    REQUIRE(err.empty());
    Transition t;
    REQUIRE(parser.get_program().get_transition("q0", 'A', t) == true);
    REQUIRE(t.next_symbol == 'B');
    REQUIRE(t.move_direction == 'R');
    REQUIRE(t.next_state == "q1");
}

TEST_CASE("Parser: reject invalid transition format", "[Parser]") {
    Parser parser;
    parser.parse_tape_alphabet("A _");
    parser.parse_tape_states("q0");
    std::string err = parser.parse_and_add_transition("invalid");
    REQUIRE_FALSE(err.empty());
}

TEST_CASE("Parser: reject invalid state", "[Parser]") {
    Parser parser;
    parser.parse_tape_alphabet("A _");
    parser.parse_tape_states("q0");
    std::string err = parser.parse_and_add_transition("q99 A _ R q0");
    REQUIRE_FALSE(err.empty());
}

TEST_CASE("Parser: reject invalid symbol", "[Parser]") {
    Parser parser;
    parser.parse_tape_alphabet("A _");
    parser.parse_tape_states("q0");
    std::string err = parser.parse_and_add_transition("q0 Z _ R q0");
    REQUIRE_FALSE(err.empty());
}

TEST_CASE("Parser: reject invalid direction", "[Parser]") {
    Parser parser;
    parser.parse_tape_alphabet("A _");
    parser.parse_tape_states("q0");
    std::string err = parser.parse_and_add_transition("q0 A _ X q0");
    REQUIRE_FALSE(err.empty());
}

TEST_CASE("Parser: output operator", "[Parser][IO]") {
    Parser parser;
    parser.parse_tape_alphabet("A B _");
    parser.parse_tape_states("q0 q1 qf");
    Transition t{'B', 'R', "q1"};
    parser.get_program().add_transition("q0", 'A', t);
    std::ostringstream oss;
    oss << parser;
    std::string output = oss.str();
    REQUIRE(output.find("A") != std::string::npos);
    REQUIRE(output.find("q0") != std::string::npos);
}

TEST_CASE("Parser: multiple transitions", "[Parser]") {
    Parser parser;
    parser.parse_tape_alphabet("A B _");
    parser.parse_tape_states("q0 q1 qf");
    parser.parse_and_add_transition("q0 A B R q1");
    parser.parse_and_add_transition("q1 B A L q0");
    parser.parse_and_add_transition("q0 _ _ S qf");
    Transition t;
    REQUIRE(parser.get_program().get_transition("q0", 'A', t) == true);
    REQUIRE(t.next_state == "q1");
    REQUIRE(parser.get_program().get_transition("q1", 'B', t) == true);
    REQUIRE(t.next_state == "q0");
}

}

namespace TuringMachineTests {

TEST_CASE("TuringMachine: set tape with valid symbols", "[TuringMachine]") {
    TuringMachine tm;
    tm.set_alphabet({'A', 'B', '_'});
    tm.set_initial_state("q0");
    std::string err = tm.set_tape("AAB");
    REQUIRE(err.empty());
    REQUIRE(tm.get_tape().get_symbol() == 'A');
}

TEST_CASE("TuringMachine: reject empty tape", "[TuringMachine]") {
    TuringMachine tm;
    tm.set_alphabet({'A', '_'});
    std::string err = tm.set_tape("");
    REQUIRE_FALSE(err.empty());
}

TEST_CASE("TuringMachine: reject tape with invalid symbol", "[TuringMachine]") {
    TuringMachine tm;
    tm.set_alphabet({'A', '_'});
    std::string err = tm.set_tape("ABC");
    REQUIRE_FALSE(err.empty());
}

TEST_CASE("TuringMachine: set head position", "[TuringMachine]") {
    TuringMachine tm;
    tm.set_alphabet({'A', 'B', 'C', '_'});
    tm.set_initial_state("q0");
    tm.set_tape("ABC");
    tm.set_head_position(2);
    REQUIRE(tm.get_tape().get_symbol() == 'C');
}

TEST_CASE("TuringMachine: single step", "[TuringMachine]") {
    TuringMachine tm;
    tm.set_alphabet({'A', 'B', '_'});
    tm.set_initial_state("q0");
    Program program;
    program.add_transition("q0", 'A', {'B', 'S', "q1"});
    tm.set_program(program);
    tm.set_tape("A");
    tm.set_head_position(0);
    bool success = tm.step();
    REQUIRE(success == true);
    REQUIRE(tm.get_current_state() == "q1");
    REQUIRE(tm.get_tape().get_symbol() == 'B');
}

TEST_CASE("TuringMachine: step with no transition", "[TuringMachine]") {
    TuringMachine tm;
    tm.set_alphabet({'A', '_'});
    tm.set_initial_state("q0");
    tm.set_tape("A");
    tm.set_head_position(0);
    bool success = tm.step();
    REQUIRE(success == false);
}

TEST_CASE("TuringMachine: full algorithm run", "[TuringMachine]") {
    TuringMachine tm;
    tm.set_alphabet({'A', 'B', '_'});
    tm.set_initial_state("q0");
    Program program;
    program.add_transition("q0", 'A', {'B', 'R', "q0"});
    program.add_transition("q0", 'B', {'B', 'R', "q0"});
    program.add_transition("q0", '_', {'_', 'S', "qf"});
    tm.set_program(program);
    tm.set_tape("AAB");
    tm.set_head_position(0);
    int steps = 0;
    while (tm.step() && steps < 100) {
        steps++;
    }
    REQUIRE(tm.get_current_state() == "qf");
    REQUIRE(steps == 4);
}

TEST_CASE("TuringMachine: move left during execution", "[TuringMachine]") {
    TuringMachine tm;
    tm.set_alphabet({'A', 'B', '_'});
    tm.set_initial_state("q0");
    Program program;
    program.add_transition("q0", 'A', {'A', 'R', "q1"});
    program.add_transition("q1", 'B', {'B', 'L', "q2"});
    program.add_transition("q2", 'A', {'C', 'S', "qf"});
    tm.set_program(program);
    tm.set_tape("AB");
    tm.set_head_position(0);
    tm.step(); 
    REQUIRE(tm.get_current_state() == "q1");
    tm.step(); 
    REQUIRE(tm.get_current_state() == "q2");
    REQUIRE(tm.get_tape().get_symbol() == 'A');
    tm.step(); 
    REQUIRE(tm.get_current_state() == "qf");
    REQUIRE(tm.get_tape().get_symbol() == 'C');
}

TEST_CASE("TuringMachine: is_configured check", "[TuringMachine]") {
    TuringMachine tm;
    REQUIRE(tm.is_configured() == false);
    tm.set_alphabet({'A', '_'});
    REQUIRE(tm.is_configured() == true);
}

TEST_CASE("TuringMachine: output operator", "[TuringMachine][IO]") {
    TuringMachine tm;
    tm.set_alphabet({'A', 'B', '_'});
    tm.set_initial_state("q0");
    Program program;
    program.add_transition("q0", 'A', {'B', 'R', "q1"});
    tm.set_program(program);
    tm.set_tape("AB");
    tm.set_head_position(0);
    std::ostringstream oss;
    oss << tm;
    std::string output = oss.str();
    REQUIRE(output.find("q0") != std::string::npos);
    REQUIRE(output.find("A") != std::string::npos);
}

TEST_CASE("TuringMachine: input operator", "[TuringMachine][IO]") {
    std::istringstream iss(
        "q0\n"
        "A B _\n"
        "q0 A B R q1\n"
        "q1 B _ S qf\n"
        "AB"
    );
    TuringMachine tm;
    iss >> tm;
    REQUIRE(tm.get_current_state() == "q0");
    REQUIRE(tm.is_configured() == true);
}

TEST_CASE("TuringMachine: step with move left", "[TuringMachine]") {
    TuringMachine tm;
    tm.set_alphabet({'A', 'B', '_'});
    tm.set_initial_state("q0");
    Program program;
    program.add_transition("q0", 'A', {'B', 'L', "q1"});
    tm.set_program(program);
    tm.set_tape("A");
    tm.set_head_position(0);
    bool success = tm.step();
    REQUIRE(success == true);
    REQUIRE(tm.get_current_state() == "q1");
    REQUIRE(tm.get_tape().get_symbol() == '_');
}

TEST_CASE("TuringMachine: step with stay", "[TuringMachine]") {
    TuringMachine tm;
    tm.set_alphabet({'A', '_'});
    tm.set_initial_state("q0");
    Program program;
    program.add_transition("q0", 'A', {'B', 'S', "q1"});
    tm.set_program(program);
    tm.set_tape("A");
    tm.set_head_position(0);
    tm.step();
    REQUIRE(tm.get_current_state() == "q1");
    REQUIRE(tm.get_tape().get_symbol() == 'B');
}

TEST_CASE("TuringMachine: full run with left moves", "[TuringMachine]") {
    TuringMachine tm;
    tm.set_alphabet({'A', 'B', '_'});
    tm.set_initial_state("q0");
    Program program;
    program.add_transition("q0", 'A', {'B', 'R', "q0"});
    program.add_transition("q0", 'B', {'B', 'R', "q0"});
    program.add_transition("q0", '_', {'_', 'L', "q1"});
    program.add_transition("q1", 'B', {'A', 'L', "q1"});
    program.add_transition("q1", 'A', {'A', 'S', "qf"});
    program.add_transition("q1", '_', {'_', 'S', "qf"});
    tm.set_program(program);
    tm.set_tape("AB");
    tm.set_head_position(0);
    int steps = 0;
    while (tm.step() && steps < 100) steps++;
    REQUIRE(tm.get_current_state() == "qf");
}

}