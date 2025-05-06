#include "json.hpp"
#include"task.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <termios.h>
#include <unistd.h>
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_LEFT 68
#define KEY_RIGHT 67
#define KEY_ENTER 10
using namespace std;
using json = nlohmann::json;
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