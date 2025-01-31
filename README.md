# SpellChecker

This project is a spell checker application with a modern Python GUI (using `customtkinter`) and a C++ backend for spell-checking logic. The C++ backend uses a **Trie data structure** and **Levenshtein distance algorithm** to provide spelling suggestions.

---

## Features
- **Python GUI**: A user-friendly interface built with `customtkinter`.
- **C++ Backend**: Efficient spell-checking logic using a **Trie** and **Levenshtein distance**.
- **Cross-Platform**: Works on both Windows and Linux.
- **Dynamic Suggestions**: Provides spelling suggestions based on word length and similarity.

---

## How It Works

### Algorithm Explanation

#### 1. **Trie Data Structure**
   - A **Trie** (pronounced "try") is a tree-like data structure used to store words. Each node in the Trie represents a character of a word.
   - **Why Trie?**:
     - It allows for fast prefix-based searches. For example, if you type "app", the Trie can quickly find all words that start with "app" (like "apple", "application", etc.).
     - It is efficient for storing and retrieving words from a dictionary.

#### 2. **Levenshtein Distance**
   - The **Levenshtein distance** (also called edit distance) measures how many edits (insertions, deletions, or substitutions) are needed to change one word into another.
   - **Why Levenshtein Distance?**:
     - It helps find words that are similar to the misspelled word. For example, if you type "helo", the algorithm can suggest "hello" because it only requires one edit (adding an "l").

#### 3. **Dynamic Threshold**
   - The algorithm adjusts the **tolerance for typos** based on the length of the word:
     - Short words (e.g., "cat") have a strict threshold (only 1 typo allowed).
     - Medium words (e.g., "apple") allow 2 typos.
     - Long words (e.g., "international") allow 3 typos.
   - This ensures that suggestions are more accurate and relevant.

#### 4. **Spell-Checking Process**
   - When you enter text, the program splits it into individual words.
   - For each word, it checks if the word exists in the dictionary (stored in a Trie).
   - If the word is misspelled, the program:
     1. Finds all words in the Trie that start with the same prefix.
     2. Uses the Levenshtein distance to rank similar words.
     3. Suggests the closest matches based on the dynamic threshold.
        
### Algorithm in Simple Terms
Imagine you’re typing a word, but you make a typo. The program tries to help you by:
1. **Looking up the word in a dictionary** (stored in a Trie for fast searching).
2. If the word is misspelled, it **finds similar words** by:
   - Checking words that start with the same letters (using the Trie).
   - Calculating how many changes (like adding, removing, or swapping letters) are needed to turn your typo into a correct word (using Levenshtein distance).
3. It then **suggests the closest matches** based on how long the word is (shorter words have stricter rules).

---

## Prerequisites

### For Windows:
1. **Python**: Install Python.
2. **C++ Compiler**: Install MinGW.
3. **CustomTkinter**: Install the `customtkinter` library:
   ```bash
   pip install customtkinter
   ```

### For Linux:
1. **Python**: Install Python using your package manager:
   ```bash
   sudo apt-get install python3
   ```
2. **C++ Compiler**: Install `g++`:
   ```bash
   sudo apt-get install g++
   ```
3. **CustomTkinter**: Install the `customtkinter` library:
   ```bash
   pip install customtkinter
   ```

---

## Installation and Setup

### For Windows:
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your-username/spell-checker.git
   cd spell-checker
   ```
2. **Compile the C++ Backend**:
   Open a terminal in the project directory and run:
   ```bash
   g++ spellchecker.cpp -o spellchecker.exe
   ```
3. **Run the Python GUI**:
   ```bash
   python GUI.py
   ```

### For Linux:
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your-username/spell-checker.git
   cd spell-checker
   ```
2. **Compile the C++ Backend**:
   Open a terminal in the project directory and run:
   ```bash
   g++ spellchecker.cpp -o spellchecker
   ```
3. **Run the Python GUI**:
   ```bash
   python3 GUI.py
   ```

---

## Usage
1. Enter text in the input area.
2. Click **Check Spelling** to see spelling suggestions.
3. Click **Clear** to reset the input and result areas.

---

## Project Structure
```
spell-checker/
├── GUI.py                # Python GUI code
├── spellchecker.cpp      # C++ backend code
├── spellchecker.exe      # Compiled C++ executable (Windows)
├── spellchecker          # Compiled C++ executable (Linux)
├── dictionary.txt        # Dictionary file for spell checking
```

---

## Contributing
Contributions are welcome! If you find any issues or want to add new features, feel free to open a pull request.

---

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---
