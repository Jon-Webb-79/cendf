*****************************
Cross Section Data Structure
*****************************

.. module:: xsec
    :synopsis: Energy-dependent cross-section data management

Overview
========
The ``xsec_t`` data structure provides an efficient way to store and interpolate 
energy-dependent cross-section data. It was developed to handle large datasets of nuclear 
cross-sections where data points consist of energy-cross-section pairs, and interpolation 
between these points is frequently needed.

The structure uses parallel arrays to store cross-section and energy values, with dynamic 
memory allocation to efficiently handle datasets of varying sizes. It implements a hybrid 
growth strategy to balance memory usage and performance.

While this data structure can be used by developers for other uses, it was developed 
specifically to support the CENDF code, and only the necessary functionality for 
this purpose was developed within this code base.  The functions described in this 
section can be accessed from the ``dstructures.h`` header file.

Features
--------
- Dynamic memory allocation with hybrid growth strategy
- Thread-safe data access through const-correctness
- Linear interpolation for cross-section values at arbitrary energies
- Automatic memory management support through GCC/Clang attributes
- Comprehensive error handling using errno

Data Structures
===============

.. c:type:: xsec_t

    An opaque structure that manages the storage and access of cross-section data.
    This struct is encapsulated within the ``dstructures.c`` file and are directly
    accesible to a developer.

    .. code-block:: c

        typedef struct {
            float xs;      /* Cross-section value */
            float energy;  /* Corresponding energy value */
            size_t alloc;  /* allocated size of the xs and energy arrays */ 
            size_t len;    /* populated size o the xs and energy arrays */
        } xsecData;

.. c:type:: xsecData

    A structure representing a single cross-section/energy pair.

    .. code-block:: c

        typedef struct {
            float xs;      /* Cross-section value */
            float energy;  /* Corresponding energy value */
        } xsecData;

API Reference
=============

Initialization and Cleanup
--------------------------
The functions described in this section are used to initialize dynamically 
allocated ``xsec_t`` data types and to free them from memory.  The ``xsec_t``
struct is defined in the ``dstructures.c`` file, and therefore can not be 
directly accessed by the developer.  This struct can only be dynamically 
created with the ``init_xsec()`` function, and therefore must also be 
freed from memory after use.

.. c:function:: xsec_t* init_xsec(size_t buffer_length)

    Initializes a new cross-section data structure.  If the code is compiled 
    with gcc or clang, the developer should consider using the :ref:`XSEC_GBC <xsec-gbc-macro>`
    macro with this function to automate garbage collection once the 
    ``xsec_t`` data type goes out of scope.

    :param buffer_length: Initial capacity for the arrays
    :return: Pointer to initialized structure, or NULL on failure
    :errno: ``ENOMEM`` on memory allocation failure

Code Examples

.. code-block:: c

    #include "dstructures.h"

    int main() {
        // Initialize xsec_t variable
        xsec_t* xsec = init_xsec(1);
        // Free xsec_t variable when it is no longer needed 
        free_data(xsec);
        
        /* If the code is compiled with gcc or clang, use the XSEC_GBC macro 
         * to automate garbage collection (free_data()) is no longer required 
         */
        xsec_t* new_xsec XSEC_GBC = init_xsec(1);
        // free_data() is no longer required
    }

.. _xsec-free-func:

.. c:function:: void free_xsec(xsec_t* cross_section)

    Deallocates all memory associated with the structure. This function 
    can be used directly to free xsec_t data from memory, or the developer 
    can also use the :ref:`free_data <free-data-macro>` macro as a generic macro that can be used 
    to free memory for any data type in this code base.

    :param cross_section: Pointer to structure to be freed
    :errno: ``EINVAL`` if NULL pointer passed

Code Examples

.. code-block:: c

    #include "dstructures.h"

    int main() {
        // Initialize xsec_t variable
        xsec_t* xsec = init_xsec(1);
        // Free xsec_t variable when it is no longer needed 
        free_xsec(xsec);
        // Can also use free_data(xsec) to free memory
    }

Data Management
---------------
The functions referenced in this section can be used to add or remove
data from the ``xsec_t`` data structure.

.. c:function:: bool push_xsec(xsec_t* cross_section, float xsec, float energy)

    Appends a new cross-section/energy pair to the structure.

    :param cross_section: Target structure
    :param xsec: Cross-section value to add
    :param energy: Corresponding energy value
    :return: ``true`` on success, ``false`` on failure
    :errno: 
        - ``EINVAL`` if invalid structure pointer
        - ``ENOMEM`` if memory reallocation failed

.. _xsec-get:

Code Examples

.. code-block:: c

    #include "dstructures.h"
    #include <stdlib.h> // For EXIT_SUCCESS and EXIT_FAILURE
    #include <stdio.h>  // For printf

    int main() {
        // Initialize xsec_t variable
        xsec_t* xsec XSEC_GBC = init_xsec(5);
        float temp[5] = {1.f, 2.f, 3.f, 4.f, 5.f};
        float temp_e[5] = {10.f, 20.f, 30.f, 40.f, 50.f};
        for (size_t i = 0; i < 5; i++) {
            if (!push_xsec(xsec, temp[i], temp_e[i])) {
                EXIT_FAILURE;
            }
        }
        print("Cross Section Array\n");
        printf("[ ");
        for (size_t i = 0; i < 4; i++) {
            printf(%f, ", get_xsec(xsec, i));
        }
        printf("%f ]\\n", get_xsec(xsec, size(xsec));

        print("Energy Array\n");
        printf("[ ");
        for (size_t i = 0; i < 4; i++) {
            printf(%f, ", get_xsec_energy(xsec, i));
        }
        printf("%f ]\n", get_xec_energy(xsec, size(xsec)));
    }

.. code-block:: bash

    Cross Section Array 
    [ 1.0000, 2.0000, 3.0000, 4.0000, 5.0000 ]

    Energy Array
    [10.000, 20.0000, 30.0000, 40.0000, 50.0000]

Data Access
-----------
The following functions demonstrate how to access data from within an ``xsec_t``
data structure.

.. c:function:: const float get_xsec(const xsec_t* cross_section, size_t index)

    Retrieves a cross-section value by index.  See :ref:`push_xsec() <xsec-get>`
    for a code example.

    :param cross_section: Source structure
    :param index: Array index
    :return: Cross-section value, or -1.0f on failure
    :errno: ``EINVAL`` if invalid structure pointer or index out of bounds

.. c:function:: const float get_xsec_energy(const xsec_t* cross_section, size_t index)

    Retrieves an energy value by index. See :ref:`push_xsec() <xsec-get>` for 
    a code example.

    :param cross_section: Source structure
    :param index: Array index
    :return: Energy value, or -1.0f on failure
    :errno: ``EINVAL`` if invalid structure pointer or index out of bounds

.. c:function:: const xsecData get_xsec_data(const xsec_t* cross_section, size_t index)

    Retrieves a cross-section/energy pair by index.

    :param cross_section: Source structure
    :param index: Array index
    :return: xsecData structure (with -1.0f values on failure). Statically
             allocated data type
    :errno: ``EINVAL`` if invalid structure pointer or index out of bounds

Code Examples

.. code-block:: c

    #include "dstructures.h"
    #include <stdlib.h> // For EXIT_SUCCESS and EXIT_FAILURE
    #include <stdio.h>  // For printf

    int main() {
        // Initialize xsec_t variable
        xsec_t* xsec XSEC_GBC = init_xsec(5);
        float temp[5] = {1.f, 2.f, 3.f, 4.f, 5.f};
        float temp_e[5] = {10.f, 20.f, 30.f, 40.f, 50.f};
        for (size_t i = 0; i < 5; i++) {
            if (!push_xsec(xsec, temp[i], temp_e[i])) {
                EXIT_FAILURE;
            }
        }
        print("Energy      Cross Section\n\n");
        for (size_t i = 0; i < 5; i++) {
            xsecData dat = get_xsec_data(xsec, i);
            printf(%f    %f", dat.energy, dat.xs);
        }
    }

.. code-block:: bash

    Energy    Cross Section

    10.0000    1.0000
    20.0000    2.0000 
    30.0000    3.0000 
    40.0000    4.0000 
    50.0000    5.0000

Interpolation
-------------

.. c:function:: const float interp_xsec(const xsec_t* cross_section, float energy)

    Interpolates cross-section value for a given energy.  Uses a binary search algorithm
    to reduce the look up time complexity.

    :param cross_section: Source structure
    :param energy: Energy value for interpolation
    :return: Interpolated cross-section value, or -1.0f on failure
    :errno: 
        - ``EINVAL`` if invalid structure pointer or empty structure
        - ``ERANGE`` if energy value out of bounds

.. note:: This algorithm assumes that the energy array is organized from the smallest numerical value to the largest.  If this is not the case, the behavior will be undefined.

Example usage and output:

.. code-block:: c

    #include <stdio.h>
    #include "dstructures.h"

    int main() {
        // Initialize cross section data structure
        XSEC_GBC xsec_t* xs = init_xsec(4);
            
        // Add some example cross-section points
        push_xsec(xs, 100.0, 0.1);  // 100 barns at 0.1 MeV
        push_xsec(xs, 50.0, 1.0);   // 50 barns at 1.0 MeV
        push_xsec(xs, 10.0, 10.0);  // 10 barns at 10.0 MeV
        push_xsec(xs, 1.0, 100.0);  // 1 barn at 100.0 MeV

        // Interpolate at various energies
        float test_energies[] = {0.5, 5.0, 50.0};
            
        for (int i = 0; i < 3; i++) {
            float result = interp_xsec(xs, test_energies[i]);
            printf("Cross section at %f MeV: %f barns\n", 
                    test_energies[i], result);
        }
            
        return 0;
    }

Expected output:

    .. code-block:: text

        Cross section at 0.500000 MeV: 75.000000 barns
        Cross section at 5.000000 MeV: 30.000000 barns
        Cross section at 50.000000 MeV: 5.500000 barns

    The example demonstrates linear interpolation between the provided data points:
    
    - At 0.5 MeV (between 0.1 and 1.0 MeV): interpolates to 75 barns
    - At 5.0 MeV (between 1.0 and 10.0 MeV): interpolates to 30 barns
    - At 50.0 MeV (between 10.0 and 100.0 MeV): interpolates to 5.5 barns


Utility Functions
-----------------
The following functions can be used to access data within the ``xsec_t`` data 
structure.

.. _xsec-size-func:

.. c:function:: size_t xsec_size(const xsec_t* cross_section)

    Returns the number of elements in the structure.  The generic :ref:`size <size-macro>`
    macro can also be used in place of this function.

    :param cross_section: Target structure
    :return: Number of elements, or 0 on failure
    :errno: ``EINVAL`` if invalid structure pointer

Example Code 

.. code-block:: c

    #include <stdio.h>
    #include "dstructures.h"

    int main() {
        // Initialize cross section data structure
        XSEC_GBC xsec_t* xs = init_xsec(4);
            
        // Add some example cross-section points
        push_xsec(xs, 100.0, 0.1);  // 100 barns at 0.1 MeV
        push_xsec(xs, 50.0, 1.0);   // 50 barns at 1.0 MeV
        push_xsec(xs, 10.0, 10.0);  // 10 barns at 10.0 MeV
        push_xsec(xs, 1.0, 100.0);  // 1 barn at 100.0 MeV

        // Can also use generic size() macro in place of xsec_size()
        printf("xsec_t populated length: %ld\n", xsec_size(xs));
         
        return 0;
    }

.. code-block:: bash 

   xsec_t populated length: 4

.. _xsec-alloc-func:

.. c:function:: size_t xsec_alloc(const xsec_t* cross_section)

    Returns the allocated capacity of the structure.  The generic :ref:`alloc <alloc-macro>`
    macro can also be used in place of this function.

    :param cross_section: Target structure
    :return: Allocated capacity, or 0 on failure
    :errno: ``EINVAL`` if invalid structure pointer

.. code-block:: c

    #include <stdio.h>
    #include "dstructures.h"

    int main() {
        // Initialize cross section data structure
        XSEC_GBC xsec_t* xs = init_xsec(10);
            
        // Add some example cross-section points
        push_xsec(xs, 100.0, 0.1);  // 100 barns at 0.1 MeV
        push_xsec(xs, 50.0, 1.0);   // 50 barns at 1.0 MeV
        push_xsec(xs, 10.0, 10.0);  // 10 barns at 10.0 MeV
        push_xsec(xs, 1.0, 100.0);  // 1 barn at 100.0 MeV

        // Can also use generic alloc() macro in place of xsec_size()
        printf("xsec_t allocated length: %ld\n", xsec_alloc(xs));
         
        return 0;
    }

.. code-block:: bash 

   xsec_t allocated length: 10

.. c:function:: const float* get_xsec_xsArray(const xsec_t* xsec)

    Retrieves a pointer to the internal cross-section array. The returned array 
    is read-only to maintain data encapsulation.

    :param xsec: Source structure
    :return: Const pointer to cross-section array, or NULL on failure
    :errno: ``EINVAL`` if invalid structure pointer or NULL array

    .. warning::
        The returned pointer is only valid for the lifetime of the xsec_t structure. 
        Do not store this pointer beyond the lifetime of the structure.

.. c:function:: const float* get_xsec_enArray(const xsec_t* xsec)

    Retrieves a pointer to the internal energy array. The returned array 
    is read-only to maintain data encapsulation.

    :param xsec: Source structure
    :return: Const pointer to energy array, or NULL on failure
    :errno: ``EINVAL`` if invalid structure pointer or NULL array

    .. warning::
        The returned pointer is only valid for the lifetime of the xsec_t structure. 
        Do not store this pointer beyond the lifetime of the structure.

    Example usage of array access functions:

    .. code-block:: c

        void print_arrays(const xsec_t* xs) {
            const float* cross_sections = get_xsec_xsArray(xs);
            const float* energies = get_xsec_enArray(xs);
            
            if (!cross_sections || !energies) {
                fprintf(stderr, "Failed to get arrays\n");
                return;
            }
            
            size_t size = xsec_size(xs);
            for (size_t i = 0; i < size; i++) {
                printf("E: %f MeV, XS: %f barns\n", 
                       energies[i], cross_sections[i]);
            }
        }

Implementation Details
======================

Memory Management Strategy
--------------------------
The structure implements a hybrid growth strategy:

- For allocations < 1MB: Exponential growth (doubles capacity)
- For allocations ≥ 1MB: Linear growth (adds 1MB increments)

This approach optimizes for both small and large datasets by avoiding excessive memory 
allocation while maintaining good performance characteristics.

Thread Safety
-------------
All getter functions are const-correct and thread-safe for reading. However, modification 
functions (``push_xsec``) are not thread-safe and should be protected by appropriate 
synchronization mechanisms if used in a multi-threaded context.

Error Handling
--------------
The API uses a combination of return values and errno to indicate errors:

- NULL or -1.0f return values typically indicate errors
- errno is set to provide detailed error information
- All errors are logged to stderr with contextual information

Common error conditions:

- Memory errors (``ENOMEM``): During initialization or structure growth
- Invalid operations (``EINVAL``): NULL pointers or invalid indices
- Range errors (``ERANGE``): Out-of-bounds energy values during interpolation

.. _xsec-gbc-macro:

Memory Safety Macros
--------------------

.. c:macro:: XSEC_GBC

    When compiling with GCC or Clang, this macro provides automatic cleanup functionality 
    for xsec_t pointers when they go out of scope. This is the preferred method for 
    managing xsec_t memory in GCC/Clang environments as it helps prevent memory leaks.

    Usage example:

    .. code-block:: c

        void example_function(void) {
            XSEC_GBC xsec_t* xs = init_xsec(100);
            if (!xs) {
                // Handle error
                return;
            }
            // Use xs...
            // No need to call free_xsec - cleanup happens automatically
        }

    The macro expands to:

    .. code-block:: c

        #if defined(__GNUC__) || defined(__clang__)
            #define XSEC_GBC __attribute__((cleanup(_free_xsec)))
        #endif

    When the XSEC_GBC-decorated pointer goes out of scope, the compiler automatically 
    calls the cleanup function _free_xsec, which safely deallocates all associated 
    memory. This provides RAII-like behavior in C, similar to smart pointers in C++.

    :note: This macro only works with GCC and Clang compilers. When using other 
           compilers, manual memory management with :ref:`free_xsec() <xsec-free-func>` or
           ``free_data()`` is required.

    :warning: Even with XSEC_GBC, explicitly calling free_xsec() on the pointer 
             before it goes out of scope will cause a double-free error.

Usage Examples
==============

Basic Usage
-----------
The following example demonstrates common operations with the xsec_t structure:

.. code-block:: c

    #include <stdio.h>
    #include <errno.h>
    #include "dstructures.h"

    int main(void) {
        // Initialize with automatic cleanup (GCC/Clang only)
        XSEC_GBC xsec_t* xs = init_xsec(10);
        if (!xs) {
            fprintf(stderr, "Failed to initialize xsec\n");
            return 1;
        }

        // Add some cross-section data points
        float energies[] = {0.1, 1.0, 10.0, 100.0};
        float xsecs[] = {100.0, 50.0, 10.0, 1.0};
        
        for (size_t i = 0; i < 4; i++) {
            if (!push_xsec(xs, xsecs[i], energies[i])) {
                fprintf(stderr, "Failed to add data point\n");
                return 1;
            }
        }

        // Interpolate at various energies
        float test_energies[] = {0.5, 5.0, 50.0};
        for (size_t i = 0; i < 3; i++) {
            float result = interp_xsec(xs, test_energies[i]);
            if (result < 0) {
                fprintf(stderr, "Interpolation failed at energy %f\n", test_energies[i]);
                continue;
            }
            printf("Cross section at E=%f: %f\n", test_energies[i], result);
        }

        // Access individual data points
        for (size_t i = 0; i < xsec_size(xs); i++) {
            xsecData point = get_xsec_data(xs, i);
            printf("Point %zu: E=%f, XS=%f\n", i, point.energy, point.xs);
        }

        return 0;
    }

Memory Management Example
-------------------------
This example demonstrates different memory management approaches:

.. code-block:: c

    // GCC/Clang automatic cleanup approach
    void process_data_gcc(void) {
        XSEC_GBC xsec_t* xs = init_xsec(100);
        if (!xs) {
            return;
        }
        // Use xs...
        // Cleanup happens automatically
    }

    // Manual cleanup approach (for other compilers)
    void process_data_manual(void) {
        xsec_t* xs = init_xsec(100);
        if (!xs) {
            return;
        }
        // Use xs...
        free_xsec(xs);  // Must explicitly free
    }

Dynamic Growth Example
----------------------
This example shows how the structure handles dynamic growth:

.. code-block:: c

    void growth_example(void) {
        XSEC_GBC xsec_t* xs = init_xsec(2);  // Start small
        if (!xs) {
            return;
        }

        // Structure will grow automatically as needed
        for (float e = 0.1; e <= 1000.0; e *= 1.5) {
            float cross_section = 100.0 / e;  // Example calculation
            if (!push_xsec(xs, cross_section, e)) {
                fprintf(stderr, "Failed to add point at E=%f\n", e);
                return;
            }
        }

        printf("Final size: %zu\n", xsec_size(xs));
        printf("Allocated capacity: %zu\n", xsec_alloc(xs));
    }

These examples demonstrate:

- Proper initialization and error checking
- Data population and access
- Interpolation usage
- Memory management approaches
- Dynamic growth behavior

The structure automatically handles memory management and growth while providing 
safe access to the underlying data through its API.
