//  *no PCH
//  *subsys: console
//      ... vs2017(v141), sdk 10.0.17763.0
//      ... other settings are following defaults.

#include <Windows.h>

int main()
{
    MessageBox(nullptr, TEXT("Hello World!"), TEXT("2019 . 1 . 29"), MB_OK);

    return 0;
}