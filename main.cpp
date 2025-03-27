#include <iostream>
using namespace std;

// Task class representing a single task
class Task {
public:
    int id;
    int duration;

    Task() {
        id = 0;
        duration = 0;
    }

    Task(int idVal, int durVal) {
        id = idVal;
        duration = durVal;
    }
};

// Simple integer queue for task processing
class Queue {
private:
    int* data;
    int front;
    int back;
    int maxSize;

public:
    Queue(int size) {
        data = new int[size];
        front = 0;
        back = 0;
        maxSize = size;
    }

    void push(int value) {
        data[back++] = value;
    }

    int pop() {
        return data[front++];
    }

    bool isEmpty() {
        return front == back;
    }

    ~Queue() {
        delete[] data;
    }
};

// Dependency graph using adjacency list representation for a weighted DAG
class DependencyGraph {
private:
    int** adjList;     // Each task has a list of (toTask, weight)
    int* edgeCount;    // Number of edges for each task
    int* edgeCapacity; // Edge capacity for each task
    int taskCount;

    void expand(int task) {
        int newCapacity = edgeCapacity[task] * 2;
        int* newEdges = new int[newCapacity * 2];
        for (int i = 0; i < edgeCount[task]; ++i) {
            newEdges[2 * i] = adjList[task][2 * i];
            newEdges[2 * i + 1] = adjList[task][2 * i + 1];
        }
        delete[] adjList[task];
        adjList[task] = newEdges;
        edgeCapacity[task] = newCapacity;
    }

public:
    DependencyGraph(int taskCount) {
        this->taskCount = taskCount;
        adjList = new int*[taskCount];
        edgeCount = new int[taskCount];
        edgeCapacity = new int[taskCount];
        for (int i = 0; i < taskCount; ++i) {
            edgeCapacity[i] = 2;
            edgeCount[i] = 0;
            adjList[i] = new int[2 * edgeCapacity[i]];
        }
    }

    void addDependency(int task, int dependentTask, int weight) {
        if (edgeCount[task] == edgeCapacity[task]) {
            expand(task);
        }
        adjList[task][2 * edgeCount[task]] = dependentTask;
        adjList[task][2 * edgeCount[task] + 1] = weight;
        ++edgeCount[task];
    }

    int* getDependencies(int task, int& count) {
        count = edgeCount[task];
        return adjList[task];
    }

    ~DependencyGraph() {
        for (int i = 0; i < taskCount; ++i) {
            delete[] adjList[i];
        }
        delete[] adjList;
        delete[] edgeCount;
        delete[] edgeCapacity;
    }
};

// Project class to handle tasks, dependencies, and finding the longest path
class Project {
private:
    Task* tasks;
    DependencyGraph* dependencies;
    int taskCount;

    bool detectCycleUtil(int task, bool* visited, bool* recStack) {
        if (visited[task] == false) {
            visited[task] = true;
            recStack[task] = true;

            int depCount;
            int* deps = dependencies->getDependencies(task, depCount);
            for (int i = 0; i < depCount; ++i) {
                int nextTask = deps[2 * i];
                if (!visited[nextTask] && detectCycleUtil(nextTask, visited, recStack))
                    return true;
                else if (recStack[nextTask])
                    return true;
            }
        }
        recStack[task] = false;
        return false;
    }

    bool detectCycle() {
        bool* visited = new bool[taskCount];
        bool* recStack = new bool[taskCount];
        for (int i = 0; i < taskCount; ++i) {
            visited[i] = false;
            recStack[i] = false;
        }
        for (int i = 0; i < taskCount; ++i) {
            if (detectCycleUtil(i, visited, recStack)) {
                delete[] visited;
                delete[] recStack;
                return true;
            }
        }
        delete[] visited;
        delete[] recStack;
        return false;
    }

public:
    Project(int taskCount) : taskCount(taskCount) {
        tasks = new Task[taskCount];
        dependencies = new DependencyGraph(taskCount);
    }

    void addTask(int id, int duration) {
        if (id < 0 || id >= taskCount) {
            cout << "Error: Invalid task ID.\n";
            exit(1);
        }
        tasks[id] = Task(id, duration);  // Allow duration 0
    }

    void addDependency(int fromTask, int toTask, int weight) {
        if (fromTask < 0 || toTask < 0 || fromTask >= taskCount || toTask >= taskCount) {
            cout << "Error: Invalid task ID for dependency.\n";
            exit(1);
        }
        // Allow weight of 0 as valid
        dependencies->addDependency(fromTask, toTask, weight); 
    }

    void validateInput() {
        if (detectCycle()) {
            cout << "Error: Dependency graph contains a cycle, which is not allowed for DAG.\n";
            exit(1);
        }
    }

    void findLongestPath() {
        validateInput();

        int* inDegree = new int[taskCount](); // In-degree of tasks
        int* maxPath = new int[taskCount]();  // Longest path to reach each task
        int* prevTask = new int[taskCount];   // Previous task in the critical path

        for (int i = 0; i < taskCount; ++i) {
            prevTask[i] = -1; // Initialize with -1 (no predecessor)
        }

        // Calculate in-degrees
        for (int task = 0; task < taskCount; ++task) {
            int depCount;
            int* deps = dependencies->getDependencies(task, depCount);
            for (int i = 0; i < depCount; ++i) {
                int dependentTask = deps[2 * i];
                ++inDegree[dependentTask];
            }
        }

        // Queue for tasks with in-degree 0
        Queue zeroInDegreeQueue(taskCount);
        for (int i = 0; i < taskCount; ++i) {
            if (inDegree[i] == 0) {
                zeroInDegreeQueue.push(i);
                maxPath[i] = tasks[i].duration;
            }
        }

        // Topological processing
        while (!zeroInDegreeQueue.isEmpty()) {
            int currentTask = zeroInDegreeQueue.pop();
            int depCount;
            int* deps = dependencies->getDependencies(currentTask, depCount);
            for (int i = 0; i < depCount; ++i) {
                int nextTask = deps[2 * i];
                int taskWeight = deps[2 * i + 1];
                if (maxPath[currentTask] + taskWeight > maxPath[nextTask]) {
                    maxPath[nextTask] = maxPath[currentTask] + taskWeight;
                    prevTask[nextTask] = currentTask;
                }
                if (--inDegree[nextTask] == 0) {
                    zeroInDegreeQueue.push(nextTask);
                }
            }
        }

        // Find the longest path
        int longestPathLength = 0;
        int lastTask = 0;
        for (int i = 0; i < taskCount; ++i) {
            if (maxPath[i] > longestPathLength) {
                longestPathLength = maxPath[i];
                lastTask = i;
            }
        }

        // Display results
        cout << "\nLongest Path Length: " << longestPathLength << " units of time\n";
        cout << "Tasks in Longest Path (in reverse order): ";

        // Backtrack to find the critical path
        int task = lastTask;
        while (task != -1) {
            cout << "Task " << task << " ";
            task = prevTask[task];
        }
        cout << endl;

        delete[] inDegree;
        delete[] maxPath;
        delete[] prevTask;
    }

    ~Project() {
        delete[] tasks;
        delete dependencies;
    }
};

// Driver code
int main() {
    int taskCount, depCount;
    cout << "Enter the total number of tasks: ";
    cin >> taskCount;

    Project project(taskCount);

    cout << "Enter the duration for each task:\n";
    for (int i = 0; i < taskCount; ++i) {
        int duration;
        cout << "Duration of Task " << i << ": ";
        cin >> duration;
        project.addTask(i, duration); // Allow 0 as valid duration
    }

    cout << "Enter the number of dependencies: ";
    cin >> depCount;

    cout << "Enter each dependency as: <FromTask> <ToTask> <Weight>\n";
    for (int i = 0; i < depCount; ++i) {
        int fromTask, toTask, weight;
        cout << "Dependency " << i + 1 << ": ";
        cin >> fromTask >> toTask >> weight;
        project.addDependency(fromTask, toTask, weight); // Allow weight 0
    }

    // Find the longest path (critical path in weighted DAG)
    project.findLongestPath();

    return 0;
}
