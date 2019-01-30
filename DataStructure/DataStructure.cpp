//  * ��Ʈ, 2019-01-30-18:34
//
//  @ studied
//      1. capacity Ȯ�� ����
//          - new int[] �� realloc( n*sizoef(int) ) �� ������?
//          - �ڷ����� * �� �����Ѵٸ�,
//            ������ �� perfectly foward �� �� �־�� emplace �� �ȴ�.
//            (move semantics �� ���ذ� �� �ʿ�, "magic" �� �ƴϴ�.)
//
//          * Answer to All
//              - std::vector<T> ���.
//
//      2. �ݺ���, general iterator
//          - �ݺ��� ���� (iterator pattern) ���ΰ� �ʿ�.
//          - STL �� �ݺ��� �� �ƴ϶�, MFC �� POSITION � ���� ����.
//          - STL �� non-sequence(==associated) container ��
//            std::map ��� ������ �ȴ�. how come?, �з��� ���� Ư��ȭ?
//          - STL iterator �� ��ȿȭ review �ʿ�. (std::list vs std::vector ��.)
//
//      3. �������̽� ���
//          - ������ �� �ʿ��Ѱ�?
//
//      4. move semantics
//          - ������ �򰥸���.
//          - Ư�� ������ ���� �� ��
//              1) when does move constructor should be defined?
//              2) when does move assginment operator should be defined?

#include <stdio.h>
#include <memory>

#include "TListNode.h"
#include "TDynArray.h"

class SomeClass
{
private:
    int m;
public:
    SomeClass()
        : m( -1 )
    {
        printf("Default Constructor.\n");
    }

    SomeClass(int i)
        : m( i )
    {
        printf("Explicit Constructor. %d\n", m);
    }

    SomeClass(const SomeClass& copy)
    {
        printf("Copy Constructor.\n");
        m = copy.m;
    }
    
    SomeClass(SomeClass&& move)
    {
        printf("Move Constructor.\n");
        m = std::move( move.m );
    }

    ~SomeClass()
    {
        printf("%p Destructor.\n", this);
    }

    SomeClass& operator= (const SomeClass& other)
    {
        printf("copy operator=\n");
        m = other.m;

        return *this;
    }

    SomeClass& operator= (SomeClass&& mov)
    {
        printf("move operator=\n");
        m = std::move( mov.m );

        return *this;
    }

    void print()
    {
        printf("%d ", m);
    }
};

int main()
{
    printf("Hello World!\r\n");

    /*MinorLife::NodeList<int> myList;

    myList.Add( 5 );
    myList.Add( 10 );
    myList.Add( 15 );

    myList.RemoveAt( 1 );*/

    MinorLife::DynArray<int> myArray( 10 );
    
    myArray.Add( 1 );
    myArray.Add( 2 );
    myArray.Add( 3 );
    myArray.Add( 4 );
    myArray.InsertAt( 5, 1 );

    myArray.Shrink();

    for (int i = 0; i < myArray.Length(); ++i)
    {
        printf("%d ", myArray[ i ]);
    }
    printf("\n - - - - - - - - - - -\n");


    MinorLife::DynArray<SomeClass> myClasses( 5 );

    auto a = SomeClass( 100 );
    myClasses.Add( SomeClass() );
    myClasses.Add( SomeClass(1) );
    myClasses.Add( SomeClass(2) );
    myClasses.Add( SomeClass(3) );
    /*myClasses.Emplace( SomeClass() );
    myClasses.Emplace( SomeClass(1) );
    myClasses.Emplace( SomeClass(2) );
    myClasses.Emplace( SomeClass(3) );*/

    // copy ellision?   no calls of constructor.
    //                  but does call the 'operator=( move )'
    // may help: https://stackoverflow.com/questions/27888873/copy-vs-stdmove-for-ints
    myClasses.Add( std::move(a) );
    //myClasses.Emplace( std::move(a) );

    //  parameter �� ���� ���� ��� copy �Ǵ� move constructor ȣ���� ����.
    //  vs2017 �̶� ����ȭ�� �ڵ����� ���ִ� ��. debug, release ��� ����.

    for (int i = 0; i < myClasses.Length(); ++i)
    {
        myClasses[ i ].print();
    }


    system("pause");
}


