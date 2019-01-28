#include <stdio.h>

#include "TListNode.h"
#include "TDynArray.h"

int main()
{
    printf("Hello World!\r\n");

    MinorLife::NodeList<int> myList;

    myList.Add( 5 );
    myList.Add( 10 );
    myList.Add( 15 );

    myList.RemoveAt( 1 );
}
