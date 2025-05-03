#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include <limits>
#include <fstream>
#include <algorithm>
#include<string>
#include <ctime>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;
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
class Task {
private:
    string _title;
    string _description;
    bool _completed;
    int _id;
    static int staticId;
    string datetime;

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
    void setId(int id) {
        _id = id;
        if (id >= staticId) {
            staticId = id + 1;
        }
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
    void setDatetime(const string& dt) {
        datetime = dt;
    }
    

    static string getDateTime() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char buffer[30];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
        return string(buffer);
    }
    void print() const {
        cout << "ID: " << getId() << endl;
        cout << "Title: " << getTitle() << endl;
        cout << "Description: " << getDescription() << endl;
        cout << "Status: " << (getCompleted() ? "Completed" : "Not Completed")<<endl;
        cout<<"\nDate & Time: " << datetime << endl;
    }
};

int Task::staticId = 1;

class ToDoList {
private:
    vector<Task> tasks;
    int selectedOption;
    string username; 

public:
    void setUsername(const string& user) {
        username = user;
    }
    string getFilename() const {
        return username + "_tasks.json";
    }
    ToDoList() : selectedOption(0) {}

    void addTask(string title, string description, bool completed) {
        tasks.push_back(Task(title, description, completed));
    }
    void TaskStatusById() {
        if (tasks.empty()) {
            cout << "No tasks available to update.\n";
            cout << "\nPress Enter to continue";
            cin.ignore();
            cin.get();
            return;
        }
        int selection = 0;
        while (true) {
            system("clear");
            cout << "Select a task to toggle status:\n\n";
            for (int i = 0; i < tasks.size(); ++i) {
                const auto& task = tasks[i];
                string status = task.getCompleted() ? "Completed" : "Not Completed";
                if (i == selection)
                    cout << "\033[32m > ID: " << task.getId() << " -- " << task.getTitle() << " (" << status << ")\033[0m\n";
                else
                    cout << "   ID: " << task.getId() << " -- " << task.getTitle() << " (" << status << ")\n";
            }
    
            int c = getch();
            if (c == 27) { 
                int c1 = getch();
                int c2 = getch();
                if (c1 == 91) {
                    switch (c2) {
                        case KEY_UP:
                            selection = (selection > 0) ? selection - 1 : tasks.size() - 1;
                            break;
                        case KEY_DOWN:
                            selection = (selection < tasks.size() - 1) ? selection + 1 : 0;
                            break;
                    }
                }
            } else if (c == KEY_ENTER) {
                auto& task = tasks[selection];
                bool newStatus = !task.getCompleted();
                task.setCompleted(newStatus);
                cout << "Task status updated to: " << (newStatus ? "Completed" : "Not Completed") << endl;
                cout << "\nPress Enter to continue";
                cin.ignore();
                cin.get();
                return;
            }
        }
    }
    
    void deleteTaskById() {
        if (tasks.empty()) {
            cout << "No tasks to delete.\n";
            cout << "\nPress Enter to continue";
            cin.ignore();
            cin.get();
            return;
        }
        int selection = 0;
        while (true) {
            system("clear");
            cout << "Select a task to delete:\n\n";
    
            for (int i = 0; i < tasks.size(); ++i) {
                if (i == selection)
                    cout << "\033[32m > ID: " << tasks[i].getId() << " -- " << tasks[i].getTitle() << "\033[0m\n";
                else
                    cout << "   ID: " << tasks[i].getId() << " -- " << tasks[i].getTitle() << "\n";
            }
            int c = getch();
            if (c == 27) { 
                int c1 = getch();
                int c2 = getch();
                if (c1 == 91) {
                    switch (c2) {
                        case KEY_UP:
                            selection = (selection > 0) ? selection - 1 : tasks.size() - 1;
                            break;
                        case KEY_DOWN:
                            selection = (selection < tasks.size() - 1) ? selection + 1 : 0;
                            break;
                    }
                }
            } else if (c == KEY_ENTER) {
                int id = tasks[selection].getId();
                tasks.erase(tasks.begin() + selection);
                cout << "Task with ID " << id << " deleted.\n";
                cout << "\nPress Enter to continue";
                cin.ignore();
                cin.get();
                return;
            }
        }
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
        int selection = 0;
        string options[] = {
            "Filter by Title",
            "Filter by Description",
            "Filter by Completion Status",
            "Filter by ID",
            "Exit"
        };
        while (true) {
            system("clear");
            cout << "Filter Tasks Menu:\n\n";
            for (int i = 0; i < 5; ++i) {
                if (i == selection)
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
                            selection = (selection > 0) ? selection - 1 : 4;
                            break;
                        case KEY_DOWN:
                            selection = (selection < 4) ? selection + 1 : 0;
                            break;
                    }
                }
            } else if (c == KEY_ENTER) {
                system("clear");
                bool found = false;
    
                switch (selection) {
                    case 0: {
                        vector<string> titles;
                        for (const auto& task : tasks)
                            titles.push_back(task.getTitle());
    
                        int titleIndex = 0;
                        while (true) {
                            system("clear");
                            cout << "Select a Title to filter:\n\n";
                            for (int i = 0; i < titles.size(); ++i) {
                                if (i == titleIndex)
                                    cout << "\033[32m > " << titles[i] << "\033[0m\n";
                                else
                                    cout << "   " << titles[i] << "\n";
                            }
    
                            int ch = getch();
                            if (ch == 27) {
                                int c1 = getch(), c2 = getch();
                                if (c1 == 91) {
                                    if (c2 == KEY_UP) titleIndex = (titleIndex > 0) ? titleIndex - 1 : titles.size() - 1;
                                    if (c2 == KEY_DOWN) titleIndex = (titleIndex + 1) % titles.size();
                                }
                            } else if (ch == KEY_ENTER) {
                                string selectedTitle = titles[titleIndex];
                                for (const auto& task : tasks) {
                                    if (task.getTitle() == selectedTitle) {
                                        task.print();
                                        found = true;
                                    }
                                }
                                break;
                            }
                        }
                        break;
                    }
                    case 1: {
                        vector<string> descriptions;
                        for (const auto& task : tasks)
                            descriptions.push_back(task.getDescription());
    
                        int descIndex = 0;
                        while (true) {
                            system("clear");
                            cout << "Select a Description to filter:\n\n";
                            for (int i = 0; i < descriptions.size(); ++i) {
                                if (i == descIndex)
                                    cout << "\033[32m > " << descriptions[i] << "\033[0m\n";
                                else
                                    cout << "   " << descriptions[i] << "\n";
                            }
    
                            int ch = getch();
                            if (ch == 27) {
                                int c1 = getch(), c2 = getch();
                                if (c1 == 91) {
                                    if (c2 == KEY_UP) descIndex = (descIndex > 0) ? descIndex - 1 : descriptions.size() - 1;
                                    if (c2 == KEY_DOWN) descIndex = (descIndex + 1) % descriptions.size();
                                }
                            } else if (ch == KEY_ENTER) {
                                string selectedDesc = descriptions[descIndex];
                                for (const auto& task : tasks) {
                                    if (task.getDescription() == selectedDesc) {
                                        task.print();
                                        found = true;
                                    }
                                }
                                break;
                            }
                        }
                        break;
                    }
                    case 2: {
                        int statusSel = 0;
                        string statuses[] = { "Completed", "Not Completed" };
    
                        while (true) {
                            system("clear");
                            cout << "Select Status:\n\n";
                            for (int i = 0; i < 2; ++i) {
                                if (i == statusSel)
                                    cout << "\033[32m > " << statuses[i] << "\033[0m\n";
                                else
                                    cout << "   " << statuses[i] << "\n";
                            }
    
                            int ch = getch();
                            if (ch == 27) {
                                int c1 = getch(), c2 = getch();
                                if (c1 == 91) {
                                    if (c2 == KEY_UP) statusSel = (statusSel > 0) ? statusSel - 1 : 1;
                                    if (c2 == KEY_DOWN) statusSel = (statusSel + 1) % 2;
                                }
                            } else if (ch == KEY_ENTER) {
                                bool completed = (statusSel == 0);
                                for (const auto& task : tasks) {
                                    if (task.getCompleted() == completed) {
                                        task.print();
                                        found = true;
                                    }
                                }
                                break;
                            }
                        }
                        break;
                    }
                    case 3: { 
                        vector<int> ids;
                        for (const auto& task : tasks)
                            ids.push_back(task.getId());
    
                        int idIndex = 0;
                        while (true) {
                            system("clear");
                            cout << "Select an ID to filter:\n\n";
                            for (int i = 0; i < ids.size(); ++i) {
                                if (i == idIndex)
                                    cout << "\033[32m > ID: " << ids[i] << "\033[0m\n";
                                else
                                    cout << "   ID: " << ids[i] << "\n";
                            }
    
                            int ch = getch();
                            if (ch == 27) {
                                int c1 = getch(), c2 = getch();
                                if (c1 == 91) {
                                    if (c2 == KEY_UP) idIndex = (idIndex > 0) ? idIndex - 1 : ids.size() - 1;
                                    if (c2 == KEY_DOWN) idIndex = (idIndex + 1) % ids.size();
                                }
                            } else if (ch == KEY_ENTER) {
                                int selectedId = ids[idIndex];
                                for (const auto& task : tasks) {
                                    if (task.getId() == selectedId) {
                                        task.print();
                                        found = true;
                                    }
                                }
                                break;
                            }
                        }
                        break;
                    }
                    case 4:
                        return;
                }
    
                if (!found && selection != 4) {
                    cout << "\nNo matching tasks found.\n";
                }
    
                cout << "\nPress Enter to continue";
                cin.ignore();
                cin.get();
            }
        }
    }    
    void updateTaskById() {
        if (tasks.empty()) {
            cout << "No tasks available to update.\n";
            cout << "\nPress Enter to continue";
            cin.ignore();
            cin.get();
            return;
        }
    
        int selection = 0;
    
        while (true) {
            system("clear");
            cout << "Select a task to update:\n\n";
    
            for (int i = 0; i < tasks.size(); ++i) {
                if (i == selection)
                    cout << "\033[32m > ID: " << tasks[i].getId() << " -- " << tasks[i].getTitle() << "\033[0m\n";
                else
                    cout << "  ID: " << tasks[i].getId() << " --" << tasks[i].getTitle() << "\n";
            }
    
            int c = getch();
            if (c == 27) {
                int c1 = getch();
                int c2 = getch();
                if (c1 == 91) {
                    switch (c2) {
                        case KEY_UP:
                            selection = (selection > 0) ? selection - 1 : tasks.size() - 1;
                            break;
                        case KEY_DOWN:
                            selection = (selection < tasks.size() - 1) ? selection + 1 : 0;
                            break;
                    }
                }
            } else if (c == KEY_ENTER) {
                system("clear");
                auto& task = tasks[selection];
    
                string newTitle, newDescription;
                int updateStatus = 0;
    
                cout << "Updating Task ID: " << task.getId() << "\n";
                cout << "Current Title: " << task.getTitle() << "\n";
                cout << "Enter new title (leave empty to keep current): ";
                cin.ignore();
                getline(cin, newTitle);
                if (!newTitle.empty()) {
                    task.setTitle(newTitle);
                }
    
                cout << "\nCurrent Description: " << task.getDescription() << "\n";
                cout << "Enter new description (leave empty to keep current): ";
                getline(cin, newDescription);
                if (!newDescription.empty()) {
                    task.setDescription(newDescription);
                }
                cout << "\nDo you want to update the status?\n";
                string choices[] = { "Yes", "No" };
                int choice = 0;
    
                while (true) {
                    system("clear");
                    cout << "Do you want to update the status?\n\n";
                    for (int i = 0; i < 2; ++i) {
                        if (i == choice)
                            cout << "\033[32m > " << choices[i] << "\033[0m\n";
                        else
                            cout << "   " << choices[i] << "\n";
                    }
    
                    int ch = getch();
                    if (ch == 27) {
                        int c1 = getch(), c2 = getch();
                        if (c1 == 91) {
                            if (c2 == KEY_UP)
                                choice = (choice > 0) ? choice - 1 : 1;
                            if (c2 == KEY_DOWN)
                                choice = (choice + 1) % 2;
                        }
                    } else if (ch == KEY_ENTER) {
                        if (choice == 0) {
                            int statusSel = 0;
                            string statusOptions[] = { "Completed", "Not Completed" };
                            while (true) {
                                system("clear");
                                cout << "Select new status:\n\n";
                                for (int i = 0; i < 2; ++i) {
                                    if (i == statusSel)
                                        cout << "\033[32m > " << statusOptions[i] << "\033[0m\n";
                                    else
                                        cout << "   " << statusOptions[i] << "\n";
                                }
    
                                int k = getch();
                                if (k == 27) {
                                    int k1 = getch(), k2 = getch();
                                    if (k1 == 91) {
                                        if (k2 == KEY_UP)
                                            statusSel = (statusSel > 0) ? statusSel - 1 : 1;
                                        if (k2 == KEY_DOWN)
                                            statusSel = (statusSel + 1) % 2;
                                    }
                                } else if (k == KEY_ENTER) {
                                    task.setCompleted(statusSel == 0);
                                    break;
                                }
                            }
                        }
                        break;
                    }
                }
    
                cout << "\nTask updated successfully.\n";
                cout << "\nPress Enter to continue";
                cin.get();
                return;
            }
        }
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
    void loadTasksFromFile() {
        ifstream inputFile(getFilename());
        if (inputFile.is_open()) {
            json j;
            inputFile >> j;
            tasks.clear(); 
    
            int maxId = 0;
    
            for (const auto& taskJson : j) {
                string title = taskJson["title"];
                string description = taskJson["description"];
                bool completed = taskJson["completed"];
                int id = taskJson["id"];
                string datetime = taskJson.value("datetime", Task::getDateTime());
            
                Task task(title, description, completed);
                task.setId(id);
                task.setDatetime(datetime);  
                tasks.push_back(task);
            
                if (id > maxId) maxId = id;
            }
            
    
            Task::resetId();              
            Task temp("", "", false);
            temp.setId(maxId);            
            inputFile.close();
        } else {
            cout << "No previous tasks found for " << username << ".\n";
        }
    }    
    
    void saveTasksToFile() const {
        if (tasks.empty()) {
            cout << "No tasks to save for user: " << username << endl;
            return;
        }

        json j = json::array();
    
        for (const auto& task : tasks) {
            json taskJson;
            taskJson["datetime"] = task.getDateTime();
            taskJson["id"] = task.getId();
            taskJson["title"] = task.getTitle();
            taskJson["description"] = task.getDescription();
            taskJson["completed"] = task.getCompleted();
    
            j.push_back(taskJson);
        }
        ofstream outputFile(getFilename());
        if (outputFile) {
            outputFile << j.dump(4); 
            outputFile.close();
            cout << "Tasks saved successfully to file: " << getFilename() << endl;
        } else {
            cerr << "Error: Couldn't open file to save tasks: " << getFilename() << endl;
        }
    }
};
class Admin {
    private:
        string _username;
        string _password;
        ToDoList _todoList;
    
    public:
        Admin(string username, string password) {
            set_username(username);
            set_password(password);
            _todoList.setUsername(username); 
        }
    
        void set_username(string username) {
            _username = username;
        }
    
        void set_password(const string& password) {
            _password = password;
        }
        string getHiddenPassword() {
            string password;
            char ch;
            while (true) {
                ch = getch();  
                if (ch == '\n' || ch == '\r') { 
                    break;
                } else if (ch == 127 || ch == '\b') { 
                    if (!password.empty()) {
                        password.pop_back();
                        cout << "\b \b";  
                    }
                } else {
                    password += ch;
                    cout << "*";  
                }
            }
            cout << endl;
            return password;
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
    
        ToDoList& getToDoList() {
            return _todoList;
        }
    };
    
    class AdminManager {
        private:
            vector<Admin> admins;
            Admin* currentAdmin = nullptr;
            
        public:
            void saveAdminsToFile() const {
                json j = json::array();
                for (const auto& admin : admins) {
                    json adminJson;
                    adminJson["username"] = admin.getUsername();
                    adminJson["password"] = admin.getPassword();
                    j.push_back(adminJson);
                }
        
                ofstream file("admins.json", ios::out);
                if (!file.is_open()) {
                    cerr << "Error: Couldn't open the file to save admins.\n";
                    return;
                }
        
                file << j.dump(4);
                file.close();
            }    
        
            void loadAdminsFromFile() {
                ifstream file("admins.json");
                if (!file.is_open()) {
                    cout << "No admin file found, no admins loaded.\n";
                    return;
                }
                json j;
                file >> j;
                file.close();
        
                admins.clear();
                for (const auto& adminJson : j) {
                    string username = adminJson["username"];
                    string password = adminJson["password"];
                    admins.emplace_back(username, password);
                }
            }
        
            bool signUp(const string& username, const string& password) {
                for (const auto& admin : admins) {
                    if (admin.getUsername() == username) {
                        cout << "Username already exists.\n";
                        return false;
                    }
                }
                Admin newAdmin(username, password);
                admins.push_back(newAdmin);
                currentAdmin = &admins.back();
                currentAdmin->getToDoList().saveTasksToFile();  
                saveAdminsToFile();
        
                cout << "Sign up successful.\n";
                return true;
            }
        
            bool signIn(const string& username, const string& password) {
                for (auto& admin : admins) {
                    if (admin.getUsername() == username && admin.checkPassword(password)) {
                        currentAdmin = &admin;
                        cout << "Sign in successful. Welcome, " << username << "!\n";
                        currentAdmin->getToDoList().loadTasksFromFile();  
                        return true;
                    }
                }
                cout << "Incorrect username or password.\n";
                return false;
            }
        
            bool isLoggedIn() const {
                return currentAdmin != nullptr;
            }
        
            ToDoList& getCurrentAdminToDoList() {
                return currentAdmin->getToDoList();
            }
        };
        
    AdminManager adminManager;
    
    bool showLoginMenu() {
        int selection = 0;
        string options[] = {
            "1. Sign Up",
            "2. Sign In",
            "3. Exit"
        };
    
        while (true) {
            system("clear");
            cout<<"\033[33m" <<"_____   ____________                             ______"<<endl;
            cout<<"\033[33m" <<"___  | / /__(_)__  /  ______________ _______________  /"<<endl;
            cout<<"\033[33m" <<"__   |/ /__  /__  /   ___  __ \\  __ `/_  __ \\  _ \\_  / "<<endl;
            cout<<"\033[33m" <<"_  /|  / _  / _  /    __  /_/ / /_/ /_  / / /  __/  /  "<<endl;
            cout<<"\033[33m" <<"/_/ |_/  /_/  /_/     _  .___/\\__,_/ /_/ /_/\\___//_/   "<<endl;
            cout<<"\033[33m" <<"                      /_/                              "<<endl;
            for (int i = 0; i < 3; ++i) {
                if (i == selection) {
                    cout << "\033[32m > " << options[i] << "\033[0m" << endl;
                } else {
                    cout << "   " << options[i] << endl;
                }
            }
    
            int c = getch();
            if (c == 27) {
                int c1 = getch();
                int c2 = getch();
                if (c1 == 91) {
                    switch (c2) {
                        case KEY_UP:
                            selection = (selection > 0) ? selection - 1 : 2;
                            break;
                        case KEY_DOWN:
                            selection = (selection < 2) ? selection + 1 : 0;
                            break;
                    }
                }
            }
    
            if (c == KEY_ENTER) {
                string username, password;
                Admin tempAdmin("", "");
                switch (selection) {
                    case 0: {
                        cout << "Username: ";
                        getline(cin, username);
                        cout << "Password: ";
                        password = tempAdmin.getHiddenPassword(); 
                        if (adminManager.signUp(username, password)) {
                            return true;
                        }
                        break;
                    }
                    case 1: {
                        cout << "Username: ";
                        getline(cin, username);
                        cout << "Password: ";
                        password = tempAdmin.getHiddenPassword(); 
                        if (adminManager.signIn(username, password)) {
                            return true;
                        }
                        break;
                    }
                    case 2:
                        return false;
                }
                cout << "\nPress Enter to continue.";
                cin.get();
            }
        }
    }
int main() {
    adminManager.loadAdminsFromFile();
    adminManager.signUp("Nili", "hacil_fe44");
    adminManager.signIn("Nili", "hacil_fe44");
    adminManager.getCurrentAdminToDoList().addTask("Shopping", "Coxlu coxlu paltar al", false);
    adminManager.getCurrentAdminToDoList().addTask("Sakitliyi pozma", "Seidnen dalasma", true);

    adminManager.signUp("Seidizmo", "Nil2701");
    adminManager.signIn("Seidizmo", "Nil2701");
    adminManager.getCurrentAdminToDoList().addTask("Sevgilim", "Sevgilimin eseblerin pozmuyum", false);
    adminManager.getCurrentAdminToDoList().addTask("Ingilis", "Hec olmasa 5 soz oyrene bilim", false);



    if (!showLoginMenu()) {
        return 0;
    }

    ToDoList& manager = adminManager.getCurrentAdminToDoList();
    manager.loadTasksFromFile();

    int secim = 0;
    while (true) {
        system("clear");
        cout<<"\033[34m"<< " .-') _                                            ('-.  _  .-')         .-') _                       _ .-') _                                         .-')    .-') _    "<<endl;
        cout<<"\033[34m"<<"    ( OO ) )                                         _(  OO)( ( -O )       (  OO) )                     ( (  OO) )                                       ( OO ). (  OO) )   "<<endl;
        cout<<"\033[34m"<<",--./ ,--,' ,-.-')  ,--.     ,--. ,--.      ,------.(,------.,------.       /     '._  .-'),-----.        \\     .'_  .-'),-----.        ,--.      ,-.-') (_)---\\_)/     '._  "<<endl;
        cout<<"\033[34m"<<"|   \\ |  |\\ |  |OO) |  |.-') |  | |  |   ('-| _.---' |  .---'|   /`. '      |'--...__)( OO'  .-.  '       ,`'--..._)( OO'  .-.  '       |  |.-')  |  |OO)/    _ | |'--...__) "<<endl;
        cout<<"\033[34m"<<"|    \\|  | )|  |  \\ |  | OO )|  | | .-') (OO|(_\\     |  |    |  /  | |      '--.  .--'/   |  | |  |       |  |  \\  '/   |  | |  |       |  | OO ) |  |  \\  :` `. '--.  .--' "<<endl;
        cout<<"\033[34m"<<"|  .     |/ |  |(_/ |  |`-' ||  |_|( OO )/  |  '--. (|  '--. |  |_.' |         |  |   \\_) |  |\\|  |       |  |   ' |\\_) |  |\\|  |       |  |`-' | |  |(_/ '..`''.)   |  |    "<<endl;
        cout<<"\033[34m"<<"|  |\\    | ,|  |_.'(|  '---.'|  | | `-' /\\_)|  .--'  |  .--' |  .  '.'         |  |     \\ |  | |  |       |  |   / :  \\ |  | |  |      (|  '---.',|  |_.'.-._)   \\   |  |    "<<endl;
        cout<<"\033[34m"<<"|  | \\   |(_|  |    |      |('  '-'(_.-'   \\|  |_)   |  `---.|  |\\  \\          |  |      `'  '-'  '       |  '--'  /   `'  '-'  '       |      |(_|  |   \\       /   |  |    "<<endl;
        cout<<"\033[34m"<<"`--'  `--'  `--'    `------'  `-----'       `--'     `------'`--' '--'         `--'        `-----'        `-------'      `-----'        `------'  `--'    `-----'    `--'    "<<endl;
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
                    manager.deleteTaskById();
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
                    manager.saveTasksToFile();
                    return 0;
            }
            cout << "\nPress Enter to continue";
            cin.ignore();
            cin.get();
        }
    }
}
