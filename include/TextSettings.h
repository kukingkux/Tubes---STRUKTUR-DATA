#ifndef TEXTSETTINGS_H_INCLUDED
#define TEXTSETTINGS_H_INCLUDED
#include <string>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

struct TextSettings {
    int speedMs = 25;
    std::string color = "\033[37m";
    bool skipTyping = false;
};

extern TextSettings textSettings;

void typeText(const std::string& text, int delayMs = 25);

#endif