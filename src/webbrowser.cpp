/*
 * Copyright (c) 2011, Psiphon Inc.
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "stdafx.h"
#include "logging.h"
#include "config.h"
#include "psiclient.h"
#include "webbrowser.h"
#include "shellapi.h"
#include "shlwapi.h"
#include "utilities.h"

extern HWND g_hWnd;

// Creates a process with the given command line and returns a HANDLE to the 
// resulting process. Returns 0 on error; call GetLastError to find out why.
HANDLE LaunchApplication(LPCTSTR command)
{
    STARTUPINFO startupInfo = { 0 };
    PROCESS_INFORMATION processInfo = { 0 };

    // The command argument is in-out, so we need to pass a modifiable buffer.
    int command_length = _tcslen(command) + 1; // includes the null terminator
    TCHAR *command_buffer = new TCHAR[command_length];

    try
    {
        _tcsncpy_s(command_buffer, command_length, command, command_length);

        if (::CreateProcess(
                NULL,
                command_buffer,
                NULL, NULL, FALSE, 0, NULL, NULL,
                &startupInfo, &processInfo))
        {
            delete[] command_buffer;
            return processInfo.hProcess;
        }
    }
    catch (...)
    {
        // Fall through to error condition
    }

    delete[] command_buffer;
    return 0;
}

// Wait for the browser to become available for more page launching.
// This is pretty much voodoo.
// hProcess should be a handle to the browser process, but can be 0.
void WaitForProcessToQuiesce(HANDLE hProcess)
{
    if (hProcess) ::WaitForInputIdle(hProcess, 10000);
    Sleep(2000);
    if (hProcess) ::WaitForInputIdle(hProcess, 10000);
}

// Get the command line for the default browser (should include URL placeholder,
// but that depends on what's configured in the system).
bool GetDefaultBrowserCommandLine(tstring& o_commandLine)
{
    o_commandLine.clear();

    // Get the command line for the associated browser.

    TCHAR sBuffer[MAX_PATH] = { 0 };
    DWORD dwSize = MAX_PATH;

    HRESULT hr = AssocQueryString(
        ASSOCF_INIT_DEFAULTTOSTAR,
        ASSOCSTR_COMMAND,
        _T(".htm"),
        NULL,
        sBuffer,
        &dwSize);

    if (hr != S_OK)
    {
        my_print(NOT_SENSITIVE, true, _T("%s: AssocQueryString failed; hr:%d; GetLastError:%d"), __TFUNCTION__, (int)hr, GetLastError());
        return false;
    }

    o_commandLine = sBuffer;
    return true;
}

void OpenBrowser(const tstring& url)
{
    return;
}

void OpenBrowser(const vector<tstring>& urls)
{
    return;
}
