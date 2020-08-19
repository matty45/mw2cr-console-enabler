#include <iostream>
#include <string>
#include <windows.h>

#pragma warning(disable:4996)

static void (*SendCommandToConsole)(int a1, char* cvar) = (void(__cdecl*)(int, char*))0x14059A050;
std::string command;

DWORD WINAPI dwConsole(LPVOID)

{
    //Initialize Console
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
	
	//Redirect output to console
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);

    SetConsoleTitle(TEXT("Modern Warfare 2 Remastered Campaign - Dev Console"));

	while (true)
	{
        // Get user input
        std::cout << "\n> ";
        std::getline(std::cin, command);
        std::cout << command;

		//(char*)"unbindall";;
        // Send the command
        SendCommandToConsole(0, _strdup(command.c_str()));
	}

    return 0;
}

bool WINAPI DllMain(HMODULE hDll, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        CreateThread(0, 0, dwConsole, 0, 0, 0);
        return TRUE;
    }
    return FALSE;
}