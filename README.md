# Project-Management-Tool
# Critical Path Method (CPM) in C++

## 📌 Project Overview
This project implements the **Critical Path Method (CPM)** using **C++**. It models project tasks as a **Directed Acyclic Graph (DAG)**, allowing you to compute the **longest path** (critical path) through task dependencies. This helps in determining the minimum time required to complete the project.

## 🚀 Features
✅ Task Management – Define tasks with unique IDs and durations  
✅ Dependency Graph – Represent task dependencies using an adjacency list  
✅ Cycle Detection – Ensures no cycles exist in the dependency graph  
✅ Topological Sorting – Uses a queue-based approach for processing tasks  
✅ Longest Path Calculation – Determines the critical path in the project  
✅ Optimized Memory Usage – Uses dynamic arrays for efficient memory management  

## 📂 Project Structure
```
|-- main.cpp  # Core implementation of the CPM algorithm
|-- README.md # Project documentation
```

## 🛠️ Installation & Compilation
### Prerequisites
- **C++ Compiler** (GCC/MinGW/Clang/MSVC)

### Compilation Instructions
#### Linux/macOS
```sh
g++ main.cpp -o cpm
./cpm
```
#### Windows (MinGW)
```sh
g++ main.cpp -o cpm.exe
cpm.exe
```

## 📖 How It Works
1️⃣ **User Input** – Define number of tasks and their durations  
2️⃣ **Add Dependencies** – Specify dependencies between tasks  
3️⃣ **Graph Processing** – Build the **DAG** and check for cycles  
4️⃣ **Longest Path Calculation** – Compute the **critical path** using **topological sorting**  
5️⃣ **Display Output** – Print the longest path and minimum project duration  

## 📊 Example Usage
### Input:
```
Enter the total number of tasks: 5
Enter the duration for each task:
Duration of Task 0: 3
Duration of Task 1: 2
Duration of Task 2: 4
Duration of Task 3: 6
Duration of Task 4: 5
Enter the number of dependencies: 4
Enter each dependency as: <FromTask> <ToTask> <Weight>
Dependency 1: 0 1 2
Dependency 2: 1 2 3
Dependency 3: 2 3 2
Dependency 4: 3 4 4
```

### Output:
```
Longest Path Length: 15 units of time
Tasks in Longest Path (in reverse order): Task 4 Task 3 Task 2 Task 1 Task 0
```

## 🛠️ Future Enhancements
🚀 GUI for visualization of project scheduling  
🚀 Support for multiple critical paths  
🚀 Integration with real-world project management tools  

## 🤝 Contributing
We welcome contributions! Feel free to **fork** the repository, **create a pull request**, or **suggest improvements**.

## 📩 Contact
For any discussions or enhancements, reach out!

⭐ If you like this project, don't forget to give it a **star**! ⭐

