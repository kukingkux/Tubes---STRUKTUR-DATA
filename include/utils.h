#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include <string>
#include <iostream>

// FORMATS
#define BOLD   "\033[1m"
#define RESET   "\033[0m"

// COLORS
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

struct TextSettingsStruct {
    int speedMs = 25;
    std::string color = WHITE;
    bool skipTyping = false;
    bool devMode = false;
};

// Global settings instance
extern TextSettingsStruct textSettings;

// Utils
void typeText(const std::string& text, int delayMs = 25);
void clearInput();
std::string loadStoryText(const std::string& filepath);

// Battle
std::string damageOutput(int index, int damage, std::string enemy = "");

#endif