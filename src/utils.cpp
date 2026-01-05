#include "utils.h"
#include "TextSettings.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <limits>

using namespace std;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void typeText(const string& text, int delayMs) {
    // If environment variable is set or skipTyping is enabled in settings
    if (getenv("SKJORHEIM_TEST_MODE") || textSettings.skipTyping) {
        cout << textSettings.color << text << RESET << "\n";
        return;
    }

    cout << textSettings.color;
    for (int i = 0; i < (int)text.length(); i++) {
        cout << text[i] << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));

        // Check if user pressed Enter to skip
        // cin.rdbuf()->in_avail() checks if there is data in the input buffer
        if (cin.rdbuf()->in_avail() > 0) {
            // Check if the character is a newline (Enter)
            if (cin.peek() == '\n') {
                cin.ignore(); // Consume the newline
                // Print the rest of the string immediately
                cout << text.substr(i + 1);
                break;
            } else {
                // If it's not a newline, we might want to ignore it or keep it?
                // For "skipping", usually any key works, but requirement says "pressing Enter".
                // We'll consume it to prevent it from affecting the next menu choice.
                cin.ignore();
                cout << text.substr(i + 1);
                break;
            }
        }
    }
    cout << RESET << "\n";
}

string loadStoryText(const string& filepath) {
    ifstream file(filepath);
    if (!file.is_open()) {
        return "[ERROR: Could not load story text from " + filepath + "]";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
