*************************
Dictionary Data Structure
*************************

.. module:: dict
   :synopsis: Hash table implementation for string keys and float values

Overview
========
The ``dict_t`` data structure provides a hash table implementation specifically designed 
for storing string keys mapped to float values. It uses separate chaining with linked 
lists to handle collisions and implements automatic resizing based on load factor.

Features
--------
- Dynamic hash table with separate chaining collision resolution
- String keys mapped to float values
- Automatic resizing based on load factor
- Thread-safe access through const-correctness
- Automatic memory management support through GCC/Clang attributes
- Comprehensive error handling using errno
- Deep copy of keys for memory safety

Internal Structure
==================

.. c:type:: dictNode

   A node structure used in the hash table's collision chains.

    .. code-block:: c

        typedef struct dictNode {
            char* key;      /* String key (dynamically allocated) */
            float value;    /* Associated float value */
            struct dictNode* next;  /* Pointer to next node in chain */
        } dictNode;

.. c:type:: dict_t

   An opaque structure that manages the hash table. This struct is encapsulated 
   within the ``dstructures.c`` file and is not directly accessible to developers.

    .. code-block:: c

        struct dict_t {
            dictNode* keyValues;   /* Array of bucket head nodes */
            size_t hash_size;      /* Total number of key-value pairs */
            size_t len;           /* Number of non-empty buckets */
            size_t alloc;         /* Total number of buckets */
        };

Hash Function
-------------
The implementation uses the DJB2 hash algorithm, known for its good distribution 
and speed for string keys:

.. code-block:: c

    static size_t hash_function(const char* key) {
        size_t hash = 5381;
        int c;
        while ((c = *key++))
            hash = ((hash << 5) + hash) + c;  // hash * 33 + c
        return hash;
    }

API Reference
=============

Initialization and Cleanup
--------------------------
The functions described in this section are used to initialize dynamically 
allocated ``dict_t`` data types and to free them from memory. The ``dict_t`` struct 
is defined in the ``dstructures.c`` file, and therefore can not be directly accessed 
by the developer.

.. c:function:: dict_t* init_dict()

   Initializes a new dictionary with a default initial capacity. If the code is
   compiled with ``gcc`` or ``clang``, the developer should consider using the 
   :ref:`DICT_GBC <dict-gbc-macro>` macro with this function to automate garbage 
   collection once the ``dict_t`` data type goes out of scope.

   :return: Pointer to initialized dictionary, or NULL on failure
   :errno: ``ENOMEM`` on memory allocation failure

   Example usage:

   .. code-block:: c

       #include "dstructures.h"
       #include <stdio.h>
       #include <stdlib.h>

       int main() {
           // Initialize with automatic cleanup (GCC/Clang)
           DICT_GBC dict_t* dict = init_dict();
           if (!dict) {
               fprintf(stderr, "Dictionary initialization failed\n");
               return EXIT_FAILURE;
           }

           // Add some key-value pairs
           insert_dict(dict, "key1", 1.0);
           insert_dict(dict, "key2", 2.0);

           printf("Dictionary size: %zu\n", dict_size(dict));
           return EXIT_SUCCESS;
       }

   .. code-block:: bash

       Dictionary size: 2

.. _dict-free-func:

.. c:function:: void free_dict(vector_t* vec)

   Deallocates all memory associated with the dictionary. This function 
   can be used directly to free ``dict_t`` data from memory, or the developer 
   can also use the :ref:`free_data <free-data-macro>` macro as a generic macro that can be used 
   to free memory for any data type in this code base.

   :param vec: Pointer to vector to be freed
   :errno: ``EINVAL`` if NULL pointer passed

Code Examples

.. code-block:: c

   #include "dstructures.h"

   int main() {
       // Initialize xsec_t variable
       dict_t* dict = init_dict();
       // Free dict_t variable when it is no longer needed 
       free_dict(dict);
       // Can also use free_data(dict) to free memory
   }

Dictionary Operations
---------------------

.. c:function:: bool insert_dict(dict_t* dict, char* key, float value)

   Inserts a new key-value pair into the dictionary. If the key already exists,
   the insertion fails. The function automatically resizes the dictionary if the
   load factor exceeds the threshold.

   :param dict: Target dictionary
   :param key: String key to insert
   :param value: Float value to associate with key
   :return: ``true`` on success, ``false`` if key exists or on error
   :errno: 
       - ``EINVAL`` if key already exists
       - ``ENOMEM`` if memory allocation fails

   Example usage with collision handling:

   .. code-block:: c

       #include "dstructures.h"
       #include <stdio.h>
       #include <stdlib.h>

       int main() {
           DICT_GBC dict_t* dict = init_dict();
           if (!dict) {
               return EXIT_FAILURE;
           }

           // Insert some values that might collide
           insert_dict(dict, "abc", 1.0);
           insert_dict(dict, "cba", 2.0);  // Might hash to same bucket

           // Try to insert duplicate key
           if (!insert_dict(dict, "abc", 3.0)) {
               printf("Duplicate key rejected as expected\n");
           }

           printf("Value for 'abc': %f\n", get_dict_value(dict, "abc"));
           printf("Value for 'cba': %f\n", get_dict_value(dict, "cba"));
           printf("Total pairs: %zu\n", dict_hash_size(dict));
           return EXIT_SUCCESS;
       }

   .. code-block:: bash

       Duplicate key rejected as expected
       Value for 'abc': 1.000000
       Value for 'cba': 2.000000
       Total pairs: 2

.. c:function:: float pop_dict(dict_t* dict, char* key)

   Removes and returns the value associated with the specified key. If the key 
   does not exist, returns FLT_MAX.

   :param dict: Target dictionary
   :param key: Key to remove
   :return: Associated value, or FLT_MAX if key not found
   :errno: ``EINVAL`` if key not found

   Example usage:

   .. code-block:: c

       #include "dstructures.h"
       #include <stdio.h>
       #include <stdlib.h>
       #include <float.h>

       int main() {
           DICT_GBC dict_t* dict = init_dict();
           if (!dict) {
               return EXIT_FAILURE;
           }

           insert_dict(dict, "test", 42.0);
           
           float val = pop_dict(dict, "test");
           if (val != FLT_MAX) {
               printf("Popped value: %f\n", val);
           }

           // Try to pop non-existent key
           val = pop_dict(dict, "test");
           if (val == FLT_MAX) {
               printf("Key no longer exists\n");
           }

           return EXIT_SUCCESS;
       }

   .. code-block:: bash

       Popped value: 42.000000
       Key no longer exists

.. c:function:: const float get_dict_value(const dict_t* dict, char* key)

   Retrieves the value associated with the specified key without removing it.

   :param dict: Source dictionary
   :param key: Key to look up
   :return: Associated value, or FLT_MAX if key not found
   :errno: ``EINVAL`` if key not found

   Example usage:

   .. code-block:: c

       #include "dstructures.h"
       #include <stdio.h>
       #include <stdlib.h>
       #include <float.h>

       int main() {
           DICT_GBC dict_t* dict = init_dict();
           if (!dict) {
               return EXIT_FAILURE;
           }

           insert_dict(dict, "pi", 3.14159);
           
           float val = get_dict_value(dict, "pi");
           if (val != FLT_MAX) {
               printf("Value of pi: %f\n", val);
           }

           // Try to get non-existent key
           val = get_dict_value(dict, "e");
           if (val == FLT_MAX) {
               printf("Key 'e' not found\n");
           }

           return EXIT_SUCCESS;
       }

   .. code-block:: bash

       Value of pi: 3.141590
       Key 'e' not found

.. c:function:: bool update_dict(dict_t* dict, char* key, float value)

   Updates the value associated with an existing key. If the key doesn't exist,
   no action is taken.

   :param dict: Target dictionary
   :param key: Key to update
   :param value: New value to associate with key
   :return: ``true`` if key was found and updated, ``false`` otherwise
   :errno: ``EINVAL`` if key not found

   Example usage:

   .. code-block:: c

       #include "dstructures.h"
       #include <stdio.h>
       #include <stdlib.h>

       int main() {
           DICT_GBC dict_t* dict = init_dict();
           if (!dict) {
               return EXIT_FAILURE;
           }

           insert_dict(dict, "temp", 98.6);
           printf("Initial temp: %f\n", get_dict_value(dict, "temp"));

           // Update existing key
           if (update_dict(dict, "temp", 99.1)) {
               printf("Updated temp: %f\n", get_dict_value(dict, "temp"));
           }

           // Try to update non-existent key
           if (!update_dict(dict, "humidity", 45.0)) {
               printf("Could not update non-existent key\n");
           }

           return EXIT_SUCCESS;
       }

   .. code-block:: bash

       Initial temp: 98.600000
       Updated temp: 99.100000
       Could not update non-existent key

Utility Functions
-----------------

.. _dict-size-func:

.. c:function:: const size_t dict_size(const dict_t* dict)

   Returns the number of non-empty buckets in the dictionary. The generic 
   :ref:`size <size-macro>` macro can also be used in place of this function.

   :param dict: Target dictionary
   :return: Number of non-empty buckets
   :errno: ``EINVAL`` if dict is NULL

.. _dict-alloc-func:

.. c:function:: const size_t dict_alloc(const dict_t* dict)

   Returns the total number of buckets allocated in the dictionary. The generic 
   :ref:`alloc <alloc-macro>` macro can also be used in place of this function.

   :param dict: Target dictionary
   :return: Total number of buckets
   :errno: ``EINVAL`` if dict is NULL

.. c:function:: const size_t dict_hash_size(const dict_t* dict)

   Returns the total number of key-value pairs stored in the dictionary.

   :param dict: Target dictionary
   :return: Number of key-value pairs
   :errno: ``EINVAL`` if dict is NULL

   Example usage of utility functions:

   .. code-block:: c

       #include "dstructures.h"
       #include <stdio.h>
       #include <stdlib.h>

       int main() {
           DICT_GBC dict_t* dict = init_dict();
           if (!dict) {
               return EXIT_FAILURE;
           }

           // Add several keys that might hash to same bucket
           insert_dict(dict, "abc", 1.0);
           insert_dict(dict, "cba", 2.0);
           insert_dict(dict, "bac", 3.0);

           printf("Total buckets allocated: %zu\n", dict_alloc(dict));
           printf("Non-empty buckets: %zu\n", dict_size(dict));
           printf("Total key-value pairs: %zu\n", dict_hash_size(dict));

           return EXIT_SUCCESS;
       }

   .. code-block:: bash

       Total buckets allocated: 8
       Non-empty buckets: 2
       Total key-value pairs: 3

Memory Management
=================

Automatic Cleanup
-----------------

.. _dict-gbc-macro:

.. c:macro:: DICT_GBC

   When compiling with GCC or Clang, this macro enables automatic cleanup of
   dict_t pointers when they go out of scope.

   Example usage:

   .. code-block:: c

       void example_function(void) {
           DICT_GBC dict_t* dict = init_dict();
           if (!dict) {
               return;
           }
           // Use dict...
           // No need to call free_dict - cleanup happens automatically
       }

Hash Table Growth Strategy
--------------------------
The dictionary implements a hybrid growth strategy based on load factor:

- When load factor exceeds threshold (0.7):
  - For allocations < 1MB: Double capacity
  - For allocations ≥ 1MB: Add fixed increment (1MB)

This approach balances memory usage with performance by:
- Using exponential growth for small tables to amortize resizing cost
- Using linear growth for large tables to avoid excessive memory usage

Collision Resolution
--------------------
The implementation uses separate chaining with linked lists to handle hash collisions:

- Each bucket contains a sentinel head node
- Collisions are handled by creating a chain of nodes
- Each chain maintains key uniqueness
- Lookups traverse the chain to find exact matches

Example of collision handling:

.. code-block:: c

    #include "dstructures.h"
    #include <stdio.h>
    #include <stdlib.h>

    int main() {
        DICT_GBC dict_t* dict = init_dict();
        if (!dict) {
            return EXIT_FAILURE;
        }

        // These keys might hash to the same bucket
        insert_dict(dict, "cat", 1.0);
        insert_dict(dict, "tac", 2.0);
        insert_dict(dict, "act", 3.0);

        // All values are still accessible
        printf("cat: %f\n", get_dict_value(dict, "cat"));
        printf("tac: %f\n", get_dict_value(dict, "tac"));
        printf("act: %f\n", get_dict_value(dict, "act"));

        // Remove middle element from chain
        pop_dict(dict, "tac");
        
        // Other elements still accessible
        printf("After removal:\n");
        printf("cat: %f\n", get_dict_value(dict, "cat"));
        printf("act: %f\n", get_dict_value(dict, "act"));

        return EXIT_SUCCESS;
    }

.. code-block:: bash

    cat: 1.000000
    tac: 2.000000
    act: 3.000000
    After removal:
    cat: 1.000000
    act: 3.000000

Usage Examples
==============

Basic Dictionary Operations
---------------------------

.. code-block:: c

    #include "dstructures.h"
    #include <stdio.h>
    #include <stdlib.h>

    int main(void) {
        // Initialize dictionary with automatic cleanup
        DICT_GBC dict_t* dict = init_dict();
        if (!dict) {
            return EXIT_FAILURE;
        }

        // Insert some initial values
        insert_dict(dict, "temperature", 98.6);
        insert_dict(dict, "pressure", 1013.25);
        insert_dict(dict, "humidity", 45.7);

        // Read and display values
        printf("Temperature: %.1f°F\n", get_dict_value(dict, "temperature"));
        printf("Pressure: %.1f hPa\n", get_dict_value(dict, "pressure"));
        printf("Humidity: %.1f%%\n", get_dict_value(dict, "humidity"));

        // Update a value
        update_dict(dict, "temperature", 99.1);
        printf("Updated temperature: %.1f°F\n", get_dict_value(dict, "temperature"));

        // Remove a value
        float old_pressure = pop_dict(dict, "pressure");
        printf("Removed pressure: %.1f hPa\n", old_pressure);

        return EXIT_SUCCESS;
    }

.. code-block:: bash

    Temperature: 98.6°F
    Pressure: 1013.2 hPa
    Humidity: 45.7%
    Updated temperature: 99.1°F
    Removed pressure: 1013.2 hPa

Error Handling Examples
-----------------------

.. code-block:: c

    void error_handling_demo(void) {
        DICT_GBC dict_t* dict = init_dict();
        if (!dict) {
            fprintf(stderr, "Failed to initialize dictionary\n");
            return;
        }

        // Handle duplicate key insertion
        insert_dict(dict, "key1", 1.0);
        if (!insert_dict(dict, "key1", 2.0)) {
            fprintf(stderr, "Duplicate key rejected as expected\n");
        }

        // Handle non-existent key access
        float val = get_dict_value(dict, "nonexistent");
        if (val == FLT_MAX) {
            fprintf(stderr, "Non-existent key access handled\n");
        }

        // Handle update of non-existent key
        if (!update_dict(dict, "nonexistent", 1.0)) {
            fprintf(stderr, "Update of non-existent key handled\n");
        }

        // Handle removal of non-existent key
        val = pop_dict(dict, "nonexistent");
        if (val == FLT_MAX) {
            fprintf(stderr, "Removal of non-existent key handled\n");
        }
    }

Load Factor and Resizing Example
--------------------------------

.. code-block:: c

    void demonstrate_resizing(void) {
        DICT_GBC dict_t* dict = init_dict();
        if (!dict) {
            return;
        }

        printf("Initial capacity: %zu\n", dict_alloc(dict));

        // Insert many items to trigger resizing
        char key[10];
        for (int i = 0; i < 100; i++) {
            snprintf(key, sizeof(key), "key%d", i);
            insert_dict(dict, key, (float)i);
            
            if (i % 10 == 0) {
                printf("After %d insertions:\n", i + 1);
                printf("  Total pairs: %zu\n", dict_hash_size(dict));
                printf("  Bucket capacity: %zu\n", dict_alloc(dict));
                printf("  Non-empty buckets: %zu\n", dict_size(dict));
            }
        }
    }

Common Use Case: Temperature Recording
--------------------------------------

.. code-block:: c

    void temperature_recording(void) {
        DICT_GBC dict_t* dict = init_dict();
        if (!dict) {
            return;
        }

        // Record temperatures for different cities
        insert_dict(dict, "New York", 72.5);
        insert_dict(dict, "London", 65.2);
        insert_dict(dict, "Tokyo", 78.9);
        insert_dict(dict, "Sydney", 82.4);

        // Update temperatures as they change
        update_dict(dict, "New York", 74.1);
        update_dict(dict, "London", 66.8);

        // Print all temperatures
        const char* cities[] = {"New York", "London", "Tokyo", "Sydney"};
        for (size_t i = 0; i < 4; i++) {
            float temp = get_dict_value(dict, cities[i]);
            if (temp != FLT_MAX) {
                printf("%s: %.1f°F\n", cities[i], temp);
            }
        }

        // Remove cities we're no longer tracking
        pop_dict(dict, "Sydney");
        
        printf("\nNumber of cities tracked: %zu\n", dict_hash_size(dict));
    }

Thread Safety Notes
-------------------
- All getter functions are const-correct and thread-safe for reading
- Modification functions (insert_dict, pop_dict, update_dict) are not thread-safe
- When used in a multi-threaded context, external synchronization is required
- The hash function is stateless and thread-safe

Performance Considerations
--------------------------
- Hash function distribution affects collision rates
- Load factor threshold of 0.7 balances space and time efficiency
- Separate chaining provides good performance even with collisions
- Key comparisons use strcmp, so very long keys may impact performance
- Memory overhead per key-value pair includes:
    - Key string storage
    - Node structure (key pointer, value, next pointer)
    - Potential chain traversal time for collisions
