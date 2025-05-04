#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include<ctime>
#include<iomanip> 
#include<stdexcept>
using namespace std;

//Class to Store Details of particular Task
class Task {
 string description; 
 string activity;    
 int priority;       
 time_t deadline;    

public:
 Task() : description(""), activity(""), priority(-1), deadline(0) {}
 Task(const Task &t) : description(t.getDescription()), activity(t.getActivity()), priority(t.getPriority()), deadline(t.getDeadline()) {}

 ~Task() {}

    string getDescription() 
    const { return description; }
    string getActivity() 
    const { return activity; }
    int getPriority() 
    const { return priority; }
    time_t getDeadline() 
    const { return deadline; }

    void setDescription(string description) { this->description = description; }
    void setActivity(string activity) { this->activity = activity; }
    void setPriority(int priority) { this->priority = priority; }
    void setDeadline(time_t deadline) { this->deadline = deadline; }

    void details() {
     int p;
     cout <<"Enter Task priority:"<< endl;
     cout <<"Enter 4 for Teaching"<< endl;
     cout <<"Enter 3 for Research"<< endl;
     cout <<"Enter 2 for Academic"<< endl;
     cout <<"Enter 1 for Administrative"<< endl;

        while (true) {
         cin >> p;
          if (p == 4 || p == 3 || p == 2 || p == 1) {
           setPriority(p);
           setActivityUsingPriority(p);
           break;}
           else
           cout <<"Oops!...There was some Error...Enter Again!"<< endl; 
        }

        cout <<"Enter task description"<< endl;
        cin.ignore();
        getline(cin, description);

        // Input for deadline
        struct tm tm;
        cout <<"Enter task deadline (YYYY-MM-DD HH:MM:SS): ";
        cin >> get_time(&tm, "%Y-%m-%d %H:%M:%S");
        deadline = mktime(&tm);
    }

    void setActivityUsingPriority(int priority) {
        if (priority == 4)
         setActivity("Teaching");

        else if (priority == 3)
         setActivity("Research");

        else if (priority == 2)
         setActivity("Academic");

        else 
         setActivity("Administrative");
        
    }

    void showDetails() const {
        cout << "****************************************" << endl;
        cout << "Description : " << this->description << endl;
        cout << "Activity : " << this->activity << endl;
        cout << "Deadline : " << ctime(&deadline);
        cout << "****************************************" << endl;
    }

    bool isDeadlinePassed() const {
        return difftime(time(0), deadline) > 0;
    }
};

class List {
    string nameOfThePerson;
    vector<Task> tasks; // Use vector for dynamic memory management

public:
    List(string nameOfThePerson) : nameOfThePerson(nameOfThePerson) {}

    string getProfessor() const { return nameOfThePerson; }
    int getPendingTasks() const { return tasks.size(); }

    void addTask(const Task &task) {
        if (tasks.size() == 100)
         throw overflow_error("Task list is full");
    
        tasks.push_back(task);
        push_heap(tasks.begin(), tasks.end(), [](const Task &a, const Task &b) { return a.getPriority() < b.getPriority(); });
    }

    Task removeTask() {
        if (tasks.empty())
         throw runtime_error("No tasks to remove");
        

        pop_heap(tasks.begin(), tasks.end(), [](const Task &a, const Task &b) 
        { return a.getPriority() < b.getPriority(); });
        Task task = tasks.back();
        tasks.pop_back();
        return task;
    }

    Task currentPriority() const {
        if (tasks.empty()) 
         throw runtime_error("No tasks available");
        
        return tasks.front();
    }

    Task nextPriority() const {
        if (tasks.size() < 2) 
         throw runtime_error("Not enough tasks for next priority");
        
        return tasks[1];
    }

    void printTasks() const {
        cout << "------------------------------------------------------------" << endl;
        cout << "PENDING TASKS:" << endl;
        for (const auto &task : tasks) {
            task.showDetails();
        }
        cout << "------------------------------------------------------------" << endl;
    }

    // Search tasks by activity
    void searchTasksByActivity(const string &activity) const {
        bool found = false;
        for (const auto &task : tasks) {
            if (task.getActivity() == activity) {
                task.showDetails();
                found = true;
            }
        }
        if (!found) 
         cout << "No tasks found for activity: " << activity << endl;
        
    }
};

int main() {
    string nameOfThePerson;
    cout << "Enter name of Professor" << endl;
    cin.ignore();
    getline(cin, nameOfThePerson);

    List taskList(nameOfThePerson);
    Task task;

    while (true) {
        cout << "-------------------------------------------------------------------------------" << endl;
        cout << "Press 1 to add New Task" << endl;
        cout << "Press 2 to complete highest Priority Task" << endl;
        cout << "Press 3 to see current Priority Task" << endl;
        cout << "Press 4 to see next Priority Task" << endl;
        cout << "Press 5 to display All Pending Tasks" << endl;
        cout << "Press 6 to search tasks by activity" << endl;
        cout << "Press 7 to Exit from program" << endl;

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        try {
         switch (choice) {
          case 1:
            task.details();
            taskList.addTask(task);
            break;

         case 2:
            task = taskList.removeTask();
            task.showDetails();
            cout << "REMOVED" << endl;
            break;

         case 3:
            task = taskList.currentPriority();
            task.showDetails();
            cout << "PENDING NOW" << endl;
            break;

         case 4:
            task = taskList.nextPriority();
            task.showDetails();
            cout << "PENDING NEXT" << endl;
            break;

         case 5:
            taskList.printTasks();
            break;

         case 6: {
            string activity;
            cout <<"Enter activity to search: ";
            cin.ignore();
            getline(cin, activity);
            taskList.searchTasksByActivity(activity);
            break;
        }

         case 7:
            return 0;

            default:
            cout << "Invalid choice! Try again." << endl;
            }
        }
        catch (const exception &e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    return 0;
}
