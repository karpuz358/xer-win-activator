#include <iostream>
#include <windows.h>
#include <string>
#include <functional>
#include "activation.h"
#include "ohook.h"
#include "console.h"
#include "wininfo.h"

using namespace std;

bool runHiddenCommand(const string& command, DWORD timeoutMs) {
    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        return false;
    }

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.hStdOutput = hWritePipe;
    si.hStdError = hWritePipe;
    si.wShowWindow = SW_HIDE;

    if (!CreateProcessA(NULL, (LPSTR)command.c_str(), NULL, NULL, TRUE,
                        CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(hReadPipe);
        CloseHandle(hWritePipe);
        return false;
    }

    CloseHandle(hWritePipe);

    DWORD waitResult = WaitForSingleObject(pi.hProcess, timeoutMs);

    
    char buffer[1024];
    DWORD bytesRead;
    while (ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
    }

    DWORD exitCode = 1;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    CloseHandle(hReadPipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return (waitResult == WAIT_OBJECT_0 && exitCode == 0);
}

bool executeActivation(const string& licenseKey) {
    return runHiddenCommand(
        "cscript //nologo C:\\Windows\\System32\\slmgr.vbs /ipk " + licenseKey,
        30000);
}

bool setKMSServer() {
    return runHiddenCommand(
        "cmd /c cscript //nologo C:\\Windows\\System32\\slmgr.vbs /skms kms8.msguides.com",
        30000);
}

bool activateOnline() {
    return runHiddenCommand(
        "cmd /c cscript //nologo C:\\Windows\\System32\\slmgr.vbs /ato",
        60000);
}

void openActivationSettings() {
    ShellExecuteA(NULL, "open", "ms-settings:activation", NULL, NULL, SW_SHOWNORMAL);
}

bool runStep(int index, const string& label, const function<bool()>& step) {
    const int pad = 38;
    string dots = (label.length() < pad) ? string(pad - label.length(), '.') : " ";

    setColor(COL_DIM);
    cout << "  [" << index << "/3] ";
    setColor(COL_TEXT);
    cout << label << " ";
    cout.flush();
    setColor(COL_DIM);
    cout << dots << " ";
    cout.flush();

    bool ok = step();
    setColor(ok ? COL_OK : COL_ERR);
    cout << (ok ? "OK" : "FAILED") << endl;
    setColor(COL_TEXT);
    return ok;
}

void runActivationFlow(const string& editionInput) {
    clearScreen();
    showHeader();

    string edition = editionInput;
    if (edition.empty()) {
        setColor(COL_DIM);
        cout << "  Detecting Windows edition..." << endl;
        setColor(COL_TEXT);
        edition = getWindowsEdition();
    }
    string licenseKey = getLicenseKey(edition);

    setColor(COL_DIM);
    cout << "  Edition       ";
    setColor(COL_TEXT);
    cout << edition << endl;
    setColor(COL_DIM);
    cout << "  Product key   ";
    setColor(COL_TEXT);
    cout << licenseKey << endl;
    hr();

    if (!confirm("Run activation now?")) {
        setColor(COL_DIM);
        cout << "  Cancelled." << endl;
        setColor(COL_TEXT);
        return;
    }

    cout << endl;
    bool step1 = runStep(1, "Installing product key", [&] { return executeActivation(licenseKey); });
    bool step2 = runStep(2, "Setting KMS server", [&] { return setKMSServer(); });
    bool step3 = runStep(3, "Activating online", [&] { return activateOnline(); });

    cout << endl;
    if (step1 && step2 && step3) {
        setColor(COL_OK);
        cout << "  Activation completed." << endl;
        setColor(COL_DIM);
        cout << "  Opening activation settings to verify..." << endl;
        setColor(COL_TEXT);
        openActivationSettings();
    } else {
        setColor(COL_ERR);
        cout << "  Activation failed - see the FAILED step above." << endl;
        setColor(COL_DIM);
        cout << "  Check your connection, or the edition/key used." << endl;
        setColor(COL_TEXT);
    }
}

void runOfficeFlow() {
    clearScreen();
    showHeader();

    setColor(COL_DIM);
    cout << "  Method    ";
    setColor(COL_TEXT);
    cout << "Ohook (permanently linked)" << endl;
    setColor(COL_DIM);
    cout << "  Target    ";
    setColor(COL_TEXT);
    cout << "Office Click-To-Run installation" << endl;
    hr();

    if (!confirm("Run Office activation now?")) {
        setColor(COL_DIM);
        cout << "  Cancelled." << endl;
        setColor(COL_TEXT);
        return;
    }

    cout << endl;
    if (activateOfficeOhook()) {
        setColor(COL_OK);
        cout << "  Office activation completed." << endl;
    } else {
        setColor(COL_ERR);
        cout << "  Office activation failed - see details above." << endl;
    }
    setColor(COL_TEXT);
}
