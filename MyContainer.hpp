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

        /**
         * This function is responsible for adding data of a specific type to the container
         * @param data - the data that will be added to the container
         */
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

        /**
         * This method is responsible for deleting an object from the container
         * if a few instances of the same data exist, it will remove all instances
         * Throws out_of_range exception if the container is empty or if the element doesn't exist
         * @param data - the data to remove from the container
         */
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

        /**
         * This counts how many elements exist in the container
         * @return the size of the container
         */
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

        /**
         * This function is reponsible for overloading the << operator and decide how to print
         * the container
         * @param os - the entering stream
         * @param container - the container to print
         * @return a stream of the output that will be printed
         */
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
            
            /**
             * This function is responsible for sorting in ascending order the data 
             * in the container when the iterator is created. So later when we iterate
             * the information will be sorted.
             * @param container - the container we iterate through
             * @param atBegin - a boolean value if we return the beginning of the iteration or the end
             * @return an iterator at the begin or end value
             */
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

            /**
             * This operator overloading is responsible for going to the next
             * data in the iteration in prefix call.
             */
            AscendingOrder &operator++()
            {
                ++index;
                return *this;
            }

            /**
             * This operator overloading is responsible for accessing the current
             * data in the iteration.
             */
            T &operator*() const
            {
                return sortedList[index]->data;
            }

            /**
             * This operator overloading is responsible for derefencing the current
             * data in the iteration and returning the value.
             */
            T *operator->() const
            {
                return &(sortedList[index]->data);
            }


            /**
             * This operator overloading is responsible for accessing the current
             * data in the iteration in postfix calls.
             */
            AscendingOrder operator++(int)
            {
                AscendingOrder temp = *this;
                ++(*this);
                return temp;
            }

            /**
             * This operator overloading is responsible for checking if 
             * two iterators are on the same value (by comparing the indexes)
             */
            bool operator==(const AscendingOrder &other) const
            {
                return index == other.index && container == other.container;
            }

            /**
             * This operator overloading is responsible for checking if 
             * two iterators are not on the same value
             */
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
            /**
             * Default (end) constructor.
             */
            explicit DescendingOrder() : container(nullptr), index(0) {}

            /**
             * Builds a descending‐order iterator over the given container.
             * @param container The container whose nodes will be iterated.
             * @param atBegin If true, positions iterator at the first (largest) element.
             * if false, positions it just past the last element.
             */
            DescendingOrder(MyContainer &container, bool atBegin)
                : container(&container), index(0)
            {
                Node *temp = container.head;
                while (temp) {
                    sortedList.push_back(temp);
                    temp = temp->next;
                }
                std::sort(sortedList.begin(), sortedList.end(),
                        [](Node *a, Node *b) { return a->data > b->data; });
                if (!atBegin)
                    index = sortedList.size();
            }

            /**
             * Prefix increment: advance to the next (smaller) element.
             */
            DescendingOrder &operator++()
            {
                ++index;
                return *this;
            }

            /**
             * Dereference to obtain the current element by reference.
             */
            T &operator*() const
            {
                return sortedList[index]->data;
            }

            /**
             * Member access: pointer to the current element’s data.
             */
            T *operator->() const
            {
                return &(sortedList[index]->data);
            }

            /**
             * Postfix increment: advance but return previous iterator state.
             */
            DescendingOrder operator++(int)
            {
                DescendingOrder tmp = *this;
                ++(*this);
                return tmp;
            }

            /**
             * Equality comparison: true if both iterators point at same index/container.
             */
            bool operator==(const DescendingOrder &other) const
            {
                return index == other.index && container == other.container;
            }

            /**
             * Inequality comparison.
             */
            bool operator!=(const DescendingOrder &other) const
            {
                return !(*this == other);
            }
        };

        /// @brief Return iterator to first (largest) element in descending order.
        DescendingOrder begin_descending_order() { return DescendingOrder(*this, true); }
        /// @brief Return iterator just past the last element.
        DescendingOrder end_descending_order()   { return DescendingOrder(*this, false); }


    class SideCrossOrder
        {
        private:
            MyContainer *container;
            std::vector<Node *> crossList;
            std::size_t index;

        public:
            /**
             * Default (end) constructor.
             */
            explicit SideCrossOrder() : container(nullptr), index(0) {}

            /**
             * Builds a “side‐cross” (min, max, next‐min, next‐max, etc...) iterator.
             * @param container The container to iterate.
             * @param atBegin If true, start at the first element in cross‐pattern;
             * if false, position just past the end.
             */
            SideCrossOrder(MyContainer &container, bool atBegin) : container(&container), index(0)
            {
                Node *temp = container.head;
                while (temp) 
                {
                    crossList.push_back(temp);
                    temp = temp->next;
                }
                std::sort(crossList.begin(), crossList.end(),
                        [](Node *a, Node *b) { return a->data < b->data; });

                std::vector<Node *> pattern;
                int left = 0, right = int(crossList.size()) - 1;
                while (left <= right) {
                    pattern.push_back(crossList[left++]);
                    if (left <= right) {
                        pattern.push_back(crossList[right--]);
                    }
                }
                crossList = std::move(pattern);
                if (!atBegin)
                    index = crossList.size();
            }

            /**
             * Prefix increment: advance in the cross pattern.
             */
            SideCrossOrder &operator++()
            {
                ++index;
                return *this;
            }

            /**
             * Dereference to obtain current element.
             */
            T &operator*() const
            {
                return crossList[index]->data;
            }

            /**
             * Member access: pointer to current element’s data.
             */
            T *operator->() const
            {
                return &(crossList[index]->data);
            }

            /**
             * Postfix increment: advance but return previous state.
             */
            SideCrossOrder operator++(int)
            {
                SideCrossOrder tmp = *this;
                ++(*this);
                return tmp;
            }

            /**
             * Equality comparison by index and container.
             */
            bool operator==(const SideCrossOrder &other) const
            {
                return index == other.index && container == other.container;
            }

            /**
             * Inequality comparison.
             */
            bool operator!=(const SideCrossOrder &other) const
            {
                return !(*this == other);
            }
        };

        /// @brief Iterator to the first element in side-cross order.
        SideCrossOrder begin_side_cross_order() { return SideCrossOrder(*this, true); }
        /// @brief Iterator just past the last element.
        SideCrossOrder end_side_cross_order()   { return SideCrossOrder(*this, false); }


    class ReverseOrder
        {
        private:
            std::vector<Node *> reverseList;
            int index;

        public:
            /**
             * Default (end) constructor.
             */
            ReverseOrder() : index(-1) {}

            /**
             * Builds a reverse (tail -> head) iterator.
             * @param container The container whose list to reverse‐iterate.
             * @param atBegin If true, positions at last element; if false, just before first.
             */
            ReverseOrder(const MyContainer &container, bool atBegin)
                : index(-1)
            {
                Node *temp = container.head;
                while (temp) {
                    reverseList.push_back(temp);
                    temp = temp->next;
                }
                index = atBegin ? int(reverseList.size()) - 1 : -1;
            }

            /**
             * Dereference to get current element.
             */
            T &operator*() const
            {
                return reverseList[index]->data;
            }

            /**
             * Member access for current element.
             */
            T *operator->() const
            {
                return &reverseList[index]->data;
            }

            /**
             * Prefix increment (moves “forward” by stepping backwards in the list).
             */
            ReverseOrder &operator++()
            {
                --index;
                return *this;
            }

            /**
             * Postfix increment.
             */
            ReverseOrder operator++(int)
            {
                ReverseOrder tmp = *this;
                --index;
                return tmp;
            }

            /**
             * True if both iterators point to same position.
             */
            bool operator==(const ReverseOrder &o) const
            {
                return index == o.index;
            }

            /**
             * True if positions differ.
             */
            bool operator!=(const ReverseOrder &o) const
            {
                return !(*this == o);
            }
        };

        /// @brief Reverse‐order begin iterator.
        ReverseOrder begin_reverse_order() { return ReverseOrder(*this, true); }
        /// @brief Reverse‐order end iterator.
        ReverseOrder end_reverse_order()   { return ReverseOrder(*this, false); }



    class Order {
        private:
            Node* current;  // Pointer to the current node in the list

        public:
            /**
             * Construct an iterator pointing to the given node.
             * @param node The node at which to start iteration (nullptr for end).
             */
            explicit Order(Node* node = nullptr)
                : current(node)
            {
            }

            /**
             * Dereference operator: access the value at the current iterator position.
             * @return Reference to the element stored in the current node.
             */
            T& operator*() const
            {
                return current->data;
            }

            /**
             * Member‐access operator: access members of the current element.
             * @return Pointer to the element stored in the current node.
             */
            T* operator->() const
            {
                return &(current->data);
            }

            /**
             * Pre‐increment operator: advance the iterator to the next node.
             * @return Reference to this iterator after increment.
             */
            Order& operator++()
            {
                current = current->next;
                return *this;
            }

            /**
             * Post‐increment operator: advance the iterator, returning its previous state.
             * @return Iterator pointing to the element before increment.
             */
            const Order operator++(int)
            {
                Order tmp = *this;
                ++(*this);
                return tmp;
            }

            /**
             * Equality comparison: check if two iterators point to the same node.
             * @param other  The iterator to compare against.
             * @return True if both iterators refer to the same position.
             */
            bool operator==(const Order& other) const
            {
                return current == other.current;
            }

            /**
             * Inequality comparison: check if two iterators point to different nodes.
             * @param other  The iterator to compare against.
             * @return True if the iterators refer to different positions.
             */
            bool operator!=(const Order& other) const
            {
                return !(*this == other);
            }
        };

        /**
         * Get an iterator to the first element in insertion order.
         * @return Order iterator at the beginning.
         */
        Order begin_order() { return Order(head); }

        /**
         * Get an iterator one past the last element in insertion order.
         * @return Order iterator at the end (nullptr).
         */
        Order end_order()   { return Order(nullptr); }

    /**
     * Iterator that starts from the middle of the container and alternates outward.
     */
    class MiddleOutOrder {
        private:
            std::vector<Node*> middleList;  /// Nodes arranged in middle-out sequence
            std::size_t index;              /// Current position in the sequence

        public:
            /**
             * Default constructor: creates an invalid iterator.
             */
            explicit MiddleOutOrder()
                : index(0)
            {
            }

            /**
             * Construct a middle-out iterator over the given container.
             * @param container The container whose nodes to traverse.
             * @param atBegin If true, iterator starts at the middle; if false, at end.
             */
            MiddleOutOrder(const MyContainer& container, bool atBegin) : index(0)
            {
                // Collect all nodes in insertion order
                Node* temp = container.head;
                std::vector<Node*> allNodes;
                while (temp) {
                    allNodes.push_back(temp);
                    temp = temp->next;
                }

                // Build the middle-out sequence
                if (!allNodes.empty()) {
                    std::size_t mid = allNodes.size() / 2;
                    middleList.push_back(allNodes[mid]);

                    int left  = static_cast<int>(mid) - 1;
                    int right = static_cast<int>(mid) + 1;
                    while (left >= 0 || right < static_cast<int>(allNodes.size())) {
                        if (left >= 0) {
                            middleList.push_back(allNodes[left--]);
                        }
                        if (right < static_cast<int>(allNodes.size())) {
                            middleList.push_back(allNodes[right++]);
                        }
                    }
                }

                // If end iterator requested, position past last element
                if (!atBegin) {
                    index = middleList.size();
                }
            }

            /**
             * Pre‐increment operator: move to the next position in middle-out order.
             * @return Reference to this iterator after increment.
             */
            MiddleOutOrder& operator++()
            {
                ++index;
                return *this;
            }

            /**
             * Dereference operator: access the value at the current middle-out position.
             * @return Reference to the element at the iterator’s position.
             */
            T& operator*() const
            {
                return middleList[index]->data;
            }

            /**
             * Member‐access operator: access members of the current element.
             * @return Pointer to the element at the iterator’s position.
             */
            T* operator->() const
            {
                return &(middleList[index]->data);
            }

            /**
             * Post‐increment operator: advance the iterator, returning its previous state.
             * @return Iterator pointing to the element before increment.
             */
            const MiddleOutOrder operator++(int)
            {
                MiddleOutOrder tmp = *this;
                ++(*this);
                return tmp;
            }

            /**
             * Equality comparison: check if two middle-out iterators are at the same position.
             * @param other  The iterator to compare against.
             * @return True if both are at the same index in the same sequence.
             */
            bool operator==(const MiddleOutOrder& other) const
            {
                return index == other.index;
            }

            /**
             * Inequality comparison: check if two iterators are at different positions.
             * @param other  The iterator to compare against.
             * @return True if they are not equal.
             */
            bool operator!=(const MiddleOutOrder& other) const
            {
                return !(*this == other);
            }
        };

        /**
         * Get a middle-out iterator starting at the middle element.
         * @return MiddleOutOrder at begin.
         */
        MiddleOutOrder begin_middle_out_order() { return MiddleOutOrder(*this, true); }

        /**
         * Get a middle-out iterator positioned past the last element.
         * @return MiddleOutOrder at end.
         */
        MiddleOutOrder end_middle_out_order()   { return MiddleOutOrder(*this, false); }
    };
}
