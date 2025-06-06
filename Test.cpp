#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "MyContainer.hpp"
#include <sstream>

using namespace logic;

// Helper to convert container contents (via operator<<) into a string
template<typename T>
std::string container_to_string(const MyContainer<T> &c)
{
    std::ostringstream oss;
    oss << c;
    return oss.str();
}

TEST_SUITE("MyContainer basic operations")
{
    TEST_CASE("size() on empty container")
    {
        MyContainer<int> container;
        CHECK(container.size() == 0u);
        // operator<< of an empty container should produce an empty stream
        CHECK(container_to_string(container) == "");
    }

    TEST_CASE("add() increases size and operator<<")
    {
        MyContainer<int> container;
        container.add(5);
        CHECK(container.size() == 1u);
        // Unlike a typical container, operator<< in this implementation writes "[5, ]"
        CHECK(container_to_string(container) == "[5, ]");

        container.add(10);
        CHECK(container.size() == 2u);
        // After adding 10, it should print both elements in insertion order
        CHECK(container_to_string(container) == "[5, 10, ]");
    }

    TEST_CASE("remove() from single-element or multi-element container")
    {
        MyContainer<int> container;

        // Removing from empty should throw
        CHECK_THROWS_AS(container.remove(42), std::out_of_range);

        // Add a few elements, including duplicates
        container.add(1);
        container.add(2);
        container.add(3);
        container.add(2);
        CHECK(container.size() == 4u);

        // Remove all occurrences of '2'
        container.remove(2);
        // Now only 1 and 3 should remain, in original order
        CHECK(container.size() == 2u);
        CHECK(container_to_string(container) == "[1, 3, ]");

        // Removing a value not in the container should throw
        CHECK_THROWS_AS(container.remove(100), std::out_of_range);

        // Remove 1, then 3
        container.remove(1);
        CHECK(container.size() == 1u);
        CHECK(container_to_string(container) == "[3, ]");
        container.remove(3);
        CHECK(container.size() == 0u);
        CHECK(container_to_string(container) == "");
    }
}

TEST_SUITE("MyContainer iteration orders")
{
    //
    // Prepare a container with known elements: [3, 1, 4, 2]
    //
    struct Fixture
    {
        MyContainer<int> container;

        Fixture()
        {
            container.add(3);
            container.add(1);
            container.add(4);
            container.add(2);
            // In-memory sequence: 3 → 1 → 4 → 2
        }
    };

    TEST_CASE_FIXTURE(Fixture, "natural (insertion) order")
    {
        // begin_order() should iterate exactly in insertion order: 3, 1, 4, 2
        std::vector<int> result;
        for (auto it = container.begin_order(); it != container.end_order(); ++it)
        {
            result.push_back(*it);
        }
        CHECK(result == std::vector<int>{3, 1, 4, 2});
    }

    TEST_CASE_FIXTURE(Fixture, "ascending order")
    {
        // ascend: 1, 2, 3, 4
        std::vector<int> result;
        for (auto it = container.begin_ascending_order(); it != container.end_ascending_order(); ++it)
        {
            result.push_back(*it);
        }
        CHECK(result == std::vector<int>{1, 2, 3, 4});
    }

    TEST_CASE_FIXTURE(Fixture, "descending order")
    {
        // descend: 4, 3, 2, 1
        std::vector<int> result;
        for (auto it = container.begin_descending_order(); it != container.end_descending_order(); ++it)
        {
            result.push_back(*it);
        }
        CHECK(result == std::vector<int>{4, 3, 2, 1});
    }

    TEST_CASE_FIXTURE(Fixture, "side-cross order")
    {
        // side-cross: pick smallest, then largest, then next-smallest, then next-largest, ...
        // Sorted ascending: [1,2,3,4]
        // side-cross: 1 (left=0), 4 (right=3), 2 (left=1), 3 (right=2)
        std::vector<int> result;
        for (auto it = container.begin_side_cross_order(); it != container.end_side_cross_order(); ++it)
        {
            result.push_back(*it);
        }
        CHECK(result == std::vector<int>{1, 4, 2, 3});
    }

    TEST_CASE_FIXTURE(Fixture, "reverse order")
    {
        // reverse the insertion: insertion was [3,1,4,2], so reverse is [2,4,1,3]
        std::vector<int> result;
        // Note: ReverseOrder has no external "begin/end" members, but its constructor is
        //     ReverseOrder(const MyContainer container, bool atBegin).
        // So we must call it as: MyContainer<int> temp = container;  // copy by value
        // Then:
        for (auto it = container.begin_reverse_order();
             it != container.end_reverse_order(); ++it)
        {
            result.push_back(*it);
        }
        CHECK(result == std::vector<int>{2, 4, 1, 3});
    }

    TEST_CASE_FIXTURE(Fixture, "middle-out order")
    {
        // middle-out: find middle in sorted nodes-by-insertion: [3,1,4,2]
        //   But implementation first collects in insertion: [3,1,4,2], then
        //   middle index = size/2 = 4/2 = 2 → element at index 2 is 4
        //   Then alternating left, right from that index:
        //     order: 4, then element at index 1 (1), then index 3 (2), then index 0 (3)
        std::vector<int> result;
        for (auto it = container.begin_middle_out_order(); it != container.end_middle_out_order(); ++it)
        {
            result.push_back(*it);
        }
        CHECK(result == std::vector<int>{4, 1, 2, 3});
    }

    TEST_CASE_FIXTURE(Fixture, "mixed-type check: adding strings")
    {
        MyContainer<std::string> sc;
        sc.add("delta");
        sc.add("alpha");
        sc.add("charlie");
        sc.add("bravo");
        // Natural order: ["delta", "alpha", "charlie", "bravo"]
        std::vector<std::string> nat;
        for (auto it = sc.begin_order(); it != sc.end_order(); ++it)
        {
            nat.push_back(*it);
        }
        CHECK(nat == std::vector<std::string>{"delta", "alpha", "charlie", "bravo"});

        // Ascending lex order: ["alpha","bravo","charlie","delta"]
        std::vector<std::string> asc;
        for (auto it = sc.begin_ascending_order(); it != sc.end_ascending_order(); ++it)
        {
            asc.push_back(*it);
        }
        CHECK(asc == std::vector<std::string>{"alpha", "bravo", "charlie", "delta"});
    }
}

TEST_SUITE("operator chaining and incremental operators")
{
    TEST_CASE("post-increment and pre-increment consistency")
    {
        MyContainer<int> container;
        container.add(10);
        container.add(20);
        container.add(30);

        // AscendingOrder: sorted elements [10,20,30]
        auto itA = container.begin_ascending_order();
        CHECK(*itA == 10);
        auto itA_post = itA++;
        CHECK(*itA_post == 10);
        CHECK(*itA == 20);
        ++itA;
        CHECK(*itA == 30);

        // DescendingOrder: sorted desc [30,20,10]
        auto itD = container.begin_descending_order();
        CHECK(*itD == 30);
        auto itD_post = itD++;
        CHECK(*itD_post == 30);
        CHECK(*itD == 20);
        ++itD;
        CHECK(*itD == 10);
    }
}


TEST_SUITE("MyContainer with double type")
{
    TEST_CASE("add/remove and to-string for double")
    {
        MyContainer<double> doubleContainer;
        CHECK(doubleContainer.size() == 0u);
        CHECK(container_to_string(doubleContainer) == "");

        doubleContainer.add(2.5);
        doubleContainer.add(3.14);
        doubleContainer.add(-1.0);
        CHECK(doubleContainer.size() == 3u);

        // Depending on floating-point formatting, operator<< may produce something like "[2.5, 3.14, -1, ]"
        std::string s = container_to_string(doubleContainer);
        CHECK(s.find("2.5") != std::string::npos);
        CHECK(s.find("3.14") != std::string::npos);
        CHECK(s.find("-1") != std::string::npos);

        // Remove a middle value
        doubleContainer.remove(3.14);
        CHECK(doubleContainer.size() == 2u);
        std::string s2 = container_to_string(doubleContainer);
        CHECK(s2.find("3.14") == std::string::npos);

        // Removing a non-existent double should throw
        CHECK_THROWS_AS(doubleContainer.remove(100.0), std::out_of_range);

        // Remove remaining
        doubleContainer.remove(2.5);
        doubleContainer.remove(-1.0);
        CHECK(doubleContainer.size() == 0u);
        CHECK(container_to_string(doubleContainer) == "");
    }

    struct DoubleFixture
    {
        MyContainer<double> doubleContainer;

        DoubleFixture()
        {
            doubleContainer.add(1.1);
            doubleContainer.add(2.2);
            doubleContainer.add(0.0);
            doubleContainer.add(-5.5);
        }
    };

    TEST_CASE_FIXTURE(DoubleFixture, "ascending order for double")
    {
        std::vector<double> result;
        for (auto it = doubleContainer.begin_ascending_order(); it != doubleContainer.end_ascending_order(); ++it)
        {
            result.push_back(*it);
        }
        CHECK(result == std::vector<double>{-5.5, 0.0, 1.1, 2.2});
    }

    TEST_CASE_FIXTURE(DoubleFixture, "descending order for double")
    {
        std::vector<double> result;
        for (auto it = doubleContainer.begin_descending_order(); it != doubleContainer.end_descending_order(); ++it)
        {
            result.push_back(*it);
        }
        CHECK(result == std::vector<double>{2.2, 1.1, 0.0, -5.5});
    }

    TEST_CASE_FIXTURE(DoubleFixture, "side-cross order for double")
    {
        // Sorted ascending: [-5.5, 0.0, 1.1, 2.2]
        // Side-cross: -5.5 (left), 2.2 (right), 0.0 (left+1), 1.1 (right-1)
        std::vector<double> result;
        for (auto it = doubleContainer.begin_side_cross_order(); it != doubleContainer.end_side_cross_order(); ++it)
        {
            result.push_back(*it);
        }
        CHECK(result == std::vector<double>{-5.5, 2.2, 0.0, 1.1});
    }

    TEST_CASE_FIXTURE(DoubleFixture, "natural order for double")
    {
        std::vector<double> result;
        for (auto it = doubleContainer.begin_order(); it != doubleContainer.end_order(); ++it)
        {
            result.push_back(*it);
        }
        CHECK(result == std::vector<double>{1.1, 2.2, 0.0, -5.5});
    }

    TEST_CASE_FIXTURE(DoubleFixture, "middle-out order for double")
    {
        // Insertion sequence: [1.1, 2.2, 0.0, -5.5]
        // Middle index = 4/2 = 2 → element 0.0
        // Order: [0.0, 2.2, -5.5, 1.1]
        std::vector<double> result;
        for (auto it = doubleContainer.begin_middle_out_order(); it != doubleContainer.end_middle_out_order(); ++it)
        {
            result.push_back(*it);
        }
        CHECK(result == std::vector<double>{0.0, 2.2, -5.5, 1.1});
    }
}

TEST_SUITE("MyContainer with char type")
{
    TEST_CASE("add/remove and to-string for char")
    {
        MyContainer<char> charContainer;
        CHECK(charContainer.size() == 0u);
        CHECK(container_to_string(charContainer) == "");

        charContainer.add('a');
        charContainer.add('Z');
        charContainer.add('m');
        CHECK(charContainer.size() == 3u);

        // operator<< might produce "[a, Z, m, ]"
        std::string s = container_to_string(charContainer);
        CHECK(s.find("a") != std::string::npos);
        CHECK(s.find("Z") != std::string::npos);
        CHECK(s.find("m") != std::string::npos);

        // Remove 'Z'
        charContainer.remove('Z');
        CHECK(charContainer.size() == 2u);
        std::string s2 = container_to_string(charContainer);
        CHECK(s2.find("Z") == std::string::npos);

        // Removing a non-existent char should throw
        CHECK_THROWS_AS(charContainer.remove('x'), std::out_of_range);

        // Remove remaining
        charContainer.remove('a');
        charContainer.remove('m');
        CHECK(charContainer.size() == 0u);
        CHECK(container_to_string(charContainer) == "");
    }

    struct CharFixture
    {
        MyContainer<char> charContainer;

        CharFixture()
        {
            charContainer.add('d');
            charContainer.add('a');
            charContainer.add('c');
            charContainer.add('b');
        }
    };

    TEST_CASE_FIXTURE(CharFixture, "ascending order for char")
    {
        std::vector<char> result;
        for (auto it = charContainer.begin_ascending_order(); it != charContainer.end_ascending_order(); ++it)
        {
            result.push_back(*it);
        }
        CHECK(result == std::vector<char>{'a', 'b', 'c', 'd'});
    }

    TEST_CASE_FIXTURE(CharFixture, "descending order for char")
    {
        std::vector<char> result;
        for (auto it = charContainer.begin_descending_order(); it != charContainer.end_descending_order(); ++it)
        {
            result.push_back(*it);
        }
        CHECK(result == std::vector<char>{'d', 'c', 'b', 'a'});
    }

    TEST_CASE_FIXTURE(CharFixture, "side-cross order for char")
    {
        // Sorted ascending: ['a','b','container','d']
        // Side-cross: 'a', 'd', 'b', 'container'
        std::vector<char> result;
        for (auto it = charContainer.begin_side_cross_order(); it != charContainer.end_side_cross_order(); ++it)
        {
            result.push_back(*it);
        }
        CHECK(result == std::vector<char>{'a', 'd', 'b', 'c'});
    }

    TEST_CASE_FIXTURE(CharFixture, "natural order for char")
    {
        std::vector<char> result;
        for (auto it = charContainer.begin_order(); it != charContainer.end_order(); ++it)
        {
            result.push_back(*it);
        }
        CHECK(result == std::vector<char>{'d', 'a', 'c', 'b'});
    }

    TEST_CASE_FIXTURE(CharFixture, "middle-out order for char")
    {
        // Insertion sequence: ['d','a','container','b']
        // Middle index = 4/2 = 2 → 'container'
        // Order: ['container', 'a', 'b', 'd']
        std::vector<char> result;
        for (auto it = charContainer.begin_middle_out_order(); it != charContainer.end_middle_out_order(); ++it)
        {
            result.push_back(*it);
        }
        CHECK(result == std::vector<char>{'c', 'a', 'b', 'd'});
    }
}
