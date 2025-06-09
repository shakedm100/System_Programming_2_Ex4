// shaked1mi@gmail.com

#pragma once
#include <algorithm>
#include <ostream>
#include <stdexcept>
#include <vector>

namespace customContainer
    {
    template<typename T = int>
    class MyContainer
    {
    private:
        struct Node
        {
            T data;
            Node *next;

            explicit Node(const T &data) : data(data), next(nullptr)
            {
            }
        };

        Node *head;

    public:
        MyContainer() : head(nullptr)
        {
        }

        ~MyContainer()
        {
            if (head != nullptr)
            {
                Node *tail = head;
                while (head != nullptr)
                {
                    head = head->next;
                    delete tail;
                    tail = head;
                }
            }
        }

        void add(T data)
        {
            if (head == nullptr)
            {
                head = new Node(data);
                return;
            }

            Node *temp = head;
            while (temp->next)
                temp = temp->next;

            temp->next = new Node(data);
        }

        void remove(const T &data)
        {
            if (head == nullptr)
                throw std::out_of_range("Container is empty");

            bool found = false;
            while (head && head->data == data)
            {
                Node *tmp = head;
                head = head->next;
                delete tmp;
                found = true;
            }

            // remove matches in the rest of the list
            Node *cur = head;
            while (cur && cur->next)
            {
                if (cur->next->data == data)
                {
                    Node *toDel = cur->next;
                    cur->next = toDel->next; // skip it
                    delete toDel;
                    found = true;
                }
                else
                {
                    cur = cur->next;
                }
            }

            if (!found)
                throw std::out_of_range("Element not found");
        }

        std::size_t size() const
        {
            if (head == nullptr)
                return 0;

            int count = 0;
            Node *temp = head;
            while (temp != nullptr)
            {
                count++;
                temp = temp->next;
            }

            return count;
        }

        friend std::ostream &operator<<(std::ostream &os, const MyContainer<T> &container)
        {
            if (container.head == nullptr)
                return os;

            Node *temp = container.head;
            os << "[";
            while (temp != nullptr)
            {
                os << temp->data << ", ";
                temp = temp->next;
            }
            os << "]";

            return os;
        }

        class AscendingOrder
        {
        private:
            MyContainer *container;
            std::vector<Node *> sortedList;
            std::size_t index;

        public:
            explicit AscendingOrder() : container(nullptr), index(0)
            {
            }

            AscendingOrder(MyContainer &container, bool atBegin) : container(&container), index(0)
            {
                Node *temp = container.head;
                while (temp != nullptr)
                {
                    // Add all nodes to the list
                    sortedList.push_back(temp);
                    temp = temp->next;
                }

                // Sort the list by ascending order as defined in the function
                std::sort(sortedList.begin(), sortedList.end(),
                          [](Node *a, Node *b) { return a->data < b->data; });

                // If the constructor was called from end then return the index past the last
                if (!atBegin)
                    index = sortedList.size();
            }

            AscendingOrder &operator++()
            {
                ++index;
                return *this;
            }

            T &operator*() const
            {
                return sortedList[index]->data;
            }

            T *operator->() const
            {
                return &(sortedList[index]->data);
            }

            AscendingOrder operator++(int)
            {
                AscendingOrder temp = *this;
                ++(*this);
                return temp;
            }

            bool operator==(const AscendingOrder &other) const
            {
                return index == other.index && container == other.container;
            }

            bool operator!=(const AscendingOrder &other) const
            {
                return !(*this == other);
            }
        };

        AscendingOrder begin_ascending_order() { return AscendingOrder(*this, true); }
        AscendingOrder end_ascending_order() { return AscendingOrder(*this, false); }

        class DescendingOrder
        {
        private:
            MyContainer *container;
            std::vector<Node *> sortedList;
            std::size_t index;

        public:
            explicit DescendingOrder() : container(nullptr), index(0)
            {
            }

            DescendingOrder(MyContainer &container, bool atBegin) : container(&container), index(0)
            {
                Node *temp = container.head;
                while (temp != nullptr)
                {
                    // Add all nodes to the list
                    sortedList.push_back(temp);
                    temp = temp->next;
                }

                // Sort the list by descending order as defined in the function
                std::sort(sortedList.begin(), sortedList.end(),
                          [](Node *a, Node *b) { return a->data > b->data; });

                // If the constructor was called from end then return the index past the last
                if (!atBegin)
                    index = sortedList.size();
            }

            DescendingOrder &operator++()
            {
                ++index;
                return *this;
            }

            T &operator*() const
            {
                return sortedList[index]->data;
            }

            T *operator->() const
            {
                return &(sortedList[index]->data);
            }

            DescendingOrder operator++(int)
            {
                DescendingOrder temp = *this;
                ++(*this);
                return temp;
            }

            bool operator==(const DescendingOrder &other) const
            {
                return index == other.index && container == other.container;
            }

            bool operator!=(const DescendingOrder &other) const
            {
                return !(*this == other);
            }
        };

        DescendingOrder begin_descending_order() { return DescendingOrder(*this, true); }
        DescendingOrder end_descending_order() { return DescendingOrder(*this, false); }


        class SideCrossOrder
        {
        private:
            MyContainer *container;
            std::vector<Node *> crossList;
            std::size_t index;

        public:
            explicit SideCrossOrder() : container(nullptr), index(0)
            {
            }

            SideCrossOrder(MyContainer &container, bool atBegin) : container(&container), index(0)
            {
                Node *temp = container.head;
                while (temp != nullptr)
                {
                    crossList.push_back(temp);
                    temp = temp->next;
                }

                std::sort(crossList.begin(), crossList.end(), [](Node *a, Node *b) { return a->data < b->data; });

                std::vector<Node *> createCross;
                int left = 0, right = crossList.size() - 1;
                while (left <= right)
                {
                    createCross.push_back(crossList[left]);
                    left++;
                    if (left <= right)
                    {
                        createCross.push_back(crossList[right]);
                        --right;
                    }
                }

                crossList = std::move(createCross);

                if (!atBegin)
                    index = crossList.size();
            }

            SideCrossOrder &operator++()
            {
                ++index;
                return *this;
            }

            T &operator*() const
            {
                return crossList[index]->data;
            }

            T *operator->() const
            {
                return &(crossList[index]->data);
            }

            SideCrossOrder operator++(int)
            {
                SideCrossOrder temp = *this;
                ++(*this);
                return temp;
            }

            bool operator==(const SideCrossOrder &other) const
            {
                return index == other.index && container == other.container;
            }

            bool operator!=(const SideCrossOrder &other) const
            {
                return !(*this == other);
            }
        };

        SideCrossOrder begin_side_cross_order() { return SideCrossOrder(*this, true); }
        SideCrossOrder end_side_cross_order() { return SideCrossOrder(*this, false); }

        class ReverseOrder
        {
        private:
            std::vector<Node *> reverseList;
            int index;

        public:
            ReverseOrder() : index(-1) {}

            ReverseOrder(const MyContainer& container, const bool atBegin)
            : index(-1)
            {
                Node *temp = container.head;
                while (temp != nullptr)
                {
                    reverseList.push_back(temp);
                    temp = temp->next;
                }

                if (atBegin)
                    index = reverseList.size() - 1;
                else
                    index = -1;
            }

            T &operator*() const
            {
                return reverseList[index]->data;
            }

            T *operator->() const
            {
                return &reverseList[index]->data;
            }

            // moving forward in the iterator means stepping backwards in the list
            ReverseOrder &operator++()
            {
                --index;
                return *this;
            }

            ReverseOrder operator++(int)
            {
                ReverseOrder tmp = *this;
                --index;
                return tmp;
            }

            // compare indices (we assume same nodes vector)
            bool operator==(const ReverseOrder &o) const
            {
                return index == o.index;
            }

            bool operator!=(const ReverseOrder &o) const
            {
                return index != o.index;
            }
        };

        ReverseOrder begin_reverse_order() { return ReverseOrder(*this, true); }
        ReverseOrder end_reverse_order() { return ReverseOrder(*this, false); }

        class Order
        {
            private:
            Node* current;

            public:
            Order(Node* node = nullptr) : current(node){}

            T& operator*() const
            {
                return current->data;
            }

            T* operator->() const
            {
                return &(current->data);
            }

            Order& operator++()
            {
                current = current->next;
                return *this;
            }

            const Order operator++(int)
            {
                Order tmp = *this;
                ++(*this);
                return tmp;
            }

            bool operator==(const Order &order) const
            {
                return current == order.current;
            }

            bool operator!=(const Order &order) const
            {
                return !(*this == order);
            }
        };

        Order begin_order() { return Order(head); }
        Order end_order() { return Order(nullptr); }

        class MiddleOutOrder
        {
            private:
            std::vector<Node*> middleList;
            std::size_t index;

            public:
            explicit MiddleOutOrder() : index(0) {}

            MiddleOutOrder(const MyContainer& container, const bool atBegin) : index(0)
            {
                Node *temp = container.head;
                std::vector<Node*> allNodes;
                while (temp != nullptr)
                {
                    allNodes.push_back(temp);
                    temp = temp->next;
                }

                if(!allNodes.empty())
                {
                    Node* middle = allNodes[allNodes.size() / 2];
                    middleList.push_back(middle);

                    int left = (allNodes.size() / 2) - 1;
                    int right = (allNodes.size() / 2) + 1;

                    while (left >= 0 || right < allNodes.size())
                    {
                        if (left >= 0)
                        {
                            middleList.push_back(allNodes[left]);
                            --left;
                        }
                        if (right < allNodes.size())
                        {
                            middleList.push_back(allNodes[right]);
                            ++right;
                        }
                    }
                }

                if(!atBegin)
                    index = middleList.size();
            }

            T& operator*() const
            {
                return middleList[index]->data;
            }

            T* operator->() const
            {
                return &(middleList[index]->data);
            }

            MiddleOutOrder& operator++()
            {
                ++index;
                return *this;
            }

            const MiddleOutOrder operator++(int)
            {
                MiddleOutOrder tmp = *this;
                ++(*this);
                return tmp;
            }

            bool operator==(const MiddleOutOrder &order) const
            {
                return index == order.index;
            }

            bool operator!=(const MiddleOutOrder &order) const
            {
                return !(*this == order);
            }
        };

        MiddleOutOrder begin_middle_out_order(){ return MiddleOutOrder(*this, true); }
        MiddleOutOrder end_middle_out_order(){ return MiddleOutOrder(*this, false); }
    };
}
