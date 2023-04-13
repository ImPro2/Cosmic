#include <Windows.h>
#include <iostream>
import SandboxScript.CameraScript;
import Cosmic.Script.NativeScript;
import Cosmic.App.Application;

using namespace Cosmic;

extern "C"
{

    // Sets all necessary pointers
    __declspec(dllexport) void CSInit(Application* app)
    {
        Application::Set(app);
    }

    __declspec(dllexport) NativeScript* CSInstantiateCameraScript()
    {
        return new CameraScript();
    }

    __declspec(dllexport) void CSDestroyCameraScript(NativeScript* instance)
    {
        delete (CameraScript*)instance;
        instance = nullptr;
    }
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved)  // reserved
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
        break;

    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:

        if (lpvReserved != nullptr)
        {
            break; // do not do cleanup if process termination scenario
        }

        // Perform any necessary cleanup.
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
