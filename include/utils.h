#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <string>

void clearInput();
void typeText(const std::string& text, int delayMs = 30);
std::string loadStoryText(const std::string& filepath);

#endif