#include <iostream>
#include <windows.h>
#include "wininfo.h"
#include "menu.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    SetConsoleTitleA("Xer Activator 1.2");

    if (!checkAdminPrivileges()) {
        return 1;
    }

    showMainMenu();

    return 0;
}
