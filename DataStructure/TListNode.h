#pragma once

// @ desc
//      - ����� ���Ḯ��Ʈ

// @ todo
//      - interface �� ��
//          1. ��ȸ
//          2. ��ȸ
//          3. ����

#include <stdlib.h>

namespace MinorLife
{
    //  '��ȸ/��ȸ'�� ���� �� ������ ������ ���������� �ִ�.
    //      1) NodeList ���� Node* ����.
    //          - ���� ���� ���
    //          - Node* ��ü�� �����Ŵ���ν�, encapsulation �� ���Ѵ�.
    //              > �̸� �ذ��ϱ� ���� Node* �� ���� ������ ������ ����
    //                friend ������ NodeList; �Ǵ� �ش� �Լ����Ը� �����ϴ� ����� �ִ�.
    //      2) �ݺ����� ����
    //          - iterator �������̽��� ����� �����Ű�� ��.
    //          - ����ȭ�� �����̴�.
    //          - c++ �� �⺻ �ݺ��ڴ� ��ӱ������� �̸� �� ��԰� �ִ�.

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