//  @ testing environment
//      - windows 7 ultimate K, SP1 (KOR)

#include <stdio.h>
#include <tchar.h>

#include <iostream>

#include <Windows.h>

#include "Console.h"


void PrintConsoleCP()
{
    printf("CP:%d, Output CP:%d\r\n"
        , GetConsoleCP()
        , GetConsoleOutputCP() );
}

//CODEPAGE_ENUMPROC
BOOL CALLBACK EnumCodePagesProc( _In_ LPTSTR lpCodePageString)
{
    static int callCount = 0;
    printf("%d : ", callCount++);
    _tprintf(_T("%s\n"), lpCodePageString);
    
    return FALSE;
}

int main()
{
    CONSOLE_FONT_INFOEX info = { 0, };
    info.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    info.FontWeight = FW_DONTCARE;//FW_NORMAL;//아마도 안되는 듯?
    info.dwFontSize.X = 8;
    info.dwFontSize.Y = 8;  // expeting 8x8
    wcscpy_s(info.FaceName, L"Terminal");
    
    //
    printf("Hello World!\r\n\n");

    BOOL ret = IsValidCodePage(65001);
    printf("____for 65001 returns: %d\r\n", ret);

    SetConsoleCP(949);
    SetConsoleOutputCP(949);

    PrintConsoleCP();

    SetConsoleCP(437);
    SetConsoleOutputCP(437);
    //  SetConsoleCP(65001);
    //  SetConsoleOutputCP(65001);
    //  printf(u8"Hello월드");
    // 당연히 될리가 없다, 폰트가 없으니까. -- 나무위키 서술이 틀리다. -- 윈10은 다른가? 그럴 수 있겠다.

    SetCurrentConsoleFontEx( GetStdHandle(STD_OUTPUT_HANDLE), false, &info );
    COORD max = GetLargestConsoleWindowSize( GetStdHandle(STD_OUTPUT_HANDLE) );

    PrintConsoleCP();

    // putchar test
    printf("\n\xdb\xdb\n");
    printf("\nmax: %d %d\n", max.X, max.Y);

    char buf[512] = { 0, };
    scanf_s("%s", buf, _countof(buf));
    printf("echo: %s", buf);

    // resizing buffer & window
    CONSOLE_SCREEN_BUFFER_INFOEX csbi_ex;
    csbi_ex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

    GetConsoleScreenBufferInfoEx( GetStdHandle(STD_OUTPUT_HANDLE), &csbi_ex );
    csbi_ex.dwMaximumWindowSize.X = csbi_ex.dwSize.X = 192;
    csbi_ex.dwMaximumWindowSize.Y = csbi_ex.dwSize.Y = 108;
    SetConsoleScreenBufferInfoEx( GetStdHandle(STD_OUTPUT_HANDLE), &csbi_ex );

    SMALL_RECT sm = { 0,0, 192 - 1, 108 - 1 };
    SetConsoleWindowInfo( GetStdHandle(STD_OUTPUT_HANDLE), true, &sm );

    // centralizing..
    printf("--centralizing--\n\n");
    Console::CentralizeConsoleWindow();
    
    GetConsoleScreenBufferInfoEx( GetStdHandle(STD_OUTPUT_HANDLE), &csbi_ex );
    system("pause");

    return 0;
}