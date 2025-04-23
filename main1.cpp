#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include <limits>
#include <fstream>
#include <algorithm>
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_LEFT 68
#define KEY_RIGHT 67
#define KEY_ENTER 10
int getch() {
    termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
using namespace std;
class Admin {
private:
    string _username;
    string _password;

public:
    Admin(string username, string password) {
        set_username(username);
        set_password(password);
    }
    void set_username(string username) {
        _username=username;
    }
    void set_password(string password) {
        _password=password;
    }
    string getUsername() const {
        return _username;
    }
    string getPassword() const {
        return _password;
    }
    bool checkPassword(const string& password) const {
        return _password == password;
    }
};
class AdminManager {
private:
    vector<Admin> admins;
    Admin* currentAdmin = nullptr;
public:
    bool signUp(const string& username, const string& password) {
        for (const auto& admin : admins) {
            if (admin.getUsername() == username) {
                cout << "Username already exists.\n";
                return false;
            }
        }
        admins.push_back(Admin(username, password));
        cout << "Sign up successful.\n";
        return true;
    }

    bool signIn(const string& username, const string& password) {
        for (auto& admin : admins) {
            if (admin.getUsername() == username && admin.checkPassword(password)) {
                currentAdmin = &admin;
                cout << "Sign in successful. Welcome, " << username << "!\n";
                return true;
            }
        }
        cout << "Incorrect username or password.\n";
        return false;
    }
    bool isLoggedIn() const {
        return currentAdmin != nullptr;
    }
};
AdminManager adminManager;
void showLoginMenu() {
    while (true) {
        system("clear");
        cout << "Admin Panel Login\n";
        cout << "1. Sign Up\n";
        cout << "2. Sign In\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        int option;
        cin >> option;
        cin.ignore();
        string username, password;
        switch (option) {
            case 1:
                cout << "Username: ";
                getline(cin, username);
                cout << "Password: ";
                getline(cin, password);
                adminManager.signUp(username, password);
                break;
            case 2:
                cout << "Username: ";
                getline(cin, username);
                cout << "Password: ";
                getline(cin, password);
                if (adminManager.signIn(username, password)) return;
                break;
            case 3:
                return;
        }
        cout << "\nPress Enter to continue";
        cin.get();
    }
}

class Task {
private:
    string _title;
    string _description;
    bool _completed;
    int _id;
    static int staticId;

public:
    Task(string title, string description, bool completed) {
        _id = staticId++;
        setTitle(title);
        setDescription(description);
        setCompleted(completed);
    }

    static void resetId() {
        staticId = 1;
    }

    void setTitle(string title) {
        _title = title;
    }

    void setDescription(string description) {
        _description = description;
    }

    void setCompleted(bool completed) {
        _completed = completed;
    }

    int getId() const {
        return _id;
    }

    string getTitle() const {
        return _title;
    }

    string getDescription() const {
        return _description;
    }

    bool getCompleted() const {
        return _completed;
    }

    void print() const {
        cout << "ID: " << getId() << endl;
        cout << "Title: " << getTitle() << endl;
        cout << "Description: " << getDescription() << endl;
        cout << "Status: " << (getCompleted() ? "Completed" : "Not Completed") << endl;
    }
};

int Task::staticId = 1;

class ToDoList {
private:
    vector<Task> tasks;
    int selectedOption;

public:
    ToDoList() : selectedOption(0) {}

    void addTask(string title, string description, bool completed) {
        tasks.push_back(Task(title, description, completed));
    }
    void TaskStatusById() {
        if (tasks.empty()) {
            cout << "No tasks available to update.\n";
            return;
        }
        cout << "Available Tasks:\n";
        for (const auto& task : tasks) {
            cout << "ID: " << task.getId()
                 << " Title: " << task.getTitle()
                 << " Description: " << task.getDescription()
                 << " Status: " << (task.getCompleted() ? "Completed" : "Not Completed")
                 << endl;
        }
        int id;
        cout << "Enter the ID to choose status: ";
        cin >> id;
        cin.ignore();
        for (auto& task : tasks) {
            if (task.getId() == id) {
                bool newStatus = !task.getCompleted();
                task.setCompleted(newStatus);
                cout << "Task status updated to: " << (newStatus ? "Completed" : "Not Completed") << endl;
                return;
            }
        }
        cout << "Task with ID " << id << " not found.\n";
    }

    void deleteTaskById(int id) {
        for (auto it = tasks.begin(); it != tasks.end(); ++it) {
          if (it->getId() == id) {
            tasks.erase(it);
            cout << "Task with ID " << id << " deleted.\n";
            return;
          }
        }
        cout << "Task with ID " << id << " not found.\n";
      }

    void showTasks() const {
        if (tasks.empty()) {
            cout << "No tasks available.\n";
            return;
        }
        for (const auto& task : tasks) {
            task.print();
        }
    }

    void filterTasks() {
        int filterChoice;
        cout << "1. Title\n";
        cout << "2. Description\n";
        cout << "3. Completed Status\n";
        cout << "4. ID\n";
        cout << "Enter your choice: ";
        cin >> filterChoice;
        cin.ignore();
        bool found = false;
        switch (filterChoice) {
            case 1: {
                cout << "Available titles:\n";
                for (const auto& task : tasks) {
                    cout << "- " << task.getTitle() << endl;
                }
                string title;
                cout << "Enter title to filter: ";
                getline(cin, title);
                for (const auto& task : tasks) {
                    if (task.getTitle() == title) {
                        task.print();
                        found = true;
                    }
                }
                break;
            }
            case 2: {
                cout << "Available descriptions:\n";
                for (const auto& task : tasks) {
                    cout << "- " << task.getDescription() << endl;
                }
                string description;
                cout << "Enter description to filter: ";
                getline(cin, description);
                for (const auto& task : tasks) {
                    if (task.getDescription() == description) {
                        task.print();
                        found = true;
                    }
                }
                break;
            }
            case 3: {
                cout << "1. Completed\n2. Not Completed\n";
                cout << "Enter status choice: ";
                int statusChoice;
                cin >> statusChoice;
                bool status = (statusChoice == 1);
                for (const auto& task : tasks) {
                    if (task.getCompleted() == status) {
                        task.print();
                        found = true;
                    }
                }
                break;
            }
            case 4: {
                cout << "Available IDs:\n";
                for (const auto& task : tasks) {
                    cout << "- " << task.getId() << endl;
                }
                int id;
                cout << "Enter ID to filter: ";
                cin >> id;
                for (const auto& task : tasks) {
                    if (task.getId() == id) {
                        task.print();
                        found = true;
                    }
                }
                break;
            }
            default:
                cout << "Invalid option!" << endl;
                return;
        }

        if (!found) {
            cout << "No matching tasks found." << endl;
        }
    }

    void updateTaskById() {
        if (tasks.empty()) {
            cout << "No tasks available to update.\n";
            return;
        }
        cout << "Available Tasks:\n";
        for (const auto& task : tasks) {
            cout << "ID: " << task.getId()
                 << ", Title: " << task.getTitle()
                 << endl;
        }
        int id;
        cout << "Enter the ID to update: ";
        cin >> id;
        cin.ignore();
        for (auto& task : tasks) {
            if (task.getId() == id) {
                string newTitle, newDescription;
                int completedChoice;
                cout << "Enter new title (leave empty to keep current): ";
                getline(cin, newTitle);
                if (!newTitle.empty()) {
                    task.setTitle(newTitle);
                }
                cout << "Enter new description (leave empty to keep current): ";
                getline(cin, newDescription);
                if (!newDescription.empty()) {
                    task.setDescription(newDescription);
                }
                cout << "Update completion status?\n1. Yes\n2. No\nChoice: ";
                cin >> completedChoice;
                if (completedChoice == 1) {
                    int status;
                    cout << "1. Completed\n2. Not Completed\nChoice: ";
                    cin >> status;
                    task.setCompleted(status == 1);
                }
                cout << "Task updated successfully.\n";
                return;
            }
        }
        cout << "Task with ID " << id << " not found.\n";
    }

    void showTasksWithNavigation() {
        if (tasks.empty()) {
            cout << "No tasks available.\n";
            return;
        }

        int selection = 0;
        while (true) {
            system("clear");
            cout << "Available Tasks:\n";
            for (int i = 0; i < tasks.size(); ++i) {
                if (i == selection) {
                    cout << "\033[32m > ";
                } else {
                    cout << "   ";
                }
                tasks[i].print();
                cout << endl;
            }
            int key = getch();
            if (key == KEY_UP && selection > 0) {
                selection--;
            } else if (key == KEY_DOWN && selection < tasks.size() - 1) {
                selection++;
            } else if (key == KEY_ENTER) {
                tasks[selection].print();
                break;
            }
        }
    }
};

int main() {
    showLoginMenu();
    ToDoList manager;
    int secim = 0;
    while (true) {
        system("clear");
        cout << "Task Manager Menu:\n\n";
        string options[] = {
            "1. Show Tasks",
            "2. Add Task",
            "3. Update Task",
            "4. Delete Task",
            "5. Filter Tasks",
            "6. Toggle Task Status",
            "7. Exit"
        };
        for (int i = 0; i < 7; ++i) {
            if (i == secim)
                cout << "\033[32m > " << options[i] << "\033[0m" << endl;
            else
                cout << "   " << options[i] << endl;
        }
        int c = getch();
        if (c == 27) {
            int c1 = getch();
            int c2 = getch();
            if (c1 == 91) {
                switch (c2) {
                    case KEY_UP:
                        secim = (secim > 0) ? secim - 1 : 6;
                        break;
                    case KEY_DOWN:
                        secim = (secim < 6) ? secim + 1 : 0;
                        break;
                }
            }
        }
        if (c == KEY_ENTER) {
            switch (secim) {
                case 0:
                    manager.showTasks();
                    break;
                case 1: {
                    string title, description;
                    cout << "Enter title: ";
                    getline(cin, title);
                    cout << "Enter description: ";
                    getline(cin, description);
                    manager.addTask(title, description, false);
                    break;
                }
                case 2:
                    manager.updateTaskById();
                    break;
                case 3: {
                    int id;
                    cout << "Enter task ID to delete: ";
                    cin >> id;
                    cin.ignore();
                    manager.deleteTaskById(id);
                    break;
                }
                case 4:
                    manager.filterTasks();
                    break;
                case 5:
                    manager.TaskStatusById();
                    break;
                case 6:
                    cout << "Exiting\n";
                        return 0;
                    }
                    cout << "\nPress Enter to continue";
                    cin.ignore();
                    cin.get();
                }
        }
    }
