#include "ohook.h"
#include "ohook_resources.h"
#include <windows.h>
#include <imagehlp.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <shlobj.h>
#include <vector>
#include <ctime>

#pragma comment(lib, "imagehlp.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "shell32.lib")

using std::string;
using std::vector;
using std::ofstream;
using std::cout;
using std::endl;
using std::ios;
namespace fs = std::filesystem;

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::vector<unsigned char> base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::vector<unsigned char> ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret.push_back(char_array_3[i]);
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
  }

  return ret;
}

string GetRegistryString(HKEY hKeyRoot, const char* subKey, const char* valueName) {
    HKEY hKey;
    if (RegOpenKeyExA(hKeyRoot, subKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return "";
    }

    char value[1024];
    DWORD size = sizeof(value);
    DWORD type = REG_SZ;

    string result = "";
    if (RegQueryValueExA(hKey, valueName, NULL, &type, (LPBYTE)value, &size) == ERROR_SUCCESS) {
        result = string(value);
    }

    RegCloseKey(hKey);
    return result;
}

bool Is64BitOS() {
    BOOL is64 = FALSE;
    IsWow64Process(GetCurrentProcess(), &is64);
    
    
    
    SYSTEM_INFO si;
    GetNativeSystemInfo(&si);
    return (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 || si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64);
}

bool PatchDLL(const string& filePath, DWORD exportTimeDateStampOffset) {
    
    
    
    HANDLE hFile = CreateFileA(filePath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        cout << "Failed to open file for patching: " << filePath << endl;
        return false;
    }

    HANDLE hMap = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
    if (!hMap) {
        CloseHandle(hFile);
        return false;
    }

    LPVOID lpBase = MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, 0);
    if (!lpBase) {
        CloseHandle(hMap);
        CloseHandle(hFile);
        return false;
    }

    PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpBase;
    PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)((BYTE*)lpBase + pDos->e_lfanew);

    DWORD timestampOffset = (DWORD)((BYTE*)&pNt->FileHeader.TimeDateStamp - (BYTE*)lpBase);
    DWORD checksumOffset = (DWORD)((BYTE*)&pNt->OptionalHeader.CheckSum - (BYTE*)lpBase);

    
    DWORD currentTimestamp = (DWORD)time(NULL);

    
    pNt->FileHeader.TimeDateStamp = currentTimestamp;

    
    
    
    
    if (exportTimeDateStampOffset > 0 && exportTimeDateStampOffset < GetFileSize(hFile, NULL)) {
        DWORD* pExportTime = (DWORD*)((BYTE*)lpBase + exportTimeDateStampOffset);
        *pExportTime = currentTimestamp;
    }

    
    DWORD headerSum = 0;
    DWORD checkSum = 0;
    
    
    
    PIMAGE_NT_HEADERS pNtChecked = CheckSumMappedFile(lpBase, GetFileSize(hFile, NULL), &headerSum, &checkSum);
    if (pNtChecked) {
        pNt->OptionalHeader.CheckSum = checkSum;
    }

    UnmapViewOfFile(lpBase);
    CloseHandle(hMap);
    CloseHandle(hFile);
    return true;
}

bool InstallKeys() {
    cout << "Installing Office Product Keys..." << endl;
    
    
    
    
    
    
    
    bool has16 = !GetRegistryString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Office\\ClickToRun\\Configuration", "InstallPath").empty();
    if (!has16) has16 = !GetRegistryString(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Microsoft\\Office\\ClickToRun\\Configuration", "InstallPath").empty();
    
    
    
    
    int successCount = 0;
    for (const auto& k : s_officeKeys) {
        if (k.version == "16" && has16) {
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            if (k.edition.find("ProPlus") != string::npos && k.type.find("Retail") != string::npos) {
                 string command = "cscript //nologo C:\\Windows\\System32\\slmgr.vbs /ipk " + k.key;
                 
                 
                 
                 
                 
            }
        }
    }
    
    
    return true;
}

bool activateOfficeOhook() {
    cout << "\nOhook activation" << endl;

    
    string officeRoot = GetRegistryString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Office\\ClickToRun\\Configuration", "InstallPath");
    if (officeRoot.empty()) {
        officeRoot = GetRegistryString(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Microsoft\\Office\\ClickToRun\\Configuration", "InstallPath");
    }

    if (officeRoot.empty()) {
        cout << "[!] Could not detect Office Click-To-Run installation." << endl;
        
        return false;
    }

    cout << "[+] Found Office Root: " << officeRoot << endl;

    
    
    string platform = GetRegistryString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Office\\ClickToRun\\Configuration", "Platform");
    if (platform.empty()) {
        platform = GetRegistryString(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Microsoft\\Office\\ClickToRun\\Configuration", "Platform");
    }
    
    
    
    bool isOffice64 = (platform == "x64");
    cout << "[+] Office Platform: " << (isOffice64 ? "x64" : "x86") << endl;

    
    
    
    
    
    
    
    fs::path installDir = fs::path(officeRoot) / "root" / "vfs" / "System";
    if (!fs::exists(installDir) && !isOffice64 && Is64BitOS()) {
         installDir = fs::path(officeRoot) / "root" / "vfs" / "SystemX86";
    }

    if (!fs::exists(installDir)) {
        cout << "[!] Could not find vfs\\System directory at: " << installDir << endl;
        
        installDir = fs::path(officeRoot); 
        
    }
    
    cout << "[+] Target Directory: " << installDir.string() << endl;

    fs::path sppcPath = installDir / "sppc.dll";
    fs::path sppcsPath = installDir / "sppcs.dll";
    fs::path systemSppc = "C:\\Windows\\System32\\sppc.dll"; 
    
    
    
    
    
    
    

    
    if (fs::exists(sppcsPath)) {
        cout << "[*] Removing existing sppcs.dll..." << endl;
        fs::remove(sppcsPath);
    }
    if (fs::exists(sppcPath)) {
        
        
        
        
        
        
        cout << "[*] Removing existing sppc.dll..." << endl;
        fs::remove(sppcPath);
    }

    
    cout << "[*] Creating Symlink sppcs.dll -> " << systemSppc.string() << endl;
    
    
    if (!CreateSymbolicLinkA(sppcsPath.string().c_str(), systemSppc.string().c_str(), 0)) {
        
        string mklinkCmd = "cmd /c mklink \"" + sppcsPath.string() + "\" \"" + systemSppc.string() + "\"";
        if (system(mklinkCmd.c_str()) != 0) {
             cout << "[!] Failed to create symlink. Error: " << GetLastError() << endl;
             return false;
        }
    }

    
    cout << "[*] decoding and writing Ohook DLL..." << endl;
    vector<unsigned char> dllBytes;
    DWORD offset = 0;

    if (isOffice64) {
        dllBytes = base64_decode(s_sppc64_base64);
        offset = 3076;
    } else {
        dllBytes = base64_decode(s_sppc32_base64);
        offset = 2564;
    }

    if (dllBytes.empty()) {
        cout << "[!] Failed to decode Base64 DLL." << endl;
        return false;
    }

    ofstream dllFile(sppcPath, ios::binary);
    if (!dllFile) {
        cout << "[!] Failed to write to " << sppcPath << endl;
        return false;
    }
    dllFile.write((const char*)dllBytes.data(), dllBytes.size());
    dllFile.close();

    
    cout << "[*] Patching DLL timestamps and checksum..." << endl;
    if (!PatchDLL(sppcPath.string(), offset)) {
        cout << "[!] Warning: Failed to patch DLL. Antivirus might flag it." << endl;
    }

    cout << "\n[+] Ohook installation completed successfully!" << endl;
    cout << "[+] Office should now be permanently activated." << endl;
    
    return true;
}
