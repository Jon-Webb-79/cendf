**********************
Generic Utility Macros
**********************

Overview
========
This module provides generic macros for common operations across different data structures
in the library. Using C11's ``_Generic`` feature, these macros automatically dispatch to
the appropriate function based on the data structure type.

Generic Macros
==============

.. _size-macro:

.. c:macro:: size(d_struct)

   Returns the current number of elements in a data structure as a ``size_t`` data type. 
   Dispatches to the appropriate size function based on the data structure type.

   :param d_struct: Pointer to a supported data structure
   :return: Number of elements in the structure
   
   Supported types:
       - ``xsec_t*``: Calls :ref:`xsec_size <xsec-size-func>`
       - ``string_t*``: Calls :ref:`string_size <string-size-func>`
       - ``vector_t*``: Calls :ref:`vector_size <vec-size-func>`
       - ``dict_t*``: Calls :ref:`dict_size <dict-size-func>`

   Example usage:

   .. code-block:: c

       xsec_t* xs = init_xsec(10);
       size_t num_elements = size(xs);  // Calls xsec_size(xs)

       vector_t* vec = init_vector(5);
       size_t vec_elements = size(vec); // Calls vector_size(vec)

.. _alloc-macro:

.. c:macro:: alloc(d_struct)

   Returns the total allocated capacity of a data structure as a ``size_t`` data type. 
   Dispatches to the appropriate allocation function based on the data structure type.

   :param d_struct: Pointer to a supported data structure
   :return: Allocated capacity of the structure
   
   Supported types:
       - ``xsec_t*``: Calls :ref:`xsec_alloc <xsec-alloc-func>`
       - ``string_t*``: Calls :ref:`string_alloc <string-alloc-func>`
       - ``vector_t*``: Calls :ref:`vector_alloc <vec-alloc-func>`
       - ``dict_t*``: Calls :ref:`dict_alloc <dict-alloc-func>`

   Example usage:

   .. code-block:: c

       xsec_t* xs = init_xsec(10);
       size_t capacity = alloc(xs);  // Calls xsec_alloc(xs)

       vector_t* vec = init_vector(5);
       size_t vec_capacity = alloc(vec); // Calls vector_alloc(vec)

.. _free-data-macro:

.. c:macro:: free_data(d_struct)

   Frees memory associated with a data structure. Dispatches to the appropriate 
   free function based on the data structure type, with a fallback to standard 
   ``free()`` for unsupported types.  This macro encompasses functions with a 
   ``void`` data type.

   :param d_struct: Pointer to any data structure
   
   Supported types:
       - ``xsec_t*``: :ref:`free_xsec <xsec-free-func>`
       - ``string_t*``: Calls :ref:`free_string <string-free-func>`
       - ``vector_t*``: Calls :ref:`free_vector <vec-free-func>`
       - ``dict_t*``: Calls :ref:`free_dict <dict-free-func>`
       - Default: Calls standard ``free``

   Example usage:

   .. code-block:: c

       // Using with xsec_t
       xsec_t* xs = init_xsec(10);
       // ... use xs ...
       free_data(xs);  // Calls free_xsec(xs)

       // Using with vector_t
       vector_t* vec = init_vector(5);
       // ... use vec ...
       free_data(vec); // Calls free_vector(vec)

       // Using with standard malloc'd memory
       int* array = malloc(sizeof(int) * 10);
       // ... use array ...
       free_data(array); // Calls standard free(array)

   .. note::
       When using with GCC or Clang, consider using the :ref:`XSEC_GBC <xsec-gbc-macro>` 
       macro for automatic cleanup of ``xsec_t`` structures instead of manually calling 
       ``free_data``.

Implementation Details
======================
These macros use C11's ``_Generic`` keyword to provide compile-time function dispatch 
based on the type of the input pointer. This allows for generic operations across 
different data structure types while maintaining type safety and avoiding void pointers.
