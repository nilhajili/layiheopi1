#include <iostream>
#include <vector>
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
  ToDoList todoList;
  todoList.addTask("Task 1", "Description 1", false);
  todoList.addTask("Task 2", "Description 2", true);
  todoList.addTask("Task 3", "Description 3", false);
  cout << "All Tasks:\n";
  todoList.showTasks();
  todoList.deleteTaskById(2);
  cout << "\nTasks after deletion:\n";
  todoList.showTasks();
  cout << "\nFiltering tasks:\n";
  todoList.filterTasks();
  cout << "\nUpdate tasks:\n";
  todoList.updateTaskById();
  todoList.showTasks();

}
