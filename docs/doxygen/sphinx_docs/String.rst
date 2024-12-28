*****************************
Dynamic String Data Structure
*****************************

.. module:: string
   :synopsis: Dynamic string implementation with memory management

Overview
========
The ``string_t`` data structure provides a dynamic string implementation with automatic 
memory management. It offers safe string manipulation operations while handling memory 
allocation automatically. The structure encapsulates a C-string and provides a safer 
interface for string operations compared to raw C-strings.

Features
--------
- Dynamic memory allocation for string operations
- Safe string concatenation with both string_t and C-strings
- Lexicographical string comparison operations
- Memory reservation capabilities to optimize allocations
- Thread-safe string access through const-correctness
- Automatic memory management support through GCC/Clang attributes
- Comprehensive error handling using errno
- Deep copy functionality

Data Structure
==============

.. c:type:: string_t

   An opaque structure that manages dynamic string data. This struct is
   encapsulated within the ``dstructures.c`` file and is not directly accessible
   to a developer.

    .. code-block:: c

        typedef struct {
            char* str;     /* Pointer to the string data */
            size_t len;    /* Current length of the string */
            size_t alloc;  /* Allocated capacity including null terminator */
        } string_t;

API Reference
=============

Initialization and Cleanup
--------------------------
The functions described in this section are used to initialize dynamically 
allocated ``string_t`` data types and to free them from memory. The ``string_t`` struct 
is defined in the ``dstructures.c`` file, and therefore can not be directly accessed 
by the developer. This struct can only be dynamically created with the 
``init_string()`` function, and therefore must also be freed from memory after use.

.. c:function:: string_t* init_string(const char* str)

   Initializes a new string_t object with the contents of a C-string. If the code is
   compiled with ``gcc`` or ``clang``, the developer should consider using the 
   :ref:`STRING_GBC <string-gbc-macro>` macro with this function to automate garbage 
   collection once the ``string_t`` data type goes out of scope.

   :param str: Null-terminated C string to initialize with
   :return: Pointer to initialized structure, or NULL on failure
   :errno: 
       - ``ENOMEM`` on memory allocation failure
       - ``EINVAL`` if str is NULL

   Example usage:

   .. code-block:: c

       #include "dstructures.h"
       #include <stdio.h>
       #include <stdlib.h>

       int main() {
           // Initialize string_t with normal cleanup
           string_t* str1 = init_string("Hello");
           if (!str1) {
               fprintf(stderr, "String initialization failed\n");
               return EXIT_FAILURE;
           }
           printf("String 1: %s\n", get_string(str1));
           
           // Initialize with automatic cleanup (GCC/Clang only)
           STRING_GBC string_t* str2 = init_string("World");
           if (!str2) {
               free_string(str1);  // Don't forget to clean up str1
               return EXIT_FAILURE;
           }
           printf("String 2: %s\n", get_string(str2));
           
           free_string(str1);  // Manual cleanup for str1
           return EXIT_SUCCESS; // str2 cleaned up automatically
       }

   .. code-block:: bash

       String 1: Hello
       String 2: World

String Access Operations
------------------------
These functions provide safe access to the string data and its properties.

.. c:function:: const char* get_string(const string_t* str)

   Retrieves the underlying C-string from a string_t object. Returns a const 
   pointer to ensure the string cannot be modified through this access method.

   :param str: Source string_t object
   :return: Pointer to null-terminated C string, or NULL on failure
   :errno: ``EINVAL`` if str is NULL or invalid

   Example usage:

   .. code-block:: c

       #include "dstructures.h"
       #include <stdio.h>
       #include <stdlib.h>

       int main() {
           STRING_GBC string_t* str = init_string("Hello, World!");
           if (!str) {
               return EXIT_FAILURE;
           }

           const char* cstr = get_string(str);
           if (!cstr) {
               fprintf(stderr, "Failed to get string: %s\n", strerror(errno));
               return EXIT_FAILURE;
           }

           printf("String content: %s\n", cstr);
           return EXIT_SUCCESS;
       }

   .. code-block:: bash

       String content: Hello, World!

.. c:function:: const size_t string_size(const string_t* str)

   Returns the current length of the string (excluding null terminator). The generic 
   :ref:`size <size-macro>` macro can also be used in place of this function.

   :param str: Target string object
   :return: Length of string, or 0 on failure
   :errno: ``EINVAL`` if str is NULL or invalid

   Example usage:

   .. code-block:: c

       #include "dstructures.h"
       #include <stdio.h>
       #include <stdlib.h>

       int main() {
           STRING_GBC string_t* str = init_string("Hello");
           if (!str) {
               return EXIT_FAILURE;
           }

           // Can use either direct function or size macro
           size_t len = string_size(str);  // or size(str)
           printf("String length: %zu\n", len);
           
           return EXIT_SUCCESS;
       }

   .. code-block:: bash

       String length: 5

.. c:function:: const size_t string_alloc(const string_t* str)

   Returns the total allocated capacity of the string buffer (including null terminator).
   The generic :ref:`alloc <alloc-macro>` macro can also be used in place of this function.

   :param str: Target string object
   :return: Allocated capacity, or 0 on failure
   :errno: ``EINVAL`` if str is NULL or invalid

   Example usage:

   .. code-block:: c

       #include "dstructures.h"
       #include <stdio.h>
       #include <stdlib.h>

       int main() {
           STRING_GBC string_t* str = init_string("Hello");
           if (!str) {
               return EXIT_FAILURE;
           }

           // Can use either direct function or alloc macro
           size_t capacity = string_alloc(str);  // or alloc(str)
           printf("String length: %zu\n", string_size(str));
           printf("Allocated capacity: %zu\n", capacity);
           
           return EXIT_SUCCESS;
       }

   .. code-block:: bash

       String length: 5
       Allocated capacity: 6

String Manipulation
-------------------

.. c:function:: bool string_string_concat(string_t* str1, const string_t* str2)

   Concatenates two string_t objects, appending str2 to str1. Memory is 
   automatically reallocated if needed. This function is called automatically by
   the generic :ref:`string_concat <string-concat-macro>` macro when both arguments
   are string_t types.

   :param str1: Destination string object
   :param str2: Source string object to append
   :return: true on success, false on failure
   :errno: 
       - ``EINVAL`` if either string is NULL or invalid
       - ``ENOMEM`` if memory reallocation fails

   Example usage:

   .. code-block:: c

       #include "dstructures.h"
       #include <stdio.h>
       #include <stdlib.h>

       int main() {
           STRING_GBC string_t* str1 = init_string("Hello, ");
           STRING_GBC string_t* str2 = init_string("World!");
           if (!str1 || !str2) {
               return EXIT_FAILURE;
           }

           if (!string_string_concat(str1, str2)) {
               fprintf(stderr, "Concatenation failed: %s\n", strerror(errno));
               return EXIT_FAILURE;
           }

           printf("Result: %s\n", get_string(str1));
           return EXIT_SUCCESS;
       }

   .. code-block:: bash

       Result: Hello, World!

.. c:function:: bool string_lit_concat(string_t* str1, const char* literal)

   Concatenates a C-string literal to a string_t object. Memory is automatically 
   reallocated if needed. This function is called automatically by the generic 
   :ref:`string_concat <string-concat-macro>` macro when the second argument is a 
   C-string.

   :param str1: Destination string object
   :param literal: C-string to append
   :return: true on success, false on failure
   :errno: 
       - ``EINVAL`` if either argument is NULL
       - ``ENOMEM`` if memory reallocation fails

   Example usage:

   .. code-block:: c

       #include "dstructures.h"
       #include <stdio.h>
       #include <stdlib.h>

       int main() {
           STRING_GBC string_t* str = init_string("Hello");
           if (!str) {
               return EXIT_FAILURE;
           }

           // Using direct function
           if (!string_lit_concat(str, ", World!")) {
               fprintf(stderr, "Concatenation failed: %s\n", strerror(errno));
               return EXIT_FAILURE;
           }

           // Or using generic macro
           if (!string_concat(str, " How are you?")) {
               fprintf(stderr, "Concatenation failed: %s\n", strerror(errno));
               return EXIT_FAILURE;
           }

           printf("Result: %s\n", get_string(str));
           return EXIT_SUCCESS;
       }

   .. code-block:: bash

       Result: Hello, World! How are you?

.. _string-concat-macro:

.. c:macro:: string_concat(str_one, str_two)

   A generic macro that automatically selects the appropriate concatenation function 
   based on the type of the second argument. Uses ``string_lit_concat`` for C-string 
   literals and ``string_string_concat`` for string_t objects.

   Example usage shown in the examples above.

String Comparison Operations
----------------------------

.. c:function:: int compare_strings_lit(const string_t* str_struct, const char* string)

   Performs a lexicographical comparison between a string_t object and a C-string. 
   This function is called automatically by the generic :ref:`compare_strings <string-compare-macro>` 
   macro when the second argument is a C-string.

   :param str_struct: string_t object to compare
   :param string: C-string to compare against
   :return: 
       - < 0 if str_struct is lexicographically less than string
       - 0 if strings are equal
       - > 0 if str_struct is lexicographically greater than string
       - INT_MIN on error
   :errno: ``EINVAL`` if either argument is NULL

   Example usage:

   .. code-block:: c

       #include "dstructures.h"
       #include <stdio.h>
       #include <stdlib.h>

       int main() {
           STRING_GBC string_t* str = init_string("Hello");
           if (!str) {
               return EXIT_FAILURE;
           }

           int result = compare_strings_lit(str, "Hello");
           printf("Comparison with 'Hello': %d\n", result);

           result = compare_strings_lit(str, "World");
           printf("Comparison with 'World': %d\n", result);

           result = compare_strings_lit(str, "Hat");
           printf("Comparison with 'Hat': %d\n", result);

           return EXIT_SUCCESS;
       }

   .. code-block:: bash

       Comparison with 'Hello': 0
       Comparison with 'World': -15  # Exact value may vary
       Comparison with 'Hat': 1      # Exact value may vary

.. c:function:: int compare_strings_string(const string_t* str_struct_one, string_t* str_struct_two)

   Performs a lexicographical comparison between two string_t objects. This function 
   is called automatically by the generic :ref:`compare_strings <string-compare-macro>` 
   macro when both arguments are string_t objects.

   :param str_struct_one: First string_t object to compare
   :param str_struct_two: Second string_t object to compare
   :return: 
       - < 0 if str_struct_one is lexicographically less than str_struct_two
       - 0 if strings are equal
       - > 0 if str_struct_one is lexicographically greater than str_struct_two
       - INT_MIN on error
   :errno: ``EINVAL`` if either argument is NULL

   Example usage:

   .. code-block:: c

       #include "dstructures.h"
       #include <stdio.h>
       #include <stdlib.h>

       int main() {
           STRING_GBC string_t* str1 = init_string("Hello");
           STRING_GBC string_t* str2 = init_string("Hello");
           STRING_GBC string_t* str3 = init_string("World");
           if (!str1 || !str2 || !str3) {
               return EXIT_FAILURE;
           }

           int result = compare_strings_string(str1, str2);
           printf("'Hello' vs 'Hello': %d\n", result);

           result = compare_strings_string(str1, str3);
           printf("'Hello' vs 'World': %d\n", result);

           result = compare_strings(str1, str3);
           printf("'Hello' vs 'World' woth compare_strings': %d\n", result);

           return EXIT_SUCCESS;
       }

   .. code-block:: bash

       'Hello' vs 'Hello': 0
       'Hello' vs 'World': -15  # Exact value may vary
       'Hello' vs 'World' with compare_strings: -15  # Exact value may vary

.. _string-compare-macro:

.. c:macro:: compare_string(str_one, str_two)

   A generic macro that automatically selects the appropriate compare function 
   based on the type of the second argument. Uses ``compare_string_lit`` for C-string 
   literals and ``compare_string_string`` for string_t objects.

   Example usage shown in the examples above.

.. c:function:: string_t* copy_string(const string_t* str)

   Creates a deep copy of a string_t object, including allocation capacity. 
   The new string_t object maintains the same allocated capacity as the source.

   :param str: string_t object to copy
   :return: Pointer to new string_t object, or NULL on failure
   :errno: 
       - ``EINVAL`` if str is NULL
       - ``ENOMEM`` if memory allocation fails

   Example usage:

   .. code-block:: c

       #include "dstructures.h"
       #include <stdio.h>
       #include <stdlib.h>

       int main() {
           STRING_GBC string_t* str1 = init_string("Hello, World!");
           if (!str1) {
               return EXIT_FAILURE;
           }

           string_t* str2 = copy_string(str1);
           if (!str2) {
               fprintf(stderr, "Copy failed: %s\n", strerror(errno));
               return EXIT_FAILURE;
           }

           printf("Original string: %s\n", get_string(str1));
           printf("Copied string: %s\n", get_string(str2));
           printf("Original allocated: %zu\n", string_alloc(str1));
           printf("Copy allocated: %zu\n", string_alloc(str2));

           free_string(str2);  // Manual cleanup for copy
           return EXIT_SUCCESS;
       }

   .. code-block:: bash

       Original string: Hello, World!
       Copied string: Hello, World!
       Original allocated: 14
       Copy allocated: 14

.. c:function:: bool reserve_string(string_t* str, size_t len)

   Pre-allocates memory for a string to avoid frequent reallocations during 
   operations like concatenation. The requested length must be larger than the 
   current allocation.

   :param str: Target string_t object
   :param len: New capacity to allocate
   :return: true on success, false on failure
   :errno: 
       - ``EINVAL`` if str is NULL or len <= current allocation
       - ``ENOMEM`` if memory allocation fails

   Example usage:

   .. code-block:: c

       #include "dstructures.h"
       #include <stdio.h>
       #include <stdlib.h>

       int main() {
           STRING_GBC string_t* str = init_string("Hello");
           if (!str) {
               return EXIT_FAILURE;
           }

           printf("Initial allocation: %zu\n", string_alloc(str));

           // Reserve space for a longer string
           if (!reserve_string(str, 100)) {
               fprintf(stderr, "Reservation failed: %s\n", strerror(errno));
               return EXIT_FAILURE;
           }

           printf("After reservation: %zu\n", string_alloc(str));

           // Attempt to reserve less space (should fail)
           if (!reserve_string(str, 50)) {
               printf("Cannot reduce allocation as expected\n");
           }

           return EXIT_SUCCESS;
       }

   .. code-block:: bash

       Initial allocation: 6
       After reservation: 100
       Cannot reduce allocation as expected

Memory Management
=================

Automatic Cleanup
-----------------

.. _string-gbc-macro:

.. c:macro:: STRING_GBC

   When compiling with GCC or Clang, this macro enables automatic cleanup of
   string_t pointers when they go out of scope. This is the preferred method
   for managing string_t memory in GCC/Clang environments.

   Example usage:

   .. code-block:: c

       void example_function(void) {
           STRING_GBC string_t* str = init_string("Hello");
           if (!str) {
               return;
           }
           // Use str...
           // No need to call free_string - cleanup happens automatically
       }

   .. warning::
       Even with STRING_GBC, explicitly calling free_string() on the pointer 
       before it goes out of scope will cause a double-free error.

String Concatenation Strategy
-----------------------------
The string concatenation functions implement efficient memory management:

- Current buffer is reused if it has sufficient capacity
- Memory is reallocated only when necessary
- New allocation includes space for null terminator
- All memory operations maintain string validity

Memory Safety Notes
-------------------
- All functions maintain null termination of strings
- Memory is always freed in the correct order during cleanup
- Const correctness prevents modification of source strings
- Failed operations never leave strings in invalid states
- Error conditions always release any partially allocated memory

Usage Examples
==============

Basic String Operations
-----------------------

.. code-block:: c

    #include "dstructures.h"
    #include <stdio.h>
    #include <stdlib.h>

    int main(void) {
        STRING_GBC string_t* str = init_string("Hello");
        if (!str) {
            return EXIT_FAILURE;
        }

        // String concatenation
        string_concat(str, ", ");
        string_concat(str, "World!");

        // String comparison
        if (compare_strings(str, "Hello, World!") == 0) {
            printf("Strings match: %s\n", get_string(str));
        }

        // Get string information
        printf("Length: %zu\n", string_size(str));
        printf("Capacity: %zu\n", string_alloc(str));

        return EXIT_SUCCESS;
    }

Error Handling
--------------

.. code-block:: c

    void error_handling_example(void) {
        STRING_GBC string_t* str = init_string("Test");
        if (!str) {
            fprintf(stderr, "Initialization failed: %s\n", strerror(errno));
            return;
        }

        // Handle concatenation errors
        if (!string_concat(str, NULL)) {
            fprintf(stderr, "Concatenation failed: %s\n", strerror(errno));
        }

        // Handle comparison errors
        int cmp = compare_strings(str, NULL);
        if (cmp == INT_MIN) {
            fprintf(stderr, "Comparison failed: %s\n", strerror(errno));
        }
    }
