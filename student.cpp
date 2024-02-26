#include <algorithm>
#include <ios>
#include <numeric>
#include <iostream>
#include <cctype>
#include <sstream>

#include "student.hpp"

double student::total_score() const
{
    // implicit casting from int->double
    double hwavg = std::accumulate(hw_grades.cbegin(), hw_grades.cend(), 0.0) /
                   hw_grades.size();
    hwavg = hw_grades.size() == 0 ? 0.0 : hwavg; // handle divide by zero
    return final_grade * 0.3 + midterm * 0.3 + hwavg * 0.4;
}

std::string student::pretty_print() const
{
    std::ostringstream ss;
    ss << "ID:\t\t" << id << std::endl;
    ss << "name:\t\t" << name << std::endl;
    ss << "final:\t\t" << final_grade << std::endl;
    ss << "midterm:\t" << midterm << std::endl;
    ss << "hw_grades:\t";
    for (auto it = hw_grades.cbegin(); it != hw_grades.cend(); ++it) {
        ss << *it;
        if (it != hw_grades.cend() - 1) {
            ss << ',';
        }
    }
    ss << std::endl;
    ss << "total score:\t" << total_score() << std::endl;
    return ss.str();
}

std::istream& operator>>(std::istream& is, student& obj)
{
    auto eat = [](std::istream& is, char c) -> std::istream& {
        char inch;
        if (is >> inch; inch != c) {
            is.setstate(std::ios_base::failbit);
        }
        return is;
    };

    // checks for a valid name using the stream locale because I'm pedantic like
    // that.
    auto valid_name = [](const std::istream& is, std::string_view s) -> bool {
        const auto loc = is.getloc();
        return std::ranges::all_of(s, [&](const char& c) {
            return std::isalpha(c, loc) || std::isspace(c, loc);
        });
    };

    // parse into a temporary, then swap into the given obj only if no
    // failbit/badbit is set on the istream.
    student tobj;
    std::istream::sentry s(is);
    if (s) {
        is >> std::ws;
        eat(is, '{');
        is >> tobj.id;
        eat(is, ',');
        std::string name;
        is >> std::ws;
        std::getline(is, name, ',');
        if (!valid_name(is, name)) {
            is.setstate(std::ios_base::failbit);
        }
        tobj.name = std::move(name);
        is >> tobj.final_grade;
        eat(is, ',');
        is >> tobj.midterm;
        std::vector<int> hws;
        for (char inch; is >> inch && inch != '}';) {
            if (inch != ',') {
                is.setstate(std::ios_base::failbit);
            }
            int x;
            is >> x;
            hws.push_back(x);
        }
        tobj.hw_grades = std::move(hws);
    }
    if (is) { // if nothing bad has happened other than possibly EOF
        // successful parse so move assign into obj.
        obj = std::move(tobj);
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const student& obj)
{
    os << '{' << obj.id << ',' << obj.name << ',' << obj.final_grade << ','
       << obj.midterm;
    for (auto& hw : obj.hw_grades) {
        os << ',' << hw;
    }
    os << '}';
    return os;
}

#ifdef TESTING
#include "doctest.h"

TEST_CASE("total_score")
{
    SUBCASE("regular student")
    {
        student x(123, "Ada Lovelace", 99, 98, {10, 20, 30});

        double sgrade = 0.3 * 99 + 0.3 * 98 + ((10 + 20 + 30) / 3) * 0.4;

        CHECK(x.total_score() == sgrade);
    }
    SUBCASE("no homework")
    {
        student x(123, "Ada Lovelace", 99, 98, {});

        double sgrade = 0.3 * 99 + 0.3 * 98 + 0.0 * 0.4;

        CHECK(x.total_score() == sgrade);
    }

    SUBCASE("default student")
    {
        student x;

        CHECK(x.total_score() == 0.0);
    }
}

TEST_CASE("operator>>")
{
    SUBCASE("good parse")
    {
        std::istringstream is("{123, Ada Lovelace, 99, 98, 10, 20, 30}");
        student x;
        is >> x;
        CHECK(x == student(123, "Ada Lovelace", 99, 98, {10, 20, 30}));
    }

    SUBCASE("bad parse")
    {
        std::istringstream is("{123, Ada Lovelace, 9x, 98, 10, 20, 30}");
        student x;
        is >> x;
        CHECK(x == student());
        CHECK(is.fail());
    }
}

TEST_CASE("operator<<")
{
    SUBCASE("regular output")
    {
        std::ostringstream os;
        student x(123, "Ada Lovelace", 99, 98, {10, 20, 30});

        os << x;

        CHECK(os.str() == "{123,Ada Lovelace,99,98,10,20,30}");
    }

    SUBCASE("default output")
    {
        std::ostringstream os;
        student x;

        os << x;

        CHECK(os.str() == "{0,,0,0}");
    }
}

TEST_CASE("input-output symmetry")
{
    std::stringstream ss;
    student x(123, "Ada Lovelace", 99, 98, {10, 20, 30});
    student y;

    ss << x;
    ss >> y;

    CHECK(x == y);
}

#endif
