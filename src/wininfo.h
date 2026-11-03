#ifndef WININFO_H
#define WININFO_H

#include <string>

bool checkAdminPrivileges();

std::string getWindowsEdition();

std::string getLicenseKey(const std::string& edition);

#endif 
