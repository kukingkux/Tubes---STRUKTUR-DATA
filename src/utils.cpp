#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <limits>

using namespace std;

// Definition of the global settings instance
TextSettingsStruct textSettings;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void typeText(const string& text, int delayMs) {
    // If text is empty, just return
    if (text.empty()) return;

    // Clear any previous input (e.g. newlines from menu choices)
    // to prevent instant skipping. Using in_avail checks prevents blocking.
    while (cin.rdbuf()->in_avail() > 0) {
        cin.get();
    }

    cout << textSettings.color;

    // If skip typing is enabled in settings, print all at once
    if (textSettings.skipTyping) {
        cout << text << RESET << "\n";
        return;
    }

    // Process character by character
    int currentSpeed = textSettings.speedMs;

    for (size_t i = 0; i < text.size(); ++i) {
        cout << text[i] << flush;

        if (currentSpeed > 0) {
            // Sleep for the specified duration
            this_thread::sleep_for(chrono::milliseconds(currentSpeed));

            // Check if user pressed Enter to skip
            if (cin.rdbuf()->in_avail() > 0) {
                // Consume all pending input to avoid carry-over
                while (cin.rdbuf()->in_avail() > 0) {
                    cin.get();
                }
                // Set speed to 0 to finish instantly
                currentSpeed = 0;
            }
        }
    }
    cout << RESET << "\n";
}

string loadStoryText(const string& filepath) {
    ifstream file(filepath);
    if (!file.is_open()) {
        return "[ERROR: Could not load story text from " + filepath + "]\n";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void damageOutput(int index, int damage, string enemy) {
    if (index == 0) {
        cout << "You dealt " << damage << " damage to " << enemy << "!\n";
    } else if (index == 1) {
        cout << "You take " << damage << " damage!\n";
    }
}
