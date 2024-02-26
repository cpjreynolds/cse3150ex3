#include "student.hpp"

#include <iostream>
#include <fstream>

#ifndef TESTING

// ingests student records from istream and pretty-prints them to ostream.
static void read_print_students(std::istream& is, std::ostream& os)
{
    for (student s; is >> s;) {
        os << s.pretty_print() << std::endl;
    }
}

static constexpr std::string_view DEFAULT_FILE = "test.txt";

static constexpr std::string_view USAGE = "USAGE: ./ex3.out [file=test.txt]";

int main(int argc, char** argv)
{
    std::string_view fname = DEFAULT_FILE;
    if (argc > 1) {
        fname = argv[1];
    }
    if (argc > 2) {
        std::cerr << "invalid arguments\n\n";
        std::cerr << USAGE << std::endl;
        return 1;
    }
    std::ifstream ifile(fname);
    if (!ifile.is_open()) {
        std::cerr << "unable to open file: " << fname << std::endl;
        std::cerr << USAGE << std::endl;
        return 1;
    }

    read_print_students(ifile, std::cout);
}

#else

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#endif
