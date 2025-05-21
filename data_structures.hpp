
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

            // הוספת קונסטרקטור העתקה
            Node(const Node &other) : data(other.data), next(nullptr) {}
        };

        Node *head;

        // פונקציה פרטית להעתקת רשימה
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

        // פונקציה פרטית לניקוי רשימה
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

    public:
        List() : head(nullptr) {}

        // קונסטקטור העתקה
        List(const List<T> &other) : head(nullptr)
        {
            copyList(other);
        }

        // אופרטור השמה
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
        { // הוספת const
            if (index < 0)
                throw std::invalid_argument("Index cannot be negative");

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
                throw std::invalid_argument("Index cannot be negative");

            Node *curr = head;
            int i = 0;
            while (curr && i < index)
            {
                curr = curr->next;
                i++;
            }

            if (!curr)
                throw std::invalid_argument("Index out of bounds");

            return curr->data;
        }

        void remove(int index)
        {
            if (index < 0 || !head)
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
            if (!temp)
                throw std::invalid_argument("Index out of range");
            curr->next = temp->next;
            delete temp;
        }

        int size() const
        { // הוספת const
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
        { // הוספת const
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

        // קונסטרקטור העתקה
        Queue(const Queue<T> &other) : list(other.list) {}

        // אופרטור השמה
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
        { // הוספת const
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

        // קונסטרקטור העתקה
        PriorityQueue(const PriorityQueue<T> &other) : list(other.list) {}

        // אופרטור השמה
        PriorityQueue<T> &operator=(const PriorityQueue<T> &other)
        {
            if (this != &other)
            {
                list = other.list;
            }
            return *this;
        }

        void insert(T val)
        {
            int i = 0;
            while (i < list.size() && list.get(i) <= val)
            {
                i++;
            }
            list.add(i, val);
        }

        T extractMin()
        {
            if (isEmpty())
                throw std::runtime_error("Priority queue is empty");

            T val = list.get(0);
            list.remove(0);
            return val;
        }

        bool isEmpty() const
        { // הוספת const
            return list.isEmpty();
        }
    };

    class UnionFind
    {
    private:
        List<int> parent;
        List<int> rank;

    public:
        UnionFind(int size)
        {
            for (int i = 0; i < size; ++i)
            {
                parent.push(i);
                rank.push(0);
            }
        }

        // קונסטרקטור העתקה
        UnionFind(const UnionFind &other) : parent(other.parent), rank(other.rank) {}

        // אופרטור השמה
        UnionFind &operator=(const UnionFind &other)
        {
            if (this != &other)
            {
                parent = other.parent;
                rank = other.rank;
            }
            return *this;
        }

        int find(int x)
        {
            if (x < 0 || x >= parent.size())
            {
                throw std::out_of_range("Index out of range");
            }

            if (parent.get(x) != x)
            {
                int rep = find(parent.get(x));
                parent.set(x, rep);
            }
            return parent.get(x);
        }

        bool unionSets(int a, int b)
        {
            if (a < 0 || a >= parent.size() || b < 0 || b >= parent.size())
            {
                throw std::out_of_range("Index out of range");
            }

            int rootA = find(a);
            int rootB = find(b);

            if (rootA == rootB)
                return false;

            if (rank.get(rootA) < rank.get(rootB))
            {
                parent.set(rootA, rootB);
            }
            else if (rank.get(rootA) > rank.get(rootB))
            {
                parent.set(rootB, rootA);
            }
            else
            {
                parent.set(rootB, rootA);
                rank.set(rootA, rank.get(rootA) + 1);
            }

            return true;
        }
    };
}

#endif