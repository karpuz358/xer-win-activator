#include <windows.h>
#include <string>
#include "wininfo.h"

using namespace std;

bool checkAdminPrivileges() {
    BOOL isAdmin = FALSE;
    PSID administratorsGroup = NULL;

    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
                                DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
                                &administratorsGroup)) {
        if (!CheckTokenMembership(NULL, administratorsGroup, &isAdmin)) {
            isAdmin = FALSE;
        }
        FreeSid(administratorsGroup);
    }

    if (!isAdmin) {
        MessageBoxA(NULL,
                   "Administrator privileges required!\n\n"
                   "Please restart the program as Administrator to continue.\n"
                   "Right-click on the executable and select 'Run as administrator'.",
                   "Xer Activator - Admin Required",
                   MB_OK | MB_ICONWARNING);
        return false;
    }

    return true;
}

string getWindowsEdition() {
    string edition = "Unknown";
    HKEY hKey;

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                     "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
                     0, KEY_READ, &hKey) == ERROR_SUCCESS) {

        char buffer[256];
        DWORD bufferSize = sizeof(buffer);

        if (RegQueryValueExA(hKey, "ProductName", NULL, NULL,
                            (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
            edition = string(buffer);
        }

        
        
        bufferSize = sizeof(buffer);
        if (RegQueryValueExA(hKey, "CurrentBuildNumber", NULL, NULL,
                            (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
            if (atol(buffer) >= 22000) {
                size_t pos = edition.find("Windows 10");
                if (pos != string::npos) {
                    edition.replace(pos, 10, "Windows 11");
                }
            }
        }

        RegCloseKey(hKey);
    }

    return edition;
}

string getLicenseKey(const string& edition) {
    if (edition.find("Home") != string::npos) {
        if (edition.find("Home N") != string::npos) {
            return "3KHY7-WNT83-DGQKR-F7HPR-844BM";
        } else if (edition.find("Single Language") != string::npos) {
            return "7HNRX-D7KGG-3K4RQ-4WPJ4-YTDFH";
        } else if (edition.find("Country Specific") != string::npos) {
            return "PVMJN-6DFY6-9CCP6-7BKTT-D3WVR";
        } else {
            return "TX9XD-98N7V-6WMQ6-BX7FG-H8Q99";
        }
    }
    else if (edition.find("Pro") != string::npos) {
        if (edition.find("Pro N") != string::npos || edition.find("Professional N") != string::npos) {
            return "MH37W-N47XK-V7XM9-C7227-GCQG9";
        } else {
            return "W269N-WFGWX-YVC9B-4J6C9-T83GX";
        }
    }
    else if (edition.find("Education") != string::npos) {
        if (edition.find("Education N") != string::npos) {
            return "2WH4N-8QGBV-H22JP-CT43Q-MDWWJ";
        } else {
            return "NW6C2-QMPVW-D7KKK-3GKT6-VCFB2";
        }
    }
    else if (edition.find("Enterprise") != string::npos) {
        if (edition.find("Enterprise N") != string::npos) {
            return "DPH2V-TTNVB-4X9Q3-TJR4H-KHJW4";
        } else {
            return "NPPR9-FWDCX-D2C8J-H872K-2YT43";
        }
    }

    
    return "W269N-WFGWX-YVC9B-4J6C9-T83GX";
}
