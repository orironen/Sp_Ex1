#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

#include <stdexcept>

namespace structures
{
    template <typename T>
    class List
    {
    private:
        struct Node
        {
            T data;
            Node *next;
            Node(T val) : data(val), next(nullptr) {}
            Node(const Node &other) : data(other.data), next(nullptr) {}
        };
        Node *head;

    public:
        List() : head(nullptr) {}
        List(const List<T> &other) : head(nullptr)
        {
            copyList(other);
        }

        List<T> &operator=(const List<T> &other)
        {
            if (this != &other)
            {
                clearList();
                copyList(other);
            }
            return *this;
        }

        ~List()
        {
            clearList();
        }
        // Copying a list
        void copyList(const List<T> &other)
        {
            if (!other.head)
                return;
            head = new Node(other.head->data);
            Node *curr = head;
            Node *otherCurr = other.head->next;
            while (otherCurr)
            {
                curr->next = new Node(otherCurr->data);
                curr = curr->next;
                otherCurr = otherCurr->next;
            }
        }
        // Clearing the list
        void clearList()
        {
            Node *curr = head;
            while (curr)
            {
                Node *temp = curr->next;
                delete curr;
                curr = temp;
            }
            head = nullptr;
        }

        void push(T val)
        {
            Node *newNode = new Node(val);
            if (!head)
            {
                head = newNode;
            }
            else
            {
                Node *curr = head;
                while (curr->next)
                {
                    curr = curr->next;
                }
                curr->next = newNode;
            }
        }

        T get(int index) const
        {
            if (index < 0)
                throw std::invalid_argument("Index can't be negative");
            Node *curr = head;
            for (int i = 0; i < index; ++i)
            {
                if (!curr)
                    throw std::invalid_argument("Index out of range");
                curr = curr->next;
            }
            if (!curr)
                throw std::invalid_argument("Index out of range");
            return curr->data;
        }

        T &getRef(int index)
        {
            if (index < 0)
                throw std::invalid_argument("Index can't be negative");
            Node *curr = head;
            int i = 0;
            while (curr && i < index)
            {
                curr = curr->next;
                i++;
            }
            if (!curr)
                throw std::invalid_argument("Index out of range");
            return curr->data;
        }

        void remove(int index)
        {
            if (index < 0)
                throw std::invalid_argument("Index out of range");
            if (index == 0)
            {
                Node *temp = head;
                head = head->next;
                delete temp;
                return;
            }
            Node *curr = head;
            for (int i = 0; i < index - 1; ++i)
            {
                if (!curr->next)
                    throw std::invalid_argument("Index out of range");
                curr = curr->next;
            }
            Node *temp = curr->next;
            curr->next = temp->next;
            delete temp;
        }

        int size() const
        {
            int count = 0;
            Node *curr = head;
            while (curr)
            {
                count++;
                curr = curr->next;
            }
            return count;
        }
        void add(int index, T value)
        {
            if (index < 0)
                throw std::invalid_argument("Invalid index");
            Node *newNode = new Node(value);
            if (index == 0)
            {
                newNode->next = head;
                head = newNode;
                return;
            }
            Node *curr = head;
            for (int i = 0; i < index - 1; ++i)
            {
                if (!curr)
                    throw std::invalid_argument("Index out of range");
                curr = curr->next;
            }
            if (!curr)
                throw std::invalid_argument("Index out of range");
            newNode->next = curr->next;
            curr->next = newNode;
        }

        void set(int index, T value)
        {
            if (index < 0)
                throw std::invalid_argument("Index out of range");
            Node *curr = head;
            for (int i = 0; i < index; ++i)
            {
                if (!curr)
                    throw std::invalid_argument("Index out of range");
                curr = curr->next;
            }
            if (!curr)
                throw std::invalid_argument("Index out of range");
            curr->data = value;
        }

        bool isEmpty() const
        {
            return head == nullptr;
        }
    };

    template <typename T>
    class Queue
    {
    private:
        List<T> list;

    public:
        Queue() {}

        Queue(const Queue<T> &other) : list(other.list) {}
        Queue<T> &operator=(const Queue<T> &other)
        {
            if (this != &other)
            {
                list = other.list;
            }
            return *this;
        }

        void enqueue(T val)
        {
            list.push(val);
        }
        T dequeue()
        {
            if (isEmpty())
                throw std::runtime_error("Queue is empty");
            T val = list.get(0);
            list.remove(0);
            return val;
        }

        bool isEmpty() const
        {
            return list.isEmpty();
        }
    };

    template <typename T>
    class PriorityQueue
    {
    private:
        List<T> list;

    public:
        PriorityQueue() {}
        PriorityQueue(const PriorityQueue<T> &other) : list(other.list) {}
        PriorityQueue<T> &operator=(const PriorityQueue<T> &other)
        {
            if (this != &other)
            {
                list = other.list;
            }
            return *this;
        }
        // Adding an element to the queue at its position
        void insert(T val)
        {
            int i = 0;
            while (i < list.size() && list.get(i) <= val)
            {
                i++;
            }
            list.add(i, val);
        }
        // Removing the smallest element from the queue
        T extractMin()
        {
            if (isEmpty())
                throw std::runtime_error("Priority queue is empty");
            T val = list.get(0);
            list.remove(0);
            return val;
        }
        bool isEmpty() const
        {
            return list.isEmpty();
        }
    };

    class UnionFind
    {
    private:
        List<int> parent; // A list of the parent of each element
        List<int> rank;   // Height of the tree

    public:
        // Initially, each element is in its own set
        UnionFind(int size)
        {
            for (int i = 0; i < size; ++i)
            {
                parent.push(i);
                rank.push(0);
            }
        }

        UnionFind(const UnionFind &other) : parent(other.parent), rank(other.rank) {}
        UnionFind &operator=(const UnionFind &other)
        {
            if (this != &other)
            {
                parent = other.parent;
                rank = other.rank;
            }
            return *this;
        }

        // Finding the root of the element, including path compression
        int find(int x)
        {
            if (x < 0 || x >= parent.size())
            {
                throw std::out_of_range("Index out of range");
            }
            if (parent.get(x) != x)
            {
                int root = find(parent.get(x));
                parent.set(x, root); // Set the new root as the parent
            }
            return parent.get(x);
        }
        // Union of two elements into one set
        bool unionSets(int a, int b)
        {
            if (a < 0 || a >= parent.size() || b < 0 || b >= parent.size())
            {
                throw std::out_of_range("Index out of range");
            }
            int rootA = find(a);
            int rootB = find(b);
            // If they are already in the same set, no need to unite
            if (rootA == rootB)
                return false;
            // Otherwise, attach the smaller tree to the larger one
            if (rank.get(rootA) < rank.get(rootB))
            {
                parent.set(rootA, rootB);
            }
            else if (rank.get(rootA) > rank.get(rootB))
            {
                parent.set(rootB, rootA);
            }
            else
            // Merging two trees of equal height
            {
                parent.set(rootB, rootA);
                rank.set(rootA, rank.get(rootA) + 1);
            }

            return true;
        }
    };
}

#endif
