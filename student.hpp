#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>

// getters/setters are unnecessary given the input/output functions are friends
// and no other use requires them.
class student {
public:
    student() = default;
    student(int id, std::string_view name, int final_grade, int midterm,
            const std::vector<int>& hw_grades)
        : id{id}, name{name}, final_grade{final_grade}, midterm{midterm},
          hw_grades{hw_grades}
    {
    }

    // returns the total class score.
    double total_score() const;

    // read in a student record
    //
    // student record formatting is assumed to be a sequence of comma separated
    // values surrounded by curly braces, in the following order:
    //
    // "{id, name, final_grade, midterm, hw_grade[0], hw_grade[1], hw_grade[n]}"
    friend std::istream& operator>>(std::istream& is, student& obj);
    // output a student record
    friend std::ostream& operator<<(std::ostream& os, const student& obj);

private:
    int id;
    std::string name;
    int final_grade;
    int midterm;
    std::vector<int> hw_grades;
};

#endif
