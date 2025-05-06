#include<iostream>
#include<string>
using namespace std;
#pragma once
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
    