#pragma once

// @ desc
//      - 양방향 연결리스트

// @ todo
//      - interface 몇 개
//          1. 조회
//          2. 순회
//          3. 정렬

#include <stdlib.h>

namespace MinorLife
{
    //  '조회/순회'에 대해 두 가지의 디자인 결정사항이 있다.
    //      1) NodeList 에서 Node* 리턴.
    //          - 가장 쉬운 방법
    //          - Node* 자체를 노출시킴으로써, encapsulation 이 상한다.
    //              > 이를 해결하기 위해 Node* 의 직접 조작이 가능한 것은
    //                friend 등으로 NodeList; 또는 해당 함수에게만 노출하는 방법이 있다.
    //      2) 반복자의 도입
    //          - iterator 인터페이스를 만들어 노출시키는 것.
    //          - 정형화된 패턴이다.
    //          - c++ 의 기본 반복자는 상속구분으로 이를 잘 써먹고 있다.

    /* Node */
    template <typename T>
    struct Node
    {
        T data;
        Node* prev;
        Node* next;

        Node()
            : prev( nullptr )
            , next( nullptr )
        {}

        static Node* Create(T data_)
        {
            Node* newNode = reinterpret_cast<Node<T>*>( malloc( sizeof(Node<T>) ) );

            newNode->data = data_;
            newNode->prev = nullptr;
            newNode->next = nullptr;

            return newNode;
        }
    private:
        Node& operator=(const Node& other)  = delete;
        Node(const Node& copy)              = delete;
    };

    /* List, Doubly Linked List */
    template <typename T>
    class NodeList
    {
    private:
        int length;

        Node<T>* front;
        Node<T>* end;
    public:
        NodeList()
            : length( 0 )
            , front( nullptr )
            , end( nullptr )
        {}

        ~NodeList()
        {
            while ( length > 0 )
            {
                RemoveAt( 0 );
            }
        }
    public:
        void Add(T data)
        {
            Node<T>** curr = &front;
            Node<T>** prev = nullptr;

            while ( *curr != nullptr )
            {
                prev = curr;    // saved prev.
                curr = &( (*curr)->next );
            }

            Node<T>* newNode = Node<T>::Create( data );

            *curr = newNode;

            if ( prev != nullptr )
            {
                if ( *prev != nullptr )
                {
                    (*curr)->prev = *prev;
                }
            }

            end = *curr;

            length += 1;
        }
        bool RemoveAt(int index)
        {
            if ( index < 0 || index > length )
            {
                // out of index
                return false;
            }

            Node<T>* target = front;

            for ( int i = 0; i < index; ++i )
            {
                target = target->next;
            }

            Node<T>* prevSaved = target->prev;
            Node<T>* nextSaved = target->next;

            if ( prevSaved == nullptr )
            {
                front = nextSaved;
            }
            else
            {
                prevSaved->next = nextSaved;
            }

            ::free( target );

            length -= 1;

            return true;
        }

        void Clear()
        {
            while ( length > 0 )
            {
                RemoveAt( 0 );
            }
        }
        const int Length() const
        {
            return length;
        }

        //  Front();    // getter, iterator
        //  End();      // getter, iterator

        //  Remove( Node* node )    // *d, remove 'the' node.
        //  Sort( comparer );       // *d, maybe later ...
    };
}