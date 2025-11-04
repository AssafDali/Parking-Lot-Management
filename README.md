# Parking Lot Management System in C

This project is a comprehensive Parking Lot Management System built entirely in C. It simulates a multi-story parking structure, managed through a text-based menu interface.

The system allows users to initialize a parking lot with a custom number of floors and spots, park new cars, remove cars by their license plate, and check the status of any given floor.

---

### Key Technical Features:

* **Complex Data Structures:** Implements a dynamic array of `Floor` structs. Each floor, in turn, manages its parking spots using a **Linked List** of `Parking` nodes.
* **Dynamic Memory Management:** Demonstrates robust use of `malloc()` and `free()` for all structures (cars, parking spots, floors, and the lot itself), ensuring proper resource allocation and cleanup with no memory leaks.
* **File Persistence (Save/Load):** Users can save the entire state of the parking lot to a `.txt` file and load it back into memory. This is achieved through formatted file I/O (`fopen`, `fprintf`, `fscanf`).
* **Modular Design:** The code is cleanly separated into a `parking_lot.h` header file defining the data structures and function prototypes, and a `Source.c` file containing the full implementation.
* **Input Validation:** Includes detailed validation logic, such as a function (`isValidPlate`) to check for correct Israeli license plate formats (9 and 10-digit formats).

---

### Core Concepts Demonstrated:
* C Programming
* Pointers and Dynamic Memory Allocation
* Linked Lists
* Structs & Dynamic Arrays
* File I/O
* Modular Code (Header/Source separation)
* Error Handling & Input Validation
