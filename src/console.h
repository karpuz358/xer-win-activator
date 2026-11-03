#ifndef CONSOLE_H
#define CONSOLE_H

enum {
    COL_TEXT   = 15, 
    COL_DIM    = 8,  
    COL_ACCENT = 11, 
    COL_OK     = 10, 
    COL_ERR    = 12  
};

const int LINE_WIDTH = 56;

void setColor(int color);
void clearScreen();
void pauseScreen();
void hr();
void showHeader();

bool confirm(const std::string& question);

void menuItem(int number, const std::string& label);
int  promptChoice();
void showInvalidChoice();

#endif 
