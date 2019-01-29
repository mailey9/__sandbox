#pragma once

/*
    * DynArray<T>

    @ desc

    @ todo

    @ issues
        - capacity 확장의 문제, 알고 있는 지식으로는 vector의 capacity 확장은 기본적으로
          realloc 과 동작이 거의 유사하다. ( 확장 가능한 부분으로 찾아서 옮긴다는 것. )

          단 메모리 레이아웃을 '복사' 하는 과정에서 좀 다이나믹 한 과정이 있었던 듯. (move semantics 이전)
                -- 이유를 모르겠다, 그냥 통짜 복사아닌가? 개별로 다 복사할 필요까지는 없을텐데?
                -- iterator 와 관계가 있는 문제일까? 정확한 공부를 해보면 큰 도움이 될 듯 하다.
                    --> 아니다, ^^; 중간 삽입/삭제 과정에서 통짜로 메모리가 이동하는 과정이 문제.
                        이건 또 memmove() 와 관련된다. 여전히 의문은 남는다.
                    --> 내부 배열요소가 다른 내부배열요소를 가리키는 경우와 같은 문제일까?
                        메모리 오프셋을 정해주어야 하니까..
                -- 또한 이 realloc 관련 동작은 시스템에 의존적이다. 즉, "예외를 던질 수 있는" 함수다.

          또한, capa가 10이었다면, 여유공간 확보를 '현재 x 2 = 20' 정도 해서 잡아둔다.
          ( 왜냐하면 위의 과정이 상당히 '다이나믹' 하기 때문., 또한 os 디펜던트한 heap alloc은 진짜로 느리다. )

        - 즉, 이 double-up 의 동작정의를 따를 것인가?

        - 만약, 과정 중 할당을 100개 넣고, 실제로 삭제가 일어나 현재는 2~3개 정도의 공간을 쓰고 있다면?
          이는 97개의 공간을 낭비하고 있는 것이다. 즉, "shrink()" 가 필요하다는 것.

        - 이 operation 들이 th-safe 한가? 이것도 고민해봐야한다.




        2) realloc 과 new T[];
            - 생각치도 못한 문제.. 생성자를 부르려면 new [] 오퍼레이터를 이용해야하므로..
              
              Q. new[] operator 가 realloc 과 같은 식으로 동작하는가?
              참조: SO, <What is C++ version of realloc(),
                        to allocate the new buffer and copy the contents
                        from the old one?>

              A. short answer : use std::vector<T>

            - 진짜로 중요한 문제점은, "대입연산자" 와 "복사생성자" 의 문제다.
              지금 구현에서는 대입연산자를 이용하는데, 즉 = 이 정의되어 있어야만 한다는 것.

              InsertAt(), Add() 등의 함수가..
              int 로 테스트하니 잘 되는 것처럼 보이지만,
                1. 전혀 generic 하지 않다.
                2. 최적화가 없다면 (이동연산 등의)
                    - 함수로 들어올 때
                    - 대입 연산에서       총 2회의 "복사" 가 일어나게 된다.
                3. 내부에서 동적할당이 또 일어나는 또 다른 클래스를 생각해보아야 한다.

              즉, 겁나 비효율적이다. STL 짱짱맨이니까 STL 쓰자.
*/

#include <memory.h>
#include <stdlib.h>
#include <assert.h>

#include <memory>


namespace MinorLife
{   
    template <typename T>
    class DynArray
    {
        //static constexpr int default_capacity = 10;

    private:
        T* datas;

        int length;
        int capacity;
    public:
        DynArray()
            : datas( nullptr )
            , length( 0 )
            , capacity( 0 )
        {}

        DynArray(int capacity_)
            : length( 0 )
            , capacity( 0 )
        {
            SetCapacity( capacity );
        }

        ~DynArray()
        {
            if ( datas != nullptr )
            {
                ::free( datas );
            }
        }
    public:
        const int Length() const
        {
            return length;
        }

        const int Capacity() const
        {
            return capacity;
        }
        bool SetCapacity(int n)
        {
            if ( n <= capacity )
            {
                return true;
            }
            if ( n < length )
            {
                return false;
            }

            datas = reinterpret_cast<T*>(
                ::realloc( datas, n * sizeof(T) )
                );

            // allocation failed, as default behavior, error realloc's RV is 'null'.
            assert( datas != nullptr );

            capacity = n;
            return true;
        }

        void Shrink()
        {
            datas = reinterpret_cast<T*>(
                ::realloc( datas, length * sizeof(T) )
                );

            assert( datas != nullptr );
        }
        void Exapand()
        {
            if ( capacity > 0 )
            {
                SetCapacity( 2 * capacity );
            }
            else
            {
                SetCapacity( 1 );
            }
        }

        void Add(T data)
        {
            printf("Add( T )\n");
            if ( length >= capacity )
            {
                //SetCapacity( length + 1 );
                Exapand();
            }

            datas[ length ] = data; //equals to: *(datas + length) = data;

            length += 1;
        }

        void Emplace(T&& data)
        {
            printf("Emplace( T&& )\n");
            if ( length >= capacity )
            {
                Exapand();
            }

            
            datas[ length ] = std::forward<T>( data );
            length += 1;
        }

        bool InsertAt(T data, int index)
        {
            if ( index < 0 || index >= length )
            {
                // err, out of index.
                return false;
            }
            if ( length >= capacity )
            {
                Exapand();
            }

            //  "위치" 부터 "끝" 쪽 까지의 길이를 계산.
            //  "위치+1" 에서 "끝+1" 로 옮긴다. memmove();
            //  "위치" 에 데이터를 삽입한다.

            int nMove = length - index;
            ::memmove( datas + index + 1, datas + index, nMove * sizeof(T) );
            datas[ index ] = data;
            length += 1;

            return true;
        }
    public:
        // *operator overloads
        T& operator[]( int index )
        {
            assert( index >= 0 && index < length );
            return datas[ index ];
        }
    };
}