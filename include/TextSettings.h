#ifndef TEXTSETTINGS_H_INCLUDED
#define TEXTSETTINGS_H_INCLUDED
#include <string>

// COLORS
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

struct TextSettingsStruct {
    int speedMs = 25;
    std::string color = WHITE;
    bool skipTyping = false;
};

// Global settings instance
extern TextSettingsStruct textSettings;

void damageOutput(int index, int damage, std::string enemy = "");

#endif