#include <iostream>
#include "MyContainer.hpp"

using namespace logic;

int main()
{
    // ----------------------------
    // Integer MyContainer part
    // ----------------------------
    MyContainer<int> intContainer;
    intContainer.add(7);
    intContainer.add(15);
    intContainer.add(6);
    intContainer.add(1);
    intContainer.add(2);

    std::cout << "---- int container ----\n";
    std::cout << "Size of container: " << intContainer.size() << "\n";

    std::cout << "Ascending order: ";
    for (auto it = intContainer.begin_ascending_order(); it != intContainer.end_ascending_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: 1 2 6 7 15
    }
    std::cout << "\n";

    std::cout << "Descending order: ";
    for (auto it = intContainer.begin_descending_order(); it != intContainer.end_descending_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: 15 7 6 2 1
    }
    std::cout << "\n";

    std::cout << "Side-cross order: ";
    for (auto it = intContainer.begin_side_cross_order(); it != intContainer.end_side_cross_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: 1 15 2 7 6
    }
    std::cout << "\n";

    std::cout << "Reverse order: ";
    for (auto it = intContainer.begin_reverse_order(); it != intContainer.end_reverse_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: 2 1 6 15 7
    }
    std::cout << "\n";

    std::cout << "Insertion order: ";
    for (auto it = intContainer.begin_order(); it != intContainer.end_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: 7 15 6 1 2
    }
    std::cout << "\n";

    std::cout << "Middle-out order: ";
    for (auto it = intContainer.begin_middle_out_order(); it != intContainer.end_middle_out_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: 6 15 1 7 2
    }
    std::cout << "\n\n";

    // -------------------------------
    // Double MyContainer part
    // -------------------------------
    MyContainer<double> dblContainer;
    dblContainer.add(3.14);
    dblContainer.add(1.41);
    dblContainer.add(-2.5);
    dblContainer.add(0.0);
    dblContainer.add(2.718);

    std::cout << "---- double container ----\n";
    std::cout << "Size of container: " << dblContainer.size() << "\n";

    std::cout << "Ascending order: ";
    for (auto it = dblContainer.begin_ascending_order(); it != dblContainer.end_ascending_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: -2.5 0.0 1.41 2.718 3.14
    }
    std::cout << "\n";

    std::cout << "Descending order: ";
    for (auto it = dblContainer.begin_descending_order(); it != dblContainer.end_descending_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: 3.14 2.718 1.41 0.0 -2.5
    }
    std::cout << "\n";

    std::cout << "Side-cross order: ";
    for (auto it = dblContainer.begin_side_cross_order(); it != dblContainer.end_side_cross_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: -2.5 3.14 0.0 2.718 1.41
    }
    std::cout << "\n";

    std::cout << "Reverse order: ";
    for (auto it = dblContainer.begin_reverse_order(); it != dblContainer.end_reverse_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: 2.718 0.0 -2.5 1.41 3.14
    }
    std::cout << "\n";

    std::cout << "Insertion order: ";
    for (auto it = dblContainer.begin_order(); it != dblContainer.end_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: 3.14 1.41 -2.5 0.0 2.718
    }
    std::cout << "\n";

    std::cout << "Middle-out order: ";
    for (auto it = dblContainer.begin_middle_out_order(); it != dblContainer.end_middle_out_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: -2.5 1.41 0.0 3.14 2.718
    }
    std::cout << "\n\n";

    // -----------------------------
    // Char MyContainer part
    // -----------------------------
    MyContainer<char> charContainer;
    charContainer.add('x');
    charContainer.add('m');
    charContainer.add('a');
    charContainer.add('z');
    charContainer.add('b');

    std::cout << "---- char container ----\n";
    std::cout << "Size of container: " << charContainer.size() << "\n";

    std::cout << "Ascending order: ";
    for (auto it = charContainer.begin_ascending_order(); it != charContainer.end_ascending_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: a b m x z
    }
    std::cout << "\n";

    std::cout << "Descending order: ";
    for (auto it = charContainer.begin_descending_order(); it != charContainer.end_descending_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: z x m b a
    }
    std::cout << "\n";

    std::cout << "Side-cross order: ";
    for (auto it = charContainer.begin_side_cross_order(); it != charContainer.end_side_cross_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: a z b x m
    }
    std::cout << "\n";

    std::cout << "Reverse order: ";
    for (auto it = charContainer.begin_reverse_order(); it != charContainer.end_reverse_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: b a z m x
    }
    std::cout << "\n";

    std::cout << "Insertion order: ";
    for (auto it = charContainer.begin_order(); it != charContainer.end_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: x m a z b
    }
    std::cout << "\n";

    std::cout << "Middle-out order: ";
    for (auto it = charContainer.begin_middle_out_order(); it != charContainer.end_middle_out_order(); ++it)
    {
        std::cout << *it << ' '; // Expected: a m b x z
    }
    std::cout << "\n";

    return 0;
}
