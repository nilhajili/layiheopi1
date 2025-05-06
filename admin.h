#include"todo.h"
#include"task.h"
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
    
        bool set_username(const string& username) {
            if (username.length() <= 3) {
                cout << "Username must be longer than 3 characters." << endl;
                return false;
            }
            _username = username;
            return true;
        }
        
        bool set_password(const string& password) {
            if (password.length() <= 6) {
                cout << "Password must be longer than 6 characters." << endl;
                return false;
            }
            _password = password;
            return true;
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
    