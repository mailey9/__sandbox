#pragma once

/*
    * DynArray<T>

    @ desc

    @ todo

    @ issues
        - capacity Ȯ���� ����, �˰� �ִ� �������δ� vector�� capacity Ȯ���� �⺻������
          realloc �� ������ ���� �����ϴ�. ( Ȯ�� ������ �κ����� ã�Ƽ� �ű�ٴ� ��. )

          �� �޸� ���̾ƿ��� '����' �ϴ� �������� �� ���̳��� �� ������ �־��� ��. (move semantics ����)
                -- ������ �𸣰ڴ�, �׳� ��¥ ����ƴѰ�? ������ �� ������ �ʿ������ �����ٵ�?
                -- iterator �� ���谡 �ִ� �����ϱ�? ��Ȯ�� ���θ� �غ��� ū ������ �� �� �ϴ�.
                    --> �ƴϴ�, ^^; �߰� ����/���� �������� ��¥�� �޸𸮰� �̵��ϴ� ������ ����.
                        �̰� �� memmove() �� ���õȴ�. ������ �ǹ��� ���´�.
                    --> ���� �迭��Ұ� �ٸ� ���ι迭��Ҹ� ����Ű�� ���� ���� �����ϱ�?
                        �޸� �������� �����־�� �ϴϱ�..
                -- ���� �� realloc ���� ������ �ý��ۿ� �������̴�. ��, "���ܸ� ���� �� �ִ�" �Լ���.

          ����, capa�� 10�̾��ٸ�, �������� Ȯ���� '���� x 2 = 20' ���� �ؼ� ��Ƶд�.
          ( �ֳ��ϸ� ���� ������ ����� '���̳���' �ϱ� ����., ���� os �����Ʈ�� heap alloc�� ��¥�� ������. )

        - ��, �� double-up �� �������Ǹ� ���� ���ΰ�?

        - ����, ���� �� �Ҵ��� 100�� �ְ�, ������ ������ �Ͼ ����� 2~3�� ������ ������ ���� �ִٸ�?
          �̴� 97���� ������ �����ϰ� �ִ� ���̴�. ��, "shrink()" �� �ʿ��ϴٴ� ��.

        - �� operation ���� th-safe �Ѱ�? �̰͵� ����غ����Ѵ�.




        2) realloc �� new T[];
            - ����ġ�� ���� ����.. �����ڸ� �θ����� new [] ���۷����͸� �̿��ؾ��ϹǷ�..
              
              Q. new[] operator �� realloc �� ���� ������ �����ϴ°�?
              ����: SO, <What is C++ version of realloc(),
                        to allocate the new buffer and copy the contents
                        from the old one?>

              A. short answer : use std::vector<T>

            - ��¥�� �߿��� ��������, "���Կ�����" �� "���������" �� ������.
              ���� ���������� ���Կ����ڸ� �̿��ϴµ�, �� = �� ���ǵǾ� �־�߸� �Ѵٴ� ��.

              InsertAt(), Add() ���� �Լ���..
              int �� �׽�Ʈ�ϴ� �� �Ǵ� ��ó�� ��������,
                1. ���� generic ���� �ʴ�.
                2. ����ȭ�� ���ٸ� (�̵����� ����)
                    - �Լ��� ���� ��
                    - ���� ���꿡��       �� 2ȸ�� "����" �� �Ͼ�� �ȴ�.
                3. ���ο��� �����Ҵ��� �� �Ͼ�� �� �ٸ� Ŭ������ �����غ��ƾ� �Ѵ�.

              ��, �̳� ��ȿ�����̴�. STL ¯¯���̴ϱ� STL ����.
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

            //  "��ġ" ���� "��" �� ������ ���̸� ���.
            //  "��ġ+1" ���� "��+1" �� �ű��. memmove();
            //  "��ġ" �� �����͸� �����Ѵ�.

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