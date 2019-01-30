//  * 노트, 2019-01-30-18:34
//
//  @ studied
//      1. capacity 확장 문제
//          - new int[] 가 realloc( n*sizoef(int) ) 과 같은가?
//          - 자료형을 * 로 관리한다면,
//            들어오는 걸 perfectly foward 할 수 있어야 emplace 가 된다.
//            (move semantics 의 이해가 더 필요, "magic" 이 아니다.)
//
//          * Answer to All
//              - std::vector<T> 써라.
//
//      2. 반복자, general iterator
//          - 반복자 패턴 (iterator pattern) 공부가 필요.
//          - STL 의 반복자 뿐 아니라, MFC 의 POSITION 등도 좋은 공부.
//          - STL 은 non-sequence(==associated) container 인
//            std::map 등에도 대응이 된다. how come?, 분류에 따른 특수화?
//          - STL iterator 의 무효화 review 필요. (std::list vs std::vector 등.)
//
//      3. 인터페이스 고려
//          - 무엇이 더 필요한가?
//
//      4. move semantics
//          - 아직도 헷갈린다.
//          - 특히 눈여겨 봐야 할 점
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

    //  parameter 에 대해 눈에 띄는 copy 또는 move constructor 호출이 없다.
    //  vs2017 이라 최적화를 자동으로 해주는 듯. debug, release 모두 같다.

    for (int i = 0; i < myClasses.Length(); ++i)
    {
        myClasses[ i ].print();
    }


    system("pause");
}


