# 🟩 Wordle Game & Automated Solver (C Project)

This repository contains a modular implementation of the **Wordle Game** engine and an **Automated Solver** that uses constraint satisfaction to find the secret word in 6 attempts or fewer.

## 👥 Group Members (ISIL C)
* **Lakehal Dekrah**
* **Ramoul Meriem**
* **Seray Imene**

---

## 📁 Project Structure

The project is organized into modular components to ensure separation of concerns and maintainability:

* **`functions of the wordle game/`**: Contains the core logic for the Wordle engine (dictionary loading, feedback generation, and user input validation).
* **`functions of the wordle solver/`**: Contains the solver intelligence, including candidate elimination and scoring logic.
* **`graph of the code/`**: Visual documentation of the code structure and function dependencies (created with Obsidian).
* **Documentation**:
    * `WORDLEGAME (2).pdf`: Detailed technical report of the game engine.
    * `WORDLE_SOLVER.pdf`: Algorithmic analysis and documentation of the solver.

---

## 🧠 Algorithmic Approach

### The Game Engine
The engine handles the standard Wordle rules, including the complex "yellow vs. gray" priority logic to ensure that repeated letters are marked correctly according to their frequency in the secret word.

### The Solver (Information Theory)
The solver follows a **Constraint Satisfaction** strategy:
1.  **Candidate Generation**: It starts with a full dictionary of possible words.
2.  **Feedback Analysis**: Upon receiving feedback (Green/Yellow/Gray), it filters the search space.
3.  **Efficiency**: The filtering algorithm operates at $O(N \times L)$ complexity, where $N$ is the number of words and $L$ is the length of the word.



---

## 🛠️ Setup and Execution

### Prerequisites
* A Linux environment (tested on **Fedora**).
* `gcc` compiler.
* `make` (optional, if using the Makefile).

