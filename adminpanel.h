#include"admin.h"
#pragma once
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
                cout << "Loading admin: " << username << " / " << password << endl;
        
                try {
                    admins.emplace_back(username, password);
                } catch (const exception& ex) {
                    cout << "Skipping admin (" << username << "): " << ex.what() << endl;
                }
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