***********************
Element Data Structure
***********************

.. module:: element
   :synopsis: Data structure for storing periodic table element properties

Overview
========
The ``element_t`` data structure provides a comprehensive representation of chemical elements,
storing physical properties, electronic configurations, and thermal characteristics. It 
implements thread-safe access through const-correctness and supports automatic memory 
management through GCC/Clang attributes.

Features
--------
- Complete representation of chemical element properties
- Support for multiple melting/boiling points for different element forms
- Vector storage for ionization energies
- Thread-safe access through const-correctness
- Automatic memory management support through GCC/Clang attributes
- Comprehensive error handling using errno
- Deep copy of strings for memory safety

Internal Structure
==================

.. c:type:: element_t

   An opaque structure that holds all properties of a chemical element. This struct 
   is encapsulated within the ``dstructures.c`` file and is not directly accessible 
   to developers.

    .. code-block:: c

        struct element_t {
            string_t* symbol;          /* Chemical symbol (e.g., "H" for Hydrogen) */
            string_t* element;         /* Full element name */
            string_t* category;        /* Element category (e.g., "Noble Gas") */
            size_t atom_num;          /* Atomic number */
            float weight;             /* Atomic weight in amu */
            float electro_neg;        /* Electronegativity */
            dict_t* melting;          /* Melting points for different forms in K */
            dict_t* boiling;          /* Boiling points for different forms in K */
            float electron_affin;     /* Electron affinity in kJ/mol */
            vector_t* ionization;     /* Ionization energies in kJ */
            float radius;             /* Atomic radius in pm */
            float hardness;           /* Hardness in Vickers scale */
            float modulus;            /* Elastic modulus in GPa */
            float density;            /* Density in kg/m³ */
            float therm_cond;         /* Thermal conductivity in W/(m·K) */
            float electric_cond;      /* Electrical conductivity in MS/m */
            float specific_heat;      /* Specific heat in J/(kg·K) */
            float vaporization;       /* Heat of vaporization in kJ/mol */
            float fusion_heat;        /* Heat of fusion in kJ/mol */
            string_t* electron_config; /* Electron configuration */
        };

API Reference
=============

Initialization and Cleanup
--------------------------

.. c:function:: element_t* fetch_element(const char* element)

   Creates and populates an element_t structure by reading data from a JSON file.
   
   :param element: Chemical symbol of the element to fetch (e.g., "H" for Hydrogen)
   :return: Pointer to initialized element_t structure, or NULL on failure
   :errno: 
       - ``ENOENT`` if file doesn't exist
       - ``EINVAL`` if element not found
       - ``ENOMEM`` if memory allocation fails

   Example usage:

   .. code-block:: c

       #include "dstructures.h"
       #include <stdio.h>

       int main() {
           ELEMENT_GBC element_t* hydrogen = fetch_element("H");
           if (!hydrogen) {
               fprintf(stderr, "Failed to load hydrogen data\n");
               return EXIT_FAILURE;
           }

           printf("Hydrogen atomic number: %zu\n", element_atomic_number(hydrogen));
           return EXIT_SUCCESS;
       }

   .. code-block:: bash 

      Hydrogen atomic number: 1

.. c:function:: void free_element(element_t* elem)

   Deallocates all memory associated with an element_t structure.

   :param elem: Pointer to element structure to free
   :errno: ``EINVAL`` if NULL pointer passed

Property Accessors
------------------
All accessor functions follow similar patterns:
- Return -1.0f for unavailable numeric data
- Return NULL for unavailable string/collection data
- Set errno to EINVAL for invalid inputs
- Set errno to ENODATA for unavailable data
- Thread-safe for reading operations

Basic Properties
~~~~~~~~~~~~~~~~

.. c:function:: const string_t* element_symbol(const element_t* elem)

   Gets the chemical symbol of the element.

   :return: Element symbol (e.g., "H" for Hydrogen) or NULL if invalid

.. c:function:: const string_t* element_element(const element_t* elem)

   Gets the full name of the element.

   :return: Element name (e.g., "Hydrogen") or NULL if invalid

.. c:function:: const string_t* element_category(const element_t* elem)

   Gets the element's category.

   :return: Category name (e.g., "Noble Gas") or NULL if invalid

Physical Properties
~~~~~~~~~~~~~~~~~~~

.. c:function:: const size_t element_atomic_number(const element_t* elem)

   Gets the element's atomic number.

   :return: Atomic number or LONG_MAX if invalid

.. c:function:: const float element_weight(const element_t* elem)

   Gets the atomic weight in atomic mass units (amu).

   :return: Atomic weight or -1.0f if invalid

.. c:function:: const float element_electroneg(const element_t* elem)

   Gets the element's electronegativity.

   :return: Electronegativity value or -1.0f if invalid/unavailable

Thermal Properties
~~~~~~~~~~~~~~~~~~

.. c:function:: const dict_t* element_melting_point(const element_t* elem)

   Gets melting points for different forms of the element in Kelvin.

   :return: Dictionary of melting points or NULL if invalid

.. c:function:: const dict_t* element_boiling_point(const element_t* elem)

   Gets boiling points for different forms of the element in Kelvin.

   :return: Dictionary of boiling points or NULL if invalid

Electronic Properties
~~~~~~~~~~~~~~~~~~~~~

.. c:function:: const float element_electron_affin(const element_t* elem)

   Gets electron affinity in kJ/mol.

   :return: Electron affinity or -1.0f if invalid/unavailable

.. c:function:: const vector_t* element_ionization(const element_t* elem)

   Gets successive ionization energies in kJ.

   :return: Vector of ionization energies or NULL if invalid

.. c:function:: const string_t* element_electron_config(const element_t* elem)

   Gets electron configuration string.

   :return: Electron configuration or NULL if invalid

Physical Measurements
~~~~~~~~~~~~~~~~~~~~~

.. c:function:: const float element_radius(const element_t* elem)

   Gets atomic radius in picometers.

   :return: Atomic radius or -1.0f if invalid/unavailable

.. c:function:: const float element_hardness(const element_t* elem)

   Gets Vickers hardness value.

   :return: Hardness value or -1.0f if invalid/unavailable

.. c:function:: const float element_modulus(const element_t* elem)

   Gets elastic modulus in GPa.

   :return: Elastic modulus or -1.0f if invalid/unavailable

.. c:function:: const float element_density(const element_t* elem)

   Gets density in kg/m³.

   :return: Density or -1.0f if invalid/unavailable

Thermal and Electrical Properties
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. c:function:: const float element_thermal_cond(const element_t* elem)

   Gets thermal conductivity in W/(m·K).

   :return: Thermal conductivity or -1.0f if invalid/unavailable

.. c:function:: const float element_electrical_cond(const element_t* elem)

   Gets electrical conductivity in MS/m.

   :return: Electrical conductivity or -1.0f if invalid/unavailable

.. c:function:: const float element_specific_heat(const element_t* elem)

   Gets specific heat capacity in J/(kg·K).

   :return: Specific heat or -1.0f if invalid/unavailable

.. c:function:: const float element_vaporization_heat(const element_t* elem)

   Gets heat of vaporization in kJ/mol.

   :return: Heat of vaporization or -1.0f if invalid/unavailable

.. c:function:: const float element_fusion_heat(const element_t* elem)

   Gets heat of fusion in kJ/mol.

   :return: Heat of fusion or -1.0f if invalid/unavailable

Memory Management
=================

Automatic Cleanup
-----------------

.. _element-gbc-macro:

.. c:macro:: ELEMENT_GBC

   When compiling with GCC or Clang, enables automatic cleanup of element_t
   pointers when they go out of scope.

   Example usage:

   .. code-block:: c

       void example_function(void) {
           ELEMENT_GBC element_t* element = fetch_element("Fe");
           if (!element) {
               return;
           }
           // Use element...
           // No need to call free_element - cleanup happens automatically
       }

Usage Examples
==============

Basic Element Operations
------------------------

.. code-block:: c

    #include "dstructures.h"
    #include <stdio.h>
    #include <stdlib.h>

    int main(void) {
        // Initialize element with automatic cleanup
        ELEMENT_GBC element_t* iron = fetch_element("Fe");
        if (!iron) {
            return EXIT_FAILURE;
        }

        // Access various properties
        printf("Element: %s (%s)\n", 
               string_get(element_element(iron)),
               string_get(element_symbol(iron)));
        printf("Category: %s\n", string_get(element_category(iron)));
        printf("Atomic Number: %zu\n", element_atomic_number(iron));
        printf("Atomic Weight: %.3f amu\n", element_weight(iron));
        printf("Density: %.2f kg/m³\n", element_density(iron));

        return EXIT_SUCCESS;
    }

.. code-block:: bash 

   Category: Transition Metal 
   Atomic Number: 26
   Atomic Weight: 55.845
   Density: 7.87

Error Handling Example
----------------------

.. code-block:: c

    void error_handling_demo(void) {
        // Try to fetch non-existent element
        ELEMENT_GBC element_t* invalid = fetch_element("Xx");
        if (!invalid) {
            fprintf(stderr, "Invalid element not found, as expected\n");
        }

        // Handle missing property
        ELEMENT_GBC element_t* helium = fetch_element("He");
        if (helium) {
            float hardness = element_hardness(helium);
            if (hardness == -1.0f) {
                fprintf(stderr, "No hardness data for gas, as expected\n");
            }
        }
    }

.. code-block:: bash 

   Invalid element not found, as expected 
   No hardness data for gas, as expected

Thread Safety Notes
-------------------
- All accessor functions are const-correct and thread-safe for reading
- The fetch_element function requires external synchronization if called concurrently
- When used in a multi-threaded context, external synchronization is required for initialization

Performance Considerations
--------------------------
- JSON parsing occurs only during fetch_element
- All accessors are O(1) operations
- Memory overhead includes:
    - String allocations for text fields
    - Dictionary allocations for melting/boiling points
    - Vector allocation for ionization energies
    - Struct memory for element_t itself
