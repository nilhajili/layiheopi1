#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>
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
           << ", Description: " << task.getDescription()
           << ", Status: " << (task.getCompleted() ? "Completed" : "Not Completed")
           << endl;
    }
    int id;
    cout << "Enter the ID of the task you want to update: ";
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
};
int main() {
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
      "6. Exit"
  };
    for (int i = 0; i < 6; ++i) {
      if (i == secim)
        cout << " > " << options[i] << endl;
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
            secim = (secim > 0) ? secim - 1 : 5;
          break;
          case KEY_DOWN:
            secim = (secim < 5) ? secim + 1 : 0;
          break;
        }
      }
    } else if (c == '\n' || c == '\r') {
      system("clear");
      switch (secim) {
        case 0:
          manager.showTasks();
        break;
        case 1: {
          string title, description;
          cout << "Enter task title: ";
          cin.ignore();
          getline(cin, title);
          cout << "Enter task description: ";
          getline(cin, description);
          manager.addTask(title, description, false);
          cout << "Task added!\n";
          break;
        }
        case 2:
          manager.updateTaskById();
        break;
        case 3: {
          int id;
          cout << "Enter ID of task to delete: ";
          cin >> id;
          manager.deleteTaskById(id);
          break;
        }
        case 4:
          manager.filterTasks();
        break;
        case 5:
          cout << "Exiting\n";
        return 0;
      }
      cout << "\nPress Enter to continue...";
      cin.ignore();
      cin.get();
    }
  }
}