*****************************
Dynamic Vector Data Structure
*****************************

.. module:: vector
   :synopsis: Dynamic array implementation for float values

Overview
========
The ``vector_t`` data structure provides a dynamic array implementation for float values,
offering automatic memory management and array operations. It supports insertions and 
deletions at any position while handling memory allocation automatically.

Features
--------
- Dynamic memory allocation with hybrid growth strategy
- Operations for front, back, and arbitrary position insertion/deletion
- Thread-safe data access through const-correctness
- Automatic memory management support through GCC/Clang attributes
- Comprehensive error handling using errno
- Deep copy functionality

Data Structure
==============

.. c:type:: vector_t

   An opaque structure that manages a dynamic array of float values. This struct is
   encapsulated within the ``dstructures.c`` file and is not directly accessible
   to a developer.

    .. code-block:: c

        typedef struct {
            float data;  /* Pointer to an array of float values */
            size_t alloc;  /* allocated size of the xs and energy arrays */ 
            size_t len;    /* populated size o the xs and energy arrays */
        } vector_t;

API Reference
=============

Initialization and Cleanup
--------------------------
The functions described in this section are used to initialize dynamically 
allocated ``vector_t`` data types and to free them from memory. The ``vector_t`` struct is 
defined in the ``dstructures.c`` file, and therefore can not be directly accessed 
by the developer. This struct can only be dynamically created with the 
``init_vector()`` function, and therefore must also be freed from memory after use.

.. c:function:: vector_t* init_vector(size_t len)

   Initializes a new vector with specified initial capacity.  If the code is
   compiled with ``gcc`` or ``clang``, the developer should consider using the 
   :ref:`VECTOR_GBC <vector-gbc-macro>` with this function to automate garbage 
   collection once the ``vector_t`` data type goes out of scope.

   :param len: Initial capacity for the vector
   :return: Pointer to initialized structure, or NULL on failure
   :errno: ``ENOMEM`` on memory allocation failure

.. code-block:: c

    #include "dstructures.h"

    int main() {
        // Initialize xsec_t variable
        vector_t* vec = init_vector(10);
        // Free vector_t variable when it is no longer needed 
        free_data(xsec);
        
        /* If the code is compiled with gcc or clang, use the VECTOR_GBC macro 
         * to automate garbage collection (free_data()) is no longer required 
         */
        vector_t* new_vec VECTOR_GBC = init_xsec(1);
        // free_data() is no longer required
    }

.. _vec-free-func:

.. c:function:: void free_vector(vector_t* vec)

   Deallocates all memory associated with the vector. This function 
   can be used directly to free ``vector_t`` data from memory, or the developer 
   can also use the :ref:`free_data <free-data-macro>` macro as a generic macro that can be used 
   to free memory for any data type in this code base.

   :param vec: Pointer to vector to be freed
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

Insertion Operations
--------------------

.. c:function:: bool push_back_vector(vector_t* vec, float dat)

   Appends a value to the end of the vector. This is the most efficient
   method to write data to a vector and has a time complexity of :math:`O(1)`
   amortized.

   :param vec: Target vector
   :param dat: Value to append
   :return: ``true`` on success, ``false`` on failure
   :errno: 
       - ``EINVAL`` if invalid vector pointer
       - ``ENOMEM`` if memory reallocation failed

Code Example 

.. code-block:: c

   #include "dstructures.h"
   #include <stdio.h>  // For printf
   #include <stdlib.h> // For EXIT FAILURE and EXIT_SUCCESS

   int main() {
       // Initialize vector_t as if code is compiled in gcc or clang
       vector_t* vec VECTOR_GBC = init_xsec(5);

       float temp[5] = {1.f, 2.f, 3.f, 4.f, 5.f};
       for (size_t i = 0; i < 5; i++) {
           if (!push_back_vector(vec, temp[i])) {
               return EXIT_FAILURE;
           }
       }
       printf("[ ");
        for (size_t i = 0; i < size(vec) - 1; i++) {
            printf("%f, ", get_vector(vec, i));
        }
        printf("%f ]\n", get_vector(vec, size(vec)));
       // If not compiled in gcc or clang use free_data(vec)
       return EXIT_SUCCESS;
   }

.. code-block:: bash 

   [ 1.0000, 2.0000, 3.0000, 4.0000, 5.0000 ]

.. c:function:: bool push_front_vector(vector_t* vec, float dat)

   Inserts a value at the beginning of the vector.  This is the least efficient 
   method of populating a vector and has a time complexity of :math:`O(n)`

   :param vec: Target vector
   :param dat: Value to insert
   :return: ``true`` on success, ``false`` on failure
   :errno: 
       - ``EINVAL`` if invalid vector pointer
       - ``ENOMEM`` if memory reallocation failed

Code Example 

.. code-block:: c

   #include "dstructures.h"
   #include <stdio.h>  // For printf
   #include <stdlib.h> // For EXIT FAILURE and EXIT_SUCCESS

   int main() {
       // Initialize vector_t as if code is compiled in gcc or clang
       vector_t* vec VECTOR_GBC = init_xsec(5);

       float temp[5] = {1.f, 2.f, 3.f, 4.f, 5.f};
       for (size_t i = 0; i < 5; i++) {
           if (!push_front_vector(vec, temp[i])) {
               return EXIT_FAILURE;
           }
       }
       printf("[ ");
        for (size_t i = 0; i < size(vec) - 1; i++) {
            printf("%f, ", get_vector(vec, i));
        }
        printf("%f ]\n", get_vector(vec, size(vec)));
       // If not compiled in gcc or clang use free_data(vec)
       return EXIT_SUCCESS;
   }

.. code-block:: bash 

   [ 5.0000, 4.0000, 3.0000, 2.0000, 1.0000 ]

.. c:function:: bool insert_vector(vector_t* vec, float dat, size_t index)

   Inserts a value at the specified position. This method of inserting data into 
   a reactor can have a time complexity ranging from :math:`O(1)` to :math:`O(n)`
   depending on whether data is inserted closer to the front of, or the back of
   the vector.

   :param vec: Target vector
   :param dat: Value to insert
   :param index: Position for insertion
   :return: ``true`` on success, ``false`` on failure
   :errno: 
       - ``EINVAL`` if invalid vector pointer
       - ``ENOMEM`` if memory reallocation failed
       - ``ERANGE`` if index out of bounds

Code Example 

.. code-block:: c

   #include "dstructures.h"
   #include <stdio.h>  // For printf
   #include <stdlib.h> // For EXIT FAILURE and EXIT_SUCCESS

   int main() {
       // Initialize vector_t as if code is compiled in gcc or clang
       vector_t* vec VECTOR_GBC = init_xsec(5);

       float temp[5] = {1.f, 2.f, 3.f, 4.f, 5.f};
       for (size_t i = 0; i < 5; i++) {
           if (!push_back_vector(vec, temp[i])) {
               return EXIT_FAILURE;
           }
       }

       // Trusting function execution and not checking for a false return
       insert_vector(vec, 12, 1);
       insert_vector(vec, 13, 2);

       printf("[ ");
        for (size_t i = 0; i < size(vec) - 1; i++) {
            printf("%f, ", get_vector(vec, i));
        }
        printf("%f ]\n", get_vector(vec, size(vec)));
       // If not compiled in gcc or clang use free_data(vec)
       return EXIT_SUCCESS;
   }

.. code-block:: bash 

   [ 1.0000, 12.0000, 13.0000, 2.0000, 3.0000, 4.0000, 5.0000 ]

Removal Operations
------------------

.. c:function:: float pop_back_vector(vector_t* vec)

   Removes and returns the last element.  This is the most time efficeint data 
   removal operation and has a time complexity of :math:`O(1)`;

   :param vec: Target vector
   :return: Removed value, or FLT_MAX on failure
   :errno: ``EINVAL`` if vector is NULL or empty

Code Example 

.. code-block:: c

   #include "dstructures.h"
   #include <stdio.h>  // For printf
   #include <stdlib.h> // For EXIT FAILURE and EXIT_SUCCESS
   #include <math.h>   // for fabs

   bool isclose(float a, float b, float rel_tol) {
       float diff = fabs(a - b);
       return diff <= fmax(rel_tol * fmax(fabs(a), fabs(b)), abs_tol);
   }

   int main() {
       // Initialize vector_t as if code is compiled in gcc or clang
       vector_t* vec VECTOR_GBC = init_xsec(5);

       float temp[5] = {1.f, 2.f, 3.f, 4.f, 5.f};
       for (size_t i = 0; i < 5; i++) {
           if (!push_back_vector(vec, temp[i])) {
               return EXIT_FAILURE;
           }
       }

       float val = pop_back_vector(vec);
       if (isclose(val, FLT_MAX, 0.0001, 0) && errno == EINVAL)
           return EXIT_FAILURE;
       prinft("Popped value is: %f\n", val);

       printf("[ ");
       for (size_t i = 0; i < size(vec) - 1; i++) {
            printf("%f, ", get_vector(vec, i));
       }
       printf("%f ]\n", get_vector(vec, size(vec)));
       // If not compiled in gcc or clang use free_data(vec)
       return EXIT_SUCCESS;
   }

.. code-block:: bash 

   Popped value is: 5.0000

   [ 1.0000, 2.0000, 3.0000, 4.0000 ]

.. c:function:: float pop_front_vector(vector_t* vec)

   Removes and returns the first element.  This is the least time efficient 
   removal operation and has a time complexity of :math:`O(n)`.

   :param vec: Target vector
   :return: Removed value, or FLT_MAX on failure
   :errno: ``EINVAL`` if vector is NULL or empty

Code Example 

.. code-block:: c

   #include "dstructures.h"
   #include <stdio.h>  // For printf
   #include <stdlib.h> // For EXIT FAILURE and EXIT_SUCCESS
   #include <math.h>   // for fabs

   bool isclose(float a, float b, float rel_tol) {
       float diff = fabs(a - b);
       return diff <= fmax(rel_tol * fmax(fabs(a), fabs(b)), abs_tol);
   }

   int main() {
       // Initialize vector_t as if code is compiled in gcc or clang
       vector_t* vec VECTOR_GBC = init_xsec(5);

       float temp[5] = {1.f, 2.f, 3.f, 4.f, 5.f};
       for (size_t i = 0; i < 5; i++) {
           if (!push_back_vector(vec, temp[i])) {
               return EXIT_FAILURE;
           }
       }

       float val = pop_front_vector(vec);
       if (isclose(val, FLT_MAX, 0.0001, 0) && errno == EINVAL)
           return EXIT_FAILURE;
       prinft("Popped value is: %f\n", val);

       printf("[ ");
       for (size_t i = 0; i < size(vec) - 1; i++) {
            printf("%f, ", get_vector(vec, i));
       }
       printf("%f ]\n", get_vector(vec, size(vec)));
       // If not compiled in gcc or clang use free_data(vec)
       return EXIT_SUCCESS;
   }

.. code-block:: bash 

   Popped value is: 1.0000

   [ 2.0000, 3.0000, 4.0000, 5.0000 ]

.. c:function:: float pop_any_vector(vector_t* vec, size_t index)

   Removes and returns the element at specified index.  This method has a time 
   complexity that ranges from :math:`O(1)` to :math:`O(n)` depending on whether 
   the devoper is removing data from the front side or back side of the vector.

   :param vec: Target vector
   :param index: Position to remove from
   :return: Removed value, or FLT_MAX on failure
   :errno: 
       - ``EINVAL`` if vector is NULL or empty
       - ``ERANGE`` if index out of bounds

Code Example 

.. code-block:: c

   #include "dstructures.h"
   #include <stdio.h>  // For printf
   #include <stdlib.h> // For EXIT FAILURE and EXIT_SUCCESS
   #include <math.h>   // for fabs

   bool isclose(float a, float b, float rel_tol) {
       float diff = fabs(a - b);
       return diff <= fmax(rel_tol * fmax(fabs(a), fabs(b)), abs_tol);
   }

   int main() {
       // Initialize vector_t as if code is compiled in gcc or clang
       vector_t* vec VECTOR_GBC = init_xsec(5);

       float temp[5] = {1.f, 2.f, 3.f, 4.f, 5.f};
       for (size_t i = 0; i < 5; i++) {
           if (!push_back_vector(vec, temp[i])) {
               return EXIT_FAILURE;
           }
       }

       float val = pop_any_vector(vec, 2);
       if (isclose(val, FLT_MAX, 0.0001, 0) && errno == EINVAL)
           return EXIT_FAILURE;
       prinft("Popped value is: %f\n", val);

       printf("[ ");
       for (size_t i = 0; i < size(vec) - 1; i++) {
            printf("%f, ", get_vector(vec, i));
       }
       printf("%f ]\n", get_vector(vec, size(vec)));
       // If not compiled in gcc or clang use free_data(vec)
       return EXIT_SUCCESS;
   }

.. code-block:: bash 

   Popped value is: 3.0000

   [ 1.0000, 2.0000, 4.0000, 5.0000 ]

Access Operations
-----------------

.. c:function:: const float get_vector(const vector_t* vec, size_t index)

   Retrieves value at specified index.

   :param vec: Source vector
   :param index: Position to access
   :return: Value at index, or FLT_MAX on failure
   :errno: 
       - ``EINVAL`` if vector is NULL
       - ``ERANGE`` if index out of bounds

Code Example 

.. code-block:: c

   #include "dstructures.h"
   #include <stdio.h>  // For printf
   #include <stdlib.h> // For EXIT FAILURE and EXIT_SUCCESS

   int main() {
       // Initialize vector_t as if code is compiled in gcc or clang
       vector_t* vec VECTOR_GBC = init_xsec(5);

       float temp[5] = {1.f, 2.f, 3.f, 4.f, 5.f};
       for (size_t i = 0; i < 5; i++) {
           if (!push_back_vector(vec, temp[i])) {
               return EXIT_FAILURE;
           }
       }

       float val = get_vector(vec, 3);
       printf("Value at index 3 is: %d\n", val); 
       // If not compiled in gcc or clang use free_data(vec)
       return EXIT_SUCCESS;
   }

.. code-block:: bash 

   Value at index 3 is: 4.0000

Utility Functions
-----------------

.. _vec-size-func:

.. c:function:: const size_t vector_size(const vector_t* vec)

   Returns number of elements in vector. The generic :ref:`size <size-macro>`
   macro can also be used in place of this function. 

   :param vec: Target vector
   :return: Number of elements, or 0 on failure
   :errno: ``EINVAL`` if invalid vector pointer

.. _vec-alloc-func:

.. c:function:: const size_t vector_alloc(const vector_t* vec)

   Returns allocated capacity of vector. The generic :ref:`alloc <alloc-macro>`
   macro can also be used in place of this function. 

   :param vec: Target vector
   :return: Allocated capacity, or 0 on failure
   :errno: ``EINVAL`` if invalid vector pointer

Code Example 

.. code-block:: c

   #include "dstructures.h"
   #include <stdio.h>  // For printf
   #include <stdlib.h> // For EXIT FAILURE and EXIT_SUCCESS

   int main() {
       // Initialize vector_t as if code is compiled in gcc or clang
       vector_t* vec VECTOR_GBC = init_xsec(10);

       float temp[5] = {1.f, 2.f, 3.f, 4.f, 5.f};
       for (size_t i = 0; i < 5; i++) {
           if (!push_back_vector(vec, temp[i])) {
               return EXIT_FAILURE;
           }
       }
      
       // Can use alloc(vec) as a generic macro
       printf("Vector allocated length is: %ld\n", vector_alloc(vec));
       // Can use size(vec) as a generic macro
       printf("Vector populated length is: %ld\n", vector_size(vec));

       // If not compiled in gcc or clang use free_data(vec)
       return EXIT_SUCCESS;
   }

.. code-block:: bash 

   Vector allocated length is: 10
   Vector populated length is: 5

.. c:function:: vector_t* copy_vector(const vector_t* vec)

   Creates a deep copy of the vector.

   :param vec: Vector to copy
   :return: Pointer to new vector, or NULL on failure
   :errno: 
       - ``EINVAL`` if invalid vector pointer
       - ``ENOMEM`` if memory allocation failed

Code Example 

.. code-block:: c

   #include "dstructures.h"
   #include <stdio.h>  // For printf
   #include <stdlib.h> // For EXIT FAILURE and EXIT_SUCCESS

   int main() {
       // Initialize vector_t as if code is compiled in gcc or clang
       vector_t* vec VECTOR_GBC = init_xsec(10);

       float temp[5] = {1.f, 2.f, 3.f, 4.f, 5.f};
       for (size_t i = 0; i < 5; i++) {
           if (!push_back_vector(vec, temp[i])) {
               return EXIT_FAILURE;
           }
       }
       
       vector_t* new_vec = copy_vector(vec);
       printf("Vector1     Vector2\n\n");
       for (size_t i = 0; i < size(vec); i++) {
            printf("%f     %f\n", get_vector(vec, i), get_vector(new_vec, i));
       }
       printf("\n");
       printf("Vector1 populated length: %ld\n", size(vec));
       printf("Vector2 populated length: %ld\n", size(new_vec));
       printf("Vector1 allocated length: %ld\n", alloc(vec));
       printf("Vector2 populated length: %ld\n", size(new_vec));
       // If not compiled in gcc or clang use free_data(vec)
       return EXIT_SUCCESS;
   }

.. code-block:: bash 

   Vector1     Vector2 
   1.0000      1.0000 
   2.0000      2.0000 
   3.0000      3.0000 
   4.0000      4.0000 
   5.0000      5.0000 

   Vector1 populated length: 5
   Vector2 populated length: 5
   Vector1 allocated length: 10
   Vector2 allocated length: 10

.. c:function:: const float* get_vecArray(const vector_t* vec)

   Retrieves a pointer to the internal float array. The returned array is 
   read-only to maintain data encapsulation.

   :param vec: Source vector
   :return: Const pointer to float array, or NULL on failure
   :errno: ``EINVAL`` if invalid vector pointer or NULL array

   .. warning::
       The returned pointer is only valid for the lifetime of the vector_t 
       structure. Do not store this pointer beyond the lifetime of the structure.

   Example usage:

   .. code-block:: c

       vector_t* vec = init_vector(5);
       push_back_vector(vec, 1.f);
       push_back_vector(vec, 2.f);
       push_back_vector(vec, 3.f);
       push_back_vector(vec, 4.f);
       push_back_vector(vec, 5.f);
       
       const float* array = get_vecArray(vec);
       if (array) {
           for (size_t i = 0; i < size(vec); i++) {
               printf("Value at %zu: %f\n", i, array[i]);
           }
       }

.. code-block:: bash 

   Value at 1: 1.0000 
   Value at 2: 2.0000 
   Value at 3: 3.0000 
   Value at 4: 4.0000 
   Value at 5: 5.0000

Memory Management
=================

Automatic Cleanup
-----------------

.. _vector-gbc-macro:

.. c:macro:: VECTOR_GBC

   When compiling with GCC or Clang, this macro enables automatic cleanup of
   vector_t pointers when they go out of scope.

   Example usage:

   .. code-block:: c

       void example_function(void) {
           VECTOR_GBC vector_t* vec = init_vector(10);
           if (!vec) {
               return;
           }
           // Use vec...
           // No need to call free_vector - cleanup happens automatically
       }

Growth Strategy
---------------
The vector implements a hybrid growth strategy:

- For allocations < 1MB: Exponential growth (doubles capacity)
- For allocations ≥ 1MB: Linear growth (adds 1MB increments)

This approach optimizes for both small and large datasets.

Usage Examples
==============

Basic Operations
----------------

.. code-block:: c

    #include <stdio.h>
    #include "vector.h"

    int main(void) {
        // Initialize vector with automatic cleanup
        VECTOR_GBC vector_t* vec = init_vector(5);
        if (!vec) {
            return 1;
        }

        // Add some values
        push_back_vector(vec, 1.0);
        push_back_vector(vec, 2.0);
        push_front_vector(vec, 0.0);
        insert_vector(vec, 1.5, 2);

        // Access and print values
        for (size_t i = 0; i < vector_size(vec); i++) {
            float val = get_vector(vec, i);
            printf("Value at %zu: %f\n", i, val);
        }

        // Remove values
        float back = pop_back_vector(vec);
        float front = pop_front_vector(vec);
        printf("Removed from back: %f\n", back);
        printf("Removed from front: %f\n", front);

        return 0;
    }

Error Handling
--------------

.. code-block:: c

    void error_handling_example(void) {
        VECTOR_GBC vector_t* vec = init_vector(5);
        if (!vec) {
            fprintf(stderr, "Vector initialization failed\n");
            return;
        }

        // Handle out-of-bounds access
        float val = get_vector(vec, 0);
        if (val == FLT_MAX) {
            fprintf(stderr, "Access failed: %s\n", strerror(errno));
        }

        // Handle invalid insertion
        if (!insert_vector(vec, 1.0, 5)) {
            fprintf(stderr, "Insertion failed: %s\n", strerror(errno));
        }
    }
