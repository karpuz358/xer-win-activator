#include <iostream>
#include <windows.h>
#include <conio.h>
#include "console.h"

using namespace std;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void clearScreen() {
    system("cls");
}

void pauseScreen() {
    setColor(COL_DIM);
    cout << "\n  Press any key to continue...";
    _getch();
    setColor(COL_TEXT);
}

void hr() {
    setColor(COL_DIM);
    cout << "  " << string(LINE_WIDTH, '-') << endl;
    setColor(COL_TEXT);
}

void showHeader() {
    setColor(COL_ACCENT);
    cout << "  Xer Activator 1.2";
    setColor(COL_DIM);
    cout << "  -  Windows & Office activation" << endl;
    setColor(COL_TEXT);
    hr();
}

bool confirm(const string& question) {
    setColor(COL_DIM);
    cout << "\n  " << question << " [y/N]: ";
    setColor(COL_TEXT);

    char c;
    cin >> c;
    cin.ignore(10000, '\n');
    return (c == 'y' || c == 'Y');
}

void menuItem(int number, const string& label) {
    setColor(COL_DIM);
    cout << "  " << number << "   ";
    setColor(COL_TEXT);
    cout << label << endl;
}

int promptChoice() {
    cout << endl;
    setColor(COL_ACCENT);
    cout << "  > ";
    setColor(COL_TEXT);

    int choice = -1;
    cin >> choice;
    if (!cin) {
        cin.clear();
        cin.ignore(10000, '\n');
    }
    return choice;
}

void showInvalidChoice() {
    setColor(COL_ERR);
    cout << "  Invalid choice." << endl;
    setColor(COL_TEXT);
}
