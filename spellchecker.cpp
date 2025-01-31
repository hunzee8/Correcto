#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <cctype>

using namespace std;

// ==========================
// Helper Functions
// ==========================

// Converts a word to lowercase for case-insensitive comparisons
string toLowerCase(const string& word) {
    string lowerWord;
    for (char c : word) {
        lowerWord += tolower(static_cast<unsigned char>(c));
    }
    return lowerWord;
}

// Splits a given string (sentence) into individual words
vector<string> splitWords(const string& text) {
    vector<string> words;
    stringstream ss(text);
    string word;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
}

// Computes the Levenshtein distance (edit distance) between two words
// This measures how many edits (insertions, deletions, substitutions) are required to convert one word to another
int levenshteinDistance(const string& a, const string& b) {
    vector<vector<int>> dp(a.size() + 1, vector<int>(b.size() + 1));

    // Initialize the base cases for an empty string
    for (size_t i = 0; i <= a.size(); ++i) dp[i][0] = i;
    for (size_t j = 0; j <= b.size(); ++j) dp[0][j] = j;

    // Fill the DP table based on character matches or edits
    for (size_t i = 1; i <= a.size(); ++i) {
        for (size_t j = 1; j <= b.size(); ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] }) + 1;
            }
        }
    }
    return dp[a.size()][b.size()];
}

// ==========================
// Trie Data Structure
// ==========================

// Represents a single node in the Trie
struct TrieNode {
    unordered_map<char, TrieNode*> children; // Maps each character to its corresponding child node
    bool isEndOfWord;                        // Indicates if this node marks the end of a valid word

    TrieNode() : isEndOfWord(false) {}
};

class Trie {
private:
    TrieNode* root; // Root of the Trie

public:
    Trie() {
        root = new TrieNode();
    }

    // Inserts a word into the Trie
    void insert(const string& word) {
        TrieNode* currentNode = root;
        for (char c : word) {
            if (currentNode->children.find(c) == currentNode->children.end()) {
                currentNode->children[c] = new TrieNode();
            }
            currentNode = currentNode->children[c];
        }
        currentNode->isEndOfWord = true;
    }

    // Collects all words starting with a given prefix
    void getWordsWithPrefix(TrieNode* node, string prefix, vector<string>& results) {
        if (node->isEndOfWord) {
            results.push_back(prefix);
        }
        for (const auto& child : node->children) {
            getWordsWithPrefix(child.second, prefix + child.first, results);
        }
    }

    // Retrieves suggestions for words that start with the given prefix
    vector<string> getSuggestions(const string& prefix) {
        TrieNode* currentNode = root;
        vector<string> results;

        // Navigate through the Trie to the end of the prefix
        for (char c : prefix) {
            if (currentNode->children.find(c) == currentNode->children.end()) {
                return results; // No words with this prefix
            }
            currentNode = currentNode->children[c];
        }

        // Collect all words that start with the prefix
        getWordsWithPrefix(currentNode, prefix, results);
        return results;
    }
};

// ==========================
// SpellChecker Class
// ==========================

class SpellChecker {
private:
    unordered_set<string> dictionary; // Stores all valid words
    Trie trie;                        // Trie for fast prefix-based suggestions

public:
    // Constructor to load the dictionary from a file
    SpellChecker(const string& dictionaryFile) {
        loadDictionary(dictionaryFile);
    }

    // Loads words from a file into the dictionary and Trie
    void loadDictionary(const string& dictionaryFile) {
        ifstream file(dictionaryFile);
        string word;
        if (file.is_open()) {
            while (file >> word) {
                word = toLowerCase(word); // Ensure all words are lowercase
                dictionary.insert(word);
                trie.insert(word);
            }
            file.close();
        } else {
            cerr << "Failed to open dictionary file: " << dictionaryFile << endl;
        }
    }

    // Checks if a word is spelled correctly
    bool isWordCorrect(const string& word) {
        return dictionary.find(toLowerCase(word)) != dictionary.end();
    }

    // Adjusts the typo tolerance threshold based on word length
    int dynamicThreshold(const string& word) {
        size_t length = word.size();
        if (length <= 3) {
            return 1; // Very strict for short words
        } else if (length <= 5) {
            return 2; // Moderate tolerance for medium words
        } else {
            return 3; // Higher tolerance for longer words
        }
    }

    // Provides suggestions for misspelled words
    string getSuggestions(const string& word, int maxSuggestions = 10) {
        vector<string> suggestions;
        string lowerWord = toLowerCase(word);
        vector<string> prefixMatches = trie.getSuggestions(lowerWord);

        // Use a set to ensure unique suggestions
        unordered_set<string> uniqueSuggestions(prefixMatches.begin(), prefixMatches.end());
        vector<pair<int, string>> rankedSuggestions;

        // Get the dynamic typo tolerance threshold
        int threshold = dynamicThreshold(lowerWord);

        // Compare the input word with dictionary words using Levenshtein distance
        for (const auto& dictWord : dictionary) {
            if (uniqueSuggestions.size() >= maxSuggestions) break;
            int distance = levenshteinDistance(lowerWord, dictWord);
            if (distance <= threshold) { // Only consider close matches
                rankedSuggestions.push_back({ distance, dictWord });
            }
        }

        // Sort suggestions by distance
        sort(rankedSuggestions.begin(), rankedSuggestions.end());

        // Add ranked suggestions to the set
        for (const auto& pair : rankedSuggestions) {
            if (uniqueSuggestions.size() >= maxSuggestions) break;
            uniqueSuggestions.insert(pair.second);
        }

        // Prepare the final list of suggestions
        for (const auto& suggestion : uniqueSuggestions) {
            if (suggestions.size() >= maxSuggestions) break;
            suggestions.push_back(suggestion);
        }

        // Combine suggestions into a single string
        string result = "";
        for (const auto& suggestion : suggestions) {
            result += suggestion + " ";
        }
        return result;
    }
};

int main() {
    // Initialize the spell checker with a dictionary file
    SpellChecker spellChecker("dictionary.txt");

    //cout << "Please type a word/sentence: ";
    string userInput;
    getline(cin, userInput); // Read user input

    vector<string> words = splitWords(userInput);
    string correctWords;
    string misspelledWords;
    bool anyMisspelled = false;

    // Process each word from user input
    for (const auto& word : words) {
        if (!spellChecker.isWordCorrect(word)) {
            misspelledWords += "[" + word + "] "; // Mark misspelled word
            anyMisspelled = true;
        } else {
            correctWords += "'" + word + "' is a Correct word.\n"; // Add correct word
        }
    }

    // Output results
    if (!correctWords.empty()) {
        cout << correctWords; // Print all correct words
    }

    if (!anyMisspelled) {
        cout << "Words are correct!" << endl; // No misspelled words
    } else {
        cout << "Word misspelled: " << misspelledWords << endl; // Show misspelled words

        // Suggest corrections for each misspelled word
        for (const auto& word : words) {
            if (!spellChecker.isWordCorrect(word)) {
                string suggestions = spellChecker.getSuggestions(word);
                if (suggestions.empty()) {
                    cout << "No suggestions found for '" << word << "'." << endl;
                } else {
                    cout << "Suggestions for '" << word << "':" << endl;
                    vector<string> suggestionList = splitWords(suggestions);
                    for (const auto& suggestion : suggestionList) {
                        cout << "\t- " << suggestion << endl; // List each suggestion
                    }
                }
            }
        }
    }

    return 0;
}
