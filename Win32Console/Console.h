#pragma once

#include <stdio.h>
#include <assert.h>
#include <Windows.h>

namespace Console
{
    void CheckError(BOOL ret, const char* errMessage)
    {
        char errCode[ 32 ] = { 0 };
        sprintf_s( errCode, "%d", GetLastError() );

        if ( ret == FALSE )
        {
            MessageBoxA( nullptr, errMessage, errCode, MB_OK | MB_ICONEXCLAMATION );
        }
    }

    void CentralizeConsoleWindow()
    {
        HWND hConsole = GetConsoleWindow();
        HWND hDesktop = GetDesktopWindow();
        
        RECT rcConsole = { 0, };
        RECT rcDesktop = { 0, };

        BOOL res = FALSE;

        assert( hConsole != nullptr );
        assert( hDesktop != nullptr );

        res = GetWindowRect( hDesktop, &rcDesktop );
        CheckError( res, "GetWindowRect()" );
        /*int widthDesktop = rcDesktop.right - rcDesktop.left;
        int heightDesktop = rcDesktop.bottom - rcDesktop.top;*/

        res = GetWindowRect( hConsole, &rcConsole );
        CheckError( res, "GetWindowrect()" );

        // HWND_TOPMOST == always on top.
        // SWP_NOSIZE ignores cx, cy memebers of function
        //            also makes the window focused.
        SetWindowPos( hConsole, HWND_TOP
            , ((rcDesktop.right - rcDesktop.left) / 2)
              - ((rcConsole.right - rcConsole.left) / 2)
            , ((rcDesktop.bottom - rcDesktop.top) / 2)
              - ((rcConsole.bottom - rcConsole.top) / 2)
            , 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW );
    }
}