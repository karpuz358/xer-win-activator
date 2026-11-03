#include <iostream>
#include <string>
#include <vector>
#include "menu.h"
#include "console.h"
#include "wininfo.h"
#include "activation.h"
#include "ohook.h"

using namespace std;

static const vector<string> EDITION_LABELS = {
    "Home",
    "Home N",
    "Home Single Language",
    "Home Country Specific",
    "Professional",
    "Professional N",
    "Education",
    "Education N",
    "Enterprise",
    "Enterprise N"
};

static string osPrefix() {
    if (getWindowsEdition().find("Windows 11") != string::npos) {
        return "Windows 11 ";
    }
    return "Windows 10 ";
}

void showMainMenu() {
    bool quit = false;

    while (!quit) {
        clearScreen();
        showHeader();

        menuItem(1, "Activate Windows");
        menuItem(2, "Select Windows edition");
        menuItem(3, "Activate Office (Ohook)");
        menuItem(0, "Exit");

        switch (promptChoice()) {
            case 1:
                runActivationFlow("");   
                pauseScreen();
                break;
            case 2:
                showWindowsEditionsMenu();
                break;
            case 3:
                runOfficeFlow();
                pauseScreen();
                break;
            case 0:
                quit = true;
                break;
            default:
                showInvalidChoice();
                pauseScreen();
        }
    }
}

void showWindowsEditionsMenu() {
    bool back = false;

    while (!back) {
        clearScreen();
        showHeader();

        for (size_t i = 0; i < EDITION_LABELS.size(); i++) {
            menuItem((int)i + 1, EDITION_LABELS[i]);
        }
        menuItem(0, "Back");

        int choice = promptChoice();

        if (choice >= 1 && choice <= (int)EDITION_LABELS.size()) {
            runActivationFlow(osPrefix() + EDITION_LABELS[choice - 1]);
            pauseScreen();
        }
        else if (choice == 0) {
            back = true;
        }
        else {
            showInvalidChoice();
            pauseScreen();
        }
    }
}
