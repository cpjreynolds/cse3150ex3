#include <ios>
#include <numeric>
#include <iostream>

#include "student.hpp"

double student::total_score() const
{
    // implicit casting from int->double
    double hwsum = std::accumulate(hw_grades.cbegin(), hw_grades.cend(), 0.0);
    return final_grade * 0.3 + midterm * 0.3 + hwsum * 0.4;
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

    std::istream::sentry s(is);
    if (s) {
        eat(is, '{');
        is >> obj.id;
        eat(is, ',');
        std::string name;
        std::getline(is, name, ',');
        obj.name = std::move(name);
        is >> obj.final_grade;
        eat(is, ',');
        is >> obj.midterm;
        // eat(is, ',');
        std::vector<int> hws;
        char inch;
        is >> inch;
        while (is && inch != '}') {
            int x;
            is >> x;
            hws.push_back(x);
            is >> inch;
        }
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const student& obj)
{
    os << '{' << obj.id << ',' << obj.name << ',' << obj.final_grade << ','
       << obj.midterm << ',';
    for (auto it = obj.hw_grades.cbegin(); it != obj.hw_grades.cend(); ++it) {
        os << *it;
        if (it != obj.hw_grades.cend() - 1) {
            os << ',';
        }
    }
    os << '}';
    return os;
}
