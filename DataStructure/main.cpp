#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

namespace MinorLife
{
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

            for (int i = 0; i < index; ++i)
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

        //  Clear();
        //  InsertAt( int index );

        //  Front();    // getter
        //  End();      // getter
        //  Length();   // getter

        //  Remove( Node* node )    // remove the node.

        //  Sort( comparer ); // maybe later ..
    };
}

int main()
{
    printf("Hello World!\r\n");

    MinorLife::NodeList<int> myList;

    myList.Add( 5 );
    myList.Add( 10 );
    myList.Add( 15 );

    myList.RemoveAt( 1 );


    //printf("Hello World!\r\n");
}
