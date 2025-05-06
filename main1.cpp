#include "adminpanel.h" 
#include <iostream>
#include <stdexcept>

using namespace std;

int main() {
        adminManager.loadAdminsFromFile();
        adminManager.signUp("Nili", "hacilfe44");
        adminManager.signIn("Nili", "hacilfe44");
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

            cout << "\033[34m" << " .-') _                                            ('-.  _  .-')         .-') _                       _ .-') _                                         .-')    .-') _    " << endl;
            cout << "\033[34m" << "    ( OO ) )                                         _(  OO)( ( -O )       (  OO) )                     ( (  OO) )                                       ( OO ). (  OO) )   " << endl;
            cout << "\033[34m" << ",--./ ,--,' ,-.-')  ,--.     ,--. ,--.      ,------.(,------.,------.       /     '._  .-'),-----.        \\     .'_  .-'),-----.        ,--.      ,-.-') (_)---\\_)/     '._  " << endl;
            cout << "\033[34m" << "|   \\ |  |\\ |  |OO) |  |.-') |  | |  |   ('-| _.---' |  .---'|   /`. '      |'--...__)( OO'  .-.  '       ,`'--..._)( OO'  .-.  '       |  |.-')  |  |OO)/    _ | |'--...__) " << endl;
            cout << "\033[34m" << "|    \\|  | )|  |  \\ |  | OO )|  | | .-') (OO|(_\\     |  |    |  /  | |      '--.  .--'/   |  | |  |       |  |  \\  '/   |  | |  |       |  | OO ) |  |  \\  :` `. '--.  .--' " << endl;
            cout << "\033[34m" << "|  .     |/ |  |(_/ |  |`-' ||  |_|( OO )/  |  '--. (|  '--. |  |_.' |         |  |   \\_) |  |\\|  |       |  |   ' |\\_) |  |\\|  |       |  |`-' | |  |(_/ '..`''.)   |  |    " << endl;
            cout << "\033[34m" << "|  |\\    | ,|  |_.'(|  '---.'|  | | `-' /\\_)|  .--'  |  .--' |  .  '.'         |  |     \\ |  | |  |       |  |   / :  \\ |  | |  |      (|  '---.',|  |_.'.-._)   \\   |  |    " << endl;
            cout << "\033[34m" << "|  | \\   |(_|  |    |      |('  '-'(_.-'   \\|  |_)   |  `---.|  |\\  \\          |  |      `'  '-'  '       |  '--'  /   `'  '-'  '       |      |(_|  |   \\       /   |  |    " << endl;
            cout << "\033[34m" << "`--'  `--'  `--'    `------'  `-----'       `--'     `------'`--' '--'         `--'        `-----'        `-------'      `-----'        `------'  `--'    `-----'    `--'    " << endl;

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
                try {
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
                            if (title.empty() || description.empty()) {
                                throw invalid_argument("Title or description cannot be empty!");
                            }
                            manager.addTask(title, description, false);
                            break;
                        }
                        case 2:
                            manager.updateTaskById();
                            break;
                        case 3:
                            manager.deleteTaskById();
                            break;
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
                } catch (const exception& ex) {
                    cout << "An error occurred: " << ex.what() << endl;
                }

                cout << "\nPress Enter to continue";
                cin.ignore();
                cin.get();
            }
    } }