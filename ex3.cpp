#include "student.hpp"

#include <iostream>

#ifndef TESTING

int main()
{
    student x(0xbeef, "Meep Morp", 90, 99, {30, 40, 50, 60});

    std::cout << x << std::endl;
}

#else

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#endif
