#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>

using namespace std;

class Student {
public:
    int rollNumber;
    string name;
    string course;
    map<string, float> subjects; 

    void input() {
        cout << "Enter Roll Number: ";
        while (!(cin >> rollNumber) || rollNumber <= 0) {
            cin.clear(); 
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a valid roll number: ";
        }

        cin.ignore(); 
        cout << "Enter Name: ";
        getline(cin, name);

        cout << "Enter Course (BCA / B.Tech / BCom / BSc): ";
        getline(cin, course);

        map<string, vector<string>> courseSubjects = {
            {"BCA", {"Computing", "Programming", "Database"}},
            {"B.Tech", {"Data Structures", "Algorithms", "Computer Networks"}},
            {"BSc", {"Physics", "Chemistry", "Biology"}},
            {"BCom", {"Accounting", "Economics", "Business Studies"}}
        };

        if (courseSubjects.find(course) == courseSubjects.end()) {
            cout << "Invalid course! Defaulting to 'General'.\n";
            course = "General";
            subjects["General Subject"] = 0;
        } else {
            for (const string& sub : courseSubjects[course]) {
                float marks;
                cout << "Enter marks for " << sub << " (0-100): ";
                while (!(cin >> marks) || marks < 0 || marks > 100) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input! Enter marks for " << sub << " (0-100): ";
                }
                subjects[sub] = marks;
            }
        }
    }

    char getGrade(float marks) const {
        if (marks >= 90) return 'A';
        if (marks >= 75) return 'B';
        if (marks >= 60) return 'C';
        if (marks >= 40) return 'D';
        return 'F';
    }

    float calculateAverage() const {
        float total = 0;
        for (const auto& pair : subjects) 
            total += pair.second;
        return subjects.empty() ? 0 : total / subjects.size();
    }

    void showGraph() const {
        cout << "\nMarks graph for " << name << ":\n";
        for (const auto & [sub, mark] : subjects) {
            cout << sub << ": " << string(static_cast<int>(mark / 2), '*')  << " (" << mark << ")\n";
        }
    }

    void display() const {
        cout << "Roll Number: " << rollNumber
             << ", Name: " << name
             << ", Course: " << course << endl;
        for (const auto& pair : subjects) {
            cout << "  " << pair.first << ": " << pair.second << endl;
        }
        float avg = calculateAverage();
        cout << "Average Marks: " << avg << ", Overall Grade: " << getGrade(avg) << endl;
    }

    string serialize() const {
        string data = to_string(rollNumber) + "," + name + "," + course;
        for (const auto& pair : subjects) {
            data += "," + pair.first + ":" + to_string(pair.second);
        }
        return data;
    }

    static Student deserialize(const string& line) {
        Student s;
        size_t pos = 0, next;
        next = line.find(',', pos);
        s.rollNumber = stoi(line.substr(pos, next - pos));
        
        pos = next + 1;
        next = line.find(',', pos);
        s.name = line.substr(pos, next - pos);
        
        pos = next + 1;
        next = line.find(',', pos);
        s.course = line.substr(pos, next - pos);

        pos = next + 1;
        while (pos < line.size()) {
            next = line.find(',', pos);
            string token = (next == string::npos) ? line.substr(pos) : line.substr(pos, next - pos);
            size_t sep = token.find(':');
            if (sep != string::npos) {
                string sub = token.substr(0, sep);
                float marks = stof(token.substr(sep + 1));
                s.subjects[sub] = marks; 
            }
            if (next == string::npos) break;
            pos = next + 1;
        }
        return s;
    }
};

vector<Student> students;

void loadStudentsFromFile() {
    ifstream inFile("students.txt");
    if (!inFile) {
        cout << "No existing data found.\n";
        return;
    }

    string line;
    while (getline(inFile, line)) {
        if (!line.empty()) {
            Student s = Student::deserialize(line);
            students.push_back(s);
        }
    }
    inFile.close();
    cout << "Loaded " << students.size() << " students from file.\n";
}

void saveAllStudentsToFile() {
    ofstream outFile("students.txt");
    for (const auto& s : students) {
        outFile << s.serialize() << endl;
    }
    outFile.close();
    cout << "Saved " << students.size() << " students to file.\n";

}

void addStudent() {
    Student s;
    s.input();

    for (const auto& student : students) {
        if (student.rollNumber == s.rollNumber) {
            cout << "Error: Roll number " << s.rollNumber << " already exists!\n";
            return;
        }
    }

    students.push_back(s);
    cout << "Student added successfully!\n";
    saveAllStudentsToFile(); 
}

void displayAll() {
    cout << "\n--- All Students ---\n";
    if (students.empty()) {
        cout << "No students found.\n";
    }
    for (const auto& s : students) {
        s.display();
    }
}

void searchStudent() {
    int roll;
    cout << "Enter roll number to search: ";
    cin >> roll;
    while (cin.fail() || roll <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Please enter a valid roll number: ";
        cin >> roll;
    }

    bool found = false;
    for (const auto& s : students) {
        if (s.rollNumber == roll) {
            s.display();
            found = true;
            break;
        }
    }
    if (!found) cout << "Student not found!\n";
}

void deleteStudent() {
    int roll;
    cout << "Enter roll number to delete: ";
    cin >> roll;
    while (cin.fail() || roll <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Please enter a valid roll number: ";
        cin >> roll;
    }

    bool found = false;
    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->rollNumber == roll) {
            students.erase(it);
            cout << "Student deleted successfully!\n";
            saveAllStudentsToFile(); 
            found = true;
            break;
        }
    }
    if (!found) cout << "Student not found!\n";
}

void updateStudent() {
    int roll;
    cout << "Enter roll number to update: ";
    cin >> roll;
    while (cin.fail() || roll <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Please enter a valid roll number: ";
        cin >> roll;
    }

    bool found = false;
    for (auto& s : students) {
        if (s.rollNumber == roll) {
            cout << "Enter new details:\n";
            s.input();
            cout << "Student updated successfully!\n";
            saveAllStudentsToFile(); 
            found = true;
            break;
        }
    }
    if (!found) cout << "Student not found!\n";
}

void sortByRoll() {
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.rollNumber < b.rollNumber;
    });
    cout << "Students sorted by roll number.\n";
}

void sortByName() {
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.name < b.name;
    });
    cout << "Students sorted by name.\n";
}

void sortByAverage() {
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.calculateAverage() > b.calculateAverage();
    });
    cout << "Students sorted by average marks.\n";
}

void filterByCourse() {
    string courseName;
    cout << "Enter course name to filter: ";
    cin.ignore();
    getline(cin, courseName);

    bool found = false;
    for (const auto& s : students) {
        if (s.course == courseName) {
            s.display();
            found = true;
        }
    }
    if (!found) cout << "No students found in course: " << courseName << endl;
}

void searchByName() {
    string keyword;
    cout << "Enter part of the name to search: ";
    cin.ignore();
    getline(cin, keyword);

    bool found = false;
    for (const auto& s : students) {
        if (s.name.find(keyword) != string::npos) {
            s.display();
            found = true;
        }
    }
    if (!found) cout << "No student matches the name keyword.\n";
}

void courseWiseCount() {
    map<string, int> courseMap;
    for (const auto& s : students)
        courseMap[s.course]++;
    
    cout << "\n--- Course-wise Student Count ---\n";
    for (const auto& pair : courseMap)
        cout << pair.first << ": " << pair.second << endl;
}

void exportToCSV() {
    ofstream outFile("students.csv");
    if (!outFile) {
        cout << "Error creating CSV file.\n";
        return;
    }
    outFile << "Roll Number,Name,Course,Subjects (Marks)\n";
    for (const auto& s : students) {
        outFile << s.rollNumber << "," << s.name << "," << s.course;
        for (const auto& pair : s.subjects)
            outFile << "," << pair.first << ":" << pair.second;
        outFile << "\n";
    }
    outFile.close();
    cout << "Data exported to students.csv\n";
}

bool login() {
    string user, pass;
    cout << "Login Required.\n";
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;
    if (user == "admin" && pass == "1234") {
        cout << "Login successful!\n";
        return true;
    }
    cout << "Invalid credentials!\n";
    return false;
}


int main() {
    if (!login()) return 0;
    loadStudentsFromFile(); 

    int choice;
    do {
        cout << "\n--- Student Management System ---\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Delete Student\n";
        cout << "5. Update Student\n";
        cout << "6. Sort Students\n";
        cout << "7. Filter by Course\n";
        cout << "8. Search by Name\n";
        cout << "9. Course-wise Count\n";
        cout << "10. Export to CSV\n";
        cout << "11. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        while (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! please enter a valid choice: ";
            cin >> choice;
        }

        switch(choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: searchStudent(); break;
            case 4: deleteStudent(); break;
            case 5: updateStudent(); break;
            case 6: 
                int sopt;
                cout << "1. By Roll Number\n2. By Name\n3. By Average\nEnter choice: ";
                cin >> sopt;
                if (sopt == 1) sortByRoll();
                else if (sopt == 2) sortByName();
                else if (sopt == 3) sortByAverage();
                else cout << "Invalid sort option!\n";
                break;
            case 7: filterByCourse(); break;
            case 8: searchByName(); break;
            case 9: courseWiseCount(); break;
            case 10: exportToCSV(); break;
            case 11: cout << "Exiting program.\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while(choice != 11);
    
    return 0;
}
