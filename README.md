# MyContainer Project

This repository provides a templated container class (`MyContainer<T>`) supporting multiple iteration orders (insertion, ascending, descending, side-cross, reverse, and middle-out). It includes:

* `main.cpp`: example usage of `MyContainer` with `int`, `double`, and `char`.
* `MyContainer.hpp`: header defining the container and its iterators.
* `Test.cpp`: `doctest`‐based test suite covering all iterator types and basic operations.
* `Makefile`: targets for building, testing, running under Valgrind, and cleaning.

---

## Prerequisites

* A C++17-capable compiler (`g++`, `clang++`, etc.).
* [Valgrind](https://valgrind.org/) (optional, for the `valgrind` target).

---

## File Structure

```
├── Makefile
├── MyContainer.hpp
├── main.cpp
├── Test.cpp
└── README.md
```

* **MyContainer.hpp**
  Templated container class `MyContainer<T>` and all iterator definitions (`InsertionOrder`, `AscendingOrder`, `DescendingOrder`, `SideCrossOrder`, `ReverseOrder`, `MiddleOutOrder`).

* **main.cpp**
  Demonstrates inserting elements of type `int`, `double`, and `char` into `MyContainer<T>`, then iterating in all six orders, printing results to stdout.

* **Test.cpp**
  A comprehensive `doctest` suite verifying:

  * `addElement()`, `removeElement()`, `size()`, and `operator<<`
  * All six iteration orders for `int`, `double`, and `char` specializations.
  * Correct behavior for boundary cases (empty container, duplicate removals, etc.).

* **Makefile**

  * `make main` → build the example `main` executable
  * `make test` → compile and link `Test.cpp` into `./test`
  * `make valgrind` → run `./test` under Valgrind (`--leak-check=full`)
  * `make clean` → remove generated binaries (`main`, `test`) and object files

---

## Building and Running

1. **Clone or download** this directory so that you have:

   ```
   MyContainer.hpp
   main.cpp
   test_myContainer.cpp
   Makefile
   doctest.h    (or ensure your compiler can find it)
   ```

2. **Build everything** (both `main` and `test`):

   ```bash
   make all
   ```

   * This invokes the `main` and `test` targets.

3. **Build only the example**:

   ```bash
   make main
   ```

   * Produces the binary `./main`.

4. **Build only the tests**:

   ```bash
   make test
   ```

   * Produces the test binary `./test`.

---

## Memory Checking (Valgrind)

If Valgrind is installed, you can run:

```bash
make valgrind
```

This will:

1. Build the test binary (`make test`).
2. Launch `valgrind --leak-check=full ./test`.

You should see zero memory leaks and no invalid reads/writes.

---

## Cleaning Up

To remove all compiled binaries and object files:

```bash
make clean
```

This deletes:

* `main`
* `test`
* Any temporary `*.o` files
