#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>

class Student {
    int rollno;
    std::string name;
    int p_marks, c_marks, m_marks, e_marks, cs_marks;
    float per;
    char grade;

    void calculate() {
        per = (p_marks + c_marks + m_marks + e_marks + cs_marks) / 5.0;
        grade = (per >= 60) ? 'A' : (per >= 50) ? 'B' : (per >= 33) ? 'C' : 'F';
    }

public:
    void getdata() {
        std::cout << "\nEnter The roll number of student: ";
        std::cin >> rollno;
        std::cin.ignore();
        std::cout << "\nEnter The Name of student: ";
        std::getline(std::cin, name);
        std::cout << "\nEnter The marks in physics out of 100: ";
        std::cin >> p_marks;
        std::cout << "Enter The marks in chemistry out of 100: ";
        std::cin >> c_marks;
        std::cout << "Enter The marks in maths out of 100: ";
        std::cin >> m_marks;
        std::cout << "Enter The marks in english out of 100: ";
        std::cin >> e_marks;
        std::cout << "Enter The marks in computer science out of 100: ";
        std::cin >> cs_marks;
        calculate();
    }

    void showdata() const {
        std::cout << "\nRoll number of student: " << rollno;
        std::cout << "\nName of student: " << name;
        std::cout << "\nMarks in Physics: " << p_marks;
        std::cout << "\nMarks in Chemistry: " << c_marks;
        std::cout << "\nMarks in Maths: " << m_marks;
        std::cout << "\nMarks in English: " << e_marks;
        std::cout << "\nMarks in Computer Science: " << cs_marks;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\nPercentage of student: " << per;
        std::cout << "\nGrade of student: " << grade << "\n";
    }

    void show_tabular() const {
        std::cout << rollno << std::setw(12) << name << std::setw(10) 
                  << p_marks << std::setw(3) << c_marks << std::setw(3) << m_marks 
                  << std::setw(3) << e_marks << std::setw(3) << cs_marks 
                  << std::setw(6) << std::fixed << std::setprecision(2) << per << " " << grade << '\n';
    }

    int retrollno() const { return rollno; }
};

// Global variables for file operations
std::fstream fp;
Student st;

void write_student() {
    fp.open("student.dat", std::ios::out | std::ios::app);
    st.getdata();
    fp.write(reinterpret_cast<char*>(&st), sizeof(Student));
    fp.close();
    std::cout << "\n\nStudent record has been created.\n";
}

void display_all() {
    std::cout << "\n\n\t\tDISPLAY ALL RECORDS !!!\n\n";
    fp.open("student.dat", std::ios::in);
    if (!fp) {
        std::cout << "ERROR!!! FILE COULD NOT BE OPEN\n\n";
        return;
    }
    while (fp.read(reinterpret_cast<char*>(&st), sizeof(Student))) {
        st.showdata();
        std::cout << "\n====================================\n";
    }
    fp.close();
}

void display_sp(int n) {
    bool found = false;
    fp.open("student.dat", std::ios::in);
    while (fp.read(reinterpret_cast<char*>(&st), sizeof(Student))) {
        if (st.retrollno() == n) {
            st.showdata();
            found = true;
            break;
        }
    }
    fp.close();
    if (!found) std::cout << "\n\nRecord does not exist\n";
}

void modify_student() {
    int no;
    bool found = false;
    std::cout << "\n\nPlease Enter The roll number of student: ";
    std::cin >> no;
    fp.open("student.dat", std::ios::in | std::ios::out);
    while (fp.read(reinterpret_cast<char*>(&st), sizeof(Student)) && !found) {
        if (st.retrollno() == no) {
            st.showdata();
            std::cout << "\nPlease Enter The New Details of student:\n";
            st.getdata();
            int pos = -1 * static_cast<int>(sizeof(st));
            fp.seekp(pos, std::ios::cur);
            fp.write(reinterpret_cast<char*>(&st), sizeof(Student));
            std::cout << "\n\n\t Record Updated\n";
            found = true;
        }
    }
    fp.close();
    if (!found) std::cout << "\n\n Record Not Found\n";
}

void delete_student() {
    int no;
    std::cout << "\n\nPlease Enter The roll number of student you want to delete: ";
    std::cin >> no;
    fp.open("student.dat", std::ios::in | std::ios::out);
    std::fstream fp2;
    fp2.open("Temp.dat", std::ios::out);
    while (fp.read(reinterpret_cast<char*>(&st), sizeof(Student))) {
        if (st.retrollno() != no) {
            fp2.write(reinterpret_cast<char*>(&st), sizeof(Student));
        }
    }
    fp2.close();
    fp.close();
    std::remove("student.dat");
    std::rename("Temp.dat", "student.dat");
    std::cout << "\n\n\tRecord Deleted\n";
}

void class_result() {
    std::cout << "\n\n\t\tALL STUDENTS RESULT\n\n";
    std::cout << "====================================================\n";
    std::cout << "Roll No. Name P C M E CS %age Grade\n";
    std::cout << "====================================================\n";
    fp.open("student.dat", std::ios::in);
    while (fp.read(reinterpret_cast<char*>(&st), sizeof(Student))) {
        st.show_tabular();
    }
    fp.close();
}

int main() {
    char ch;
    do {
        std::cout << "\n\n\n\tMAIN MENU";
        std::cout << "\n\n\t01. RESULT MENU";
        std::cout << "\n\n\t02. ENTRY/EDIT MENU";
        std::cout << "\n\n\t03. EXIT";
        std::cout << "\n\n\tPlease Select Your Option (1-3): ";
        std::cin >> ch;
        switch (ch) {
        case '1':
            class_result();
            break;
        case '2':
            write_student();
            break;
        case '3':
            std::exit(0);
        default:
            std::cout << "\a";
        }
    } while (ch != '3');
    return 0;
}
