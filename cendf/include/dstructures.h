// ================================================================================
// ================================================================================
// - File:    dstructures.h
// - Purpose: Describe the file purpose here
//
// Source Metadata
// - Author:  Jonathan A. Webb
// - Date:    December 18, 2024
// - Version: 1.0
// - Copyright: Copyright 2024, Jon Webb Inc.
// ================================================================================
// ================================================================================
// Include modules here

#ifndef dstructures_H
#define dstructures_H

#include <stdlib.h>   // For size_t
#include <stdbool.h>  // For bool

#ifdef __cplusplus
extern "C" {
#endif
// ================================================================================
// ================================================================================

/**
 * @struct xsec
 * @brief Forward declaration for a dynamic data structure for storing cross-sections and their corresponding energies.
 *
 * This structure manages two parallel arrays (`xs` for cross-sections and `energy` for energy values),
 * along with metadata for the current length and allocated capacity.  The data in this 
 * struct will be encapsulated, preventing a user from directly accessing it.
 *
 * Fields:
 *  - float* xs: Pointer to an array of cross-section values.
 *  - float* energy: Pointer to an array of energy values.
 *  - size_t len: The current number of elements in the arrays.
 *  - size_t alloc: The total allocated capacity of the arrays.
 */
typedef struct xsec_t xsec_t;
// --------------------------------------------------------------------------------

/**
 * @struct xsec_data
 * @brief A structure for holding a single pair of cross-section and energy values.
 *
 * Fields:
 *  - float xs: The cross-section value.
 *  - float energy: The energy value corresponding to the cross-section.
 */
typedef struct {
    float xs;
    float energy;
} xsecData;
// ================================================================================
// ================================================================================

/**
 * @function init_xsec
 * @brief Initializes an `xsec_t` structure with a specified buffer length.
 *
 * Allocates memory for the `xsec_t` structure and its internal arrays, setting the length to 0.
 *
 * @param buffer_length The initial capacity of the cross-section and energy arrays.
 * @return A pointer to the initialized `xsec_t` structure, or NULL on failure (sets `errno` to ENOMEM).
 */
xsec_t* init_xsec(size_t buffer_length);
// --------------------------------------------------------------------------------

/**
 * @function push_xsec
 * @brief Appends a cross-section and energy value to the `xsec` structure.
 *
 * Dynamically grows the arrays if their capacity is exceeded.
 *
 * @param cross_section Pointer to the `xsec_t` structure.
 * @param xsec The cross-section value to append.
 * @param energy The energy value to append.
 * @return true on success, false on failure (sets `errno` to ENOMEM or EINVAL).
 */
bool push_xsec(xsec_t* cross_section, float xsec, float energy);
// --------------------------------------------------------------------------------

/**
 * @function get_xsec
 * @brief Retrieves the cross-section value at a specified index.
 *
 * @param cross_section Pointer to the `xsec_t` structure.
 * @param index The index of the desired cross-section value.
 * @return The cross-section value at the given index, or -1.0f on error (sets `errno` to EINVAL).
 */
const float get_xsec(const xsec_t* cross_section, size_t index);
// --------------------------------------------------------------------------------

/**
 * @function get_xsec_energy
 * @brief Retrieves the energy value at a specified index.
 *
 * @param cross_section Pointer to the `xsec_t` structure.
 * @param index The index of the desired energy value.
 * @return The energy value at the given index, or -1.0f on error (sets `errno` to EINVAL).
 */
const float get_xsec_energy(const xsec_t* cross_section, size_t index);
// --------------------------------------------------------------------------------

/**
 * @function get_xsec_data
 * @brief Retrieves a pair of cross-section and energy values at a specified index.
 *
 * @param cross_section Pointer to the `xsec_t` structure.
 * @param index The index of the desired data.
 * @return An `xsec_data_t` structure containing the values at the given index, or
 *         a structure with values -1.0f if an error occurs (sets `errno` to EINVAL).
 *         The returned struct is immutable!
 */
const xsecData get_xsec_data(const xsec_t* cross_section, size_t index);
// --------------------------------------------------------------------------------

/**
 * @brief Interpolates or retrieves the cross-section value for a given energy
 *        from an xsec_t structure.
 *
 * @param xsec Pointer to an xsec_t structure containing sorted arrays of energy
 *             values and corresponding cross-section values.
 * @param energy The energy value for which the cross-section value is to be
 *               retrieved or interpolated.
 * @return The cross-section value corresponding to the specified energy, or -1.0f
 *         on error.
 *
 * @details
 * - If the `energy` matches an exact value in the `.energy` array of `xsec`,
 *   the corresponding `.xs` value is returned.
 * - If the `energy` lies between two values, the function interpolates the
 *   cross-section using linear interpolation.
 * - If `xsec` or its `.energy` or `.xs` attributes are `NULL`, the function sets
 *   `errno` to `EINVAL`, logs an error to `stderr`, and returns -1.0f.
 * - If the energy is out of bounds, the function sets `errno` to `ERANGE`,
 *   logs an error to `stderr`, and returns -1.0f.
 *
 * @note The function assumes that the `.energy` array in `xsec` is sorted
 *       in ascending order.
 */
const float interp_xsec(const xsec_t* cross_section, float energy);
// -------------------------------------------------------------------------------- 

/**
 * @function xsec_size
 * @brief Retrieves the current number of elements in the `xsec` structure.
 *
 * @param cross_section Pointer to the `xsec_t` structure.
 * @return The number of elements stored in the structure, or 0 if the structure is NULL.
 */
size_t xsec_size(const xsec_t* cross_section);
// --------------------------------------------------------------------------------

/**
 * @function xsec_alloc
 * @brief Retrieves the allocated capacity of the `xsec_t` structure.
 *
 * @param cross_section Pointer to the `xsec` structure.
 * @return The allocated capacity of the structure, or 0 if the structure is NULL.
 */
size_t xsec_alloc(const xsec_t* cross_section);
// --------------------------------------------------------------------------------

/**
 * @function free_xsec
 * @brief Frees all memory associated with the `xsec_t` structure.
 *
 * Cleans up internal arrays and the structure itself, setting pointers to NULL to prevent
 * use-after-free bugs.
 *
 * @param cross_section Pointer to the `xsec_t` structure to be freed.
 * @return true if the structure was successfully freed, false if the input was NULL
 *         (sets `errno` to EINVAL).
 */
void free_xsec(xsec_t* cross_section);
// --------------------------------------------------------------------------------

/**
 * @brief Frees the memory allocated for an xsec_t structure and its associated data.
 *
 * @param cross_section A double pointer to an xsec_t structure to be freed.
 *                      The function sets the pointer to NULL after freeing the memory.
 *
 * @note This function is intended to be used as a cleanup function with GCC or Clang's
 *       __attribute__((cleanup)) mechanism.
 */
void _free_xsec(xsec_t** cross_section); 
// --------------------------------------------------------------------------------

/**
 * @brief Defines a macro for associating a cleanup function (_free_xsec) with
 *        an xsec_t pointer variable in GCC or Clang.
 *
 * @note When this macro is used to declare a variable, the specified cleanup
 *       function is automatically called when the variable goes out of scope.
 *
 * @note This macro is compiler-specific and works only with GCC or Clang.
 */ 
#if defined(__GNUC__) || defined(__clang__)
    #define XSEC_GBC __attribute__((cleanup(_free_xsec)))
#endif
// ================================================================================
// ================================================================================

/**
 * @struct xsec
 * @brief Forward declaration for a dynamic data structure for storing strings.
 *
 * This structre manages an array for float variables along with metadata for 
 * the current length and allocated capacity.  The data in this struct will be
 * encapsulated, preventing a user from directly accessing it.
 *
 * Fields:
 *  - chart* str: Pointer to a string literal
 *  - size_t len: The current number of elements in the arrays.
 *  - size_t alloc: The total allocated capacity of the arrays.
 */
typedef struct string_t string_t;
// --------------------------------------------------------------------------------

/**
 * @function init_string
 * @brief Allocates and initializes a dynamically allocated string_t object.
 *
 * The function initializes the string_t structure with the contents of the provided
 * string, copying the string into dynamically allocated memory.
 *
 * @param str A null-terminated C string to initialize the string_t object with.
 * @return A pointer to the initialized string_t object, or NULL on failure.
 *         Sets errno to ENOMEM if memory allocation fails or EINVAL if `str` is NULL.
 */
string_t* init_string(const char* str);
// --------------------------------------------------------------------------------

/**
 * @function free_string
 * @brief Frees all memory associated with a string_t object.
 *
 * The function releases the memory for the internal string and the string_t structure itself.
 *
 * @param str A pointer to the string_t object to be freed.
 * @return void. Logs an error if `str` is NULL.
 */
void free_string(string_t* str);
// -------------------------------------------------------------------------------- 

/**
 * @function _free_string
 * @brief A helper function for use with cleanup attributes to free string_t objects.
 *
 * This function frees the memory of a string_t object and sets the pointer to NULL.
 *
 * @param str A double pointer to the string_t object to be freed.
 * @return void.
 */
void _free_string(string_t** str);
// --------------------------------------------------------------------------------

#if defined(__GNUC__) || defined (__clang__)
    /**
     * @macro STRING_GBC
     * @brief A macro for enabling automatic cleanup of string_t objects.
     *
     * This macro uses the cleanup attribute to automatically call `_free_string`
     * when the scope ends, ensuring proper memory management.
     */
    #define STRING_GBC __attribute__((cleanup(_free_string)))
#endif
// --------------------------------------------------------------------------------

/**
 * @function get_string
 * @brief Retrieves the C string stored in a string_t object.
 *
 * @param str A pointer to the string_t object.
 * @return A pointer to the null-terminated C string stored in the object,
 *         or NULL if `str` is NULL or invalid. Sets errno to EINVAL on error.
 */
const char* get_string(const string_t* str);
// --------------------------------------------------------------------------------

/**
 * @function string_size
 * @brief Retrieves the current length of the string stored in a string_t object.
 *
 * @param str A pointer to the string_t object.
 * @return The length of the string in bytes (excluding the null terminator),
 *         or -1 on error. Sets errno to EINVAL if `str` is NULL.
 */
size_t string_size(const string_t* str);
// --------------------------------------------------------------------------------

/**
 * @function string_alloc
 * @brief Retrieves the total allocated capacity of the string in a string_t object.
 *
 * @param str A pointer to the string_t object.
 * @return The total allocated capacity in bytes, or -1 on error.
 *         Sets errno to EINVAL if `str` is NULL.
 */
size_t string_alloc(const string_t* str);
// --------------------------------------------------------------------------------

/**
 * @function string_string_concat
 * @brief Concatenates the contents of one string_t object to another.
 *
 * @param str1 A pointer to the destination string_t object.
 * @param str2 A pointer to the source string_t object.
 * @return true if successful, false on failure. Sets errno to ENOMEM if memory
 *         allocation fails or EINVAL if either input is NULL.
 */
bool string_string_concat(string_t* str, const string_t* string);
// -------------------------------------------------------------------------------- 

/**
 * @function string_lit_concat
 * @brief Concatenates a string literal to a string_t object.
 *
 * @param str1 A pointer to the destination string_t object.
 * @param literal A null-terminated C string to append to the string_t object.
 * @return true if successful, false on failure. Sets errno to ENOMEM if memory
 *         allocation fails or EINVAL if either input is NULL.
 */
bool string_lit_concat(string_t* str, const char* string);
// --------------------------------------------------------------------------------

/**
 * @macro string_concat
 * @brief A generic macro that selects the appropriate concatenation function
 *        based on the type of the second argument.
 *
 * If the second argument is a `char*`, it calls `string_lit_concat`.
 * Otherwise, it calls `string_string_concat`.
 */
#define string_concat(str_one, str_two) _Generic((str_two), \
    char*: string_lit_concat, \
    default: string_string_concat) (str_one, str_two)
// --------------------------------------------------------------------------------

/**
 * @function compare_strings_lit
 * @brief Compares a string_t object with a string literal.
 *
 * The comparison is lexicographical and case-sensitive.
 *
 * @param str_struct A pointer to the string_t object.
 * @param string A null-terminated C string to compare with.
 * @return An integer less than, equal to, or greater than zero if the string_t
 *         is found, respectively, to be less than, to match, or to be greater than `string`.
 *         Returns INT_MIN on error (sets errno to EINVAL).
 */
int compare_strings_lit(const string_t* str_struct, const char* string);
// --------------------------------------------------------------------------------

/**
 * @function compare_strings_string
 * @brief Compares two string_t objects.
 *
 * The comparison is lexicographical and case-sensitive.
 *
 * @param str_struct_one A pointer to the first string_t object.
 * @param str_struct_two A pointer to the second string_t object.
 * @return An integer less than, equal to, or greater than zero if the first string_t
 *         is found, respectively, to be less than, to match, or to be greater than the second string_t.
 *         Returns INT_MIN on error (sets errno to EINVAL).
 */
int compare_strings_string(const string_t* str_struct_one, string_t* str_struct_two);
// --------------------------------------------------------------------------------

/**
 * @macro compare_strings
 * @brief A generic macro that selects the appropriate string comparison function
 *        based on the type of the second argument.
 *
 * If the second argument is a `char*`, it calls `compare_strings_lit`.
 * Otherwise, it calls `compare_strings_string`.
 */
#define compare_strings(str_one, str_two) _Generic((str_two), \
    char*: compare_strings_lit, \
    default: compare_strings_string) (str_one, str_two)
// --------------------------------------------------------------------------------

/**
 * @function copy_string
 * @brief Creates a deep copy of a string data type
 *
 * @param str A string_t data type
 * @return A string_t data type with the exact contents of str
 */
string_t* copy_string(string_t *str);
// --------------------------------------------------------------------------------

/**
 * @function reserve_string
 * @brief Reserves memory for the string_t data type to avoid constant memory allocations
 *
 * Will not allow the user to reserve less memory than exists at function call 
 *
 * @param str A string_t data type
 * @param len The buffer length to allocate in string
 * @return true of allocation is successful, false otherwise
 */
bool reserve_string(string_t* str, size_t len);
// ================================================================================ 
// ================================================================================
// VECTOR IMPLEMENTATION

/**
 * @struct xsec
 * @brief Forward declaration for a dynamic data structure to contain a float vector.
 *
 * This structure manages two parallel arrays (`xs` for cross-sections and `energy` for energy values),
 * along with metadata for the current length and allocated capacity.  The data in this 
 * struct will be encapsulated, preventing a user from dirrectly accessing it.
 *
 * Fields:
 *  - float* str: A float pointer
 *  - size_t len: The current number of elements in the arrays.
 *  - size_t alloc: The total allocated capacity of the arrays.
 */
typedef struct vector_t vector_t;
// --------------------------------------------------------------------------------

/**
 * @function init_vector
 * @brief Initializes a dynamically allocated vector_t object.
 *
 * Allocates memory for a vector_t structure and its internal data array.
 *
 * @param len The initial capacity of the vector.
 * @return A pointer to the initialized vector_t object, or NULL on failure.
 *         Sets errno to ENOMEM if memory allocation fails.
 */
vector_t* init_vector(size_t len);
// --------------------------------------------------------------------------------

/**
 * @function push_back_vector
 * @brief Appends a float value to the end of the vector.
 *
 * Dynamically grows the vector if the capacity is exceeded.
 *
 * @param vec A pointer to the vector_t object.
 * @param dat The float value to append.
 * @return true on success, false on failure. Sets errno to ENOMEM or EINVAL.
 */
bool push_back_vector(vector_t* vec, float dat);
// --------------------------------------------------------------------------------

/**
 * @function push_front_vector
 * @brief Inserts a float value at the beginning of the vector.
 *
 * Dynamically grows the vector if the capacity is exceeded, shifting existing elements.
 *
 * @param vec A pointer to the vector_t object.
 * @param dat The float value to insert.
 * @return true on success, false on failure. Sets errno to ENOMEM or EINVAL.
 */
bool push_front_vector(vector_t* vec, float dat);
// --------------------------------------------------------------------------------

/**
 * @function insert_vector
 * @brief Inserts a float value at a specified index in the vector.
 *
 * Dynamically grows the vector if the capacity is exceeded, shifting existing elements.
 *
 * @param vec A pointer to the vector_t object.
 * @param dat The float value to insert.
 * @param index The index where the value should be inserted.
 * @return true on success, false on failure. Sets errno to ENOMEM, EINVAL, or ERANGE.
 */
bool insert_vector(vector_t* vec, float dat, size_t index);
// --------------------------------------------------------------------------------

/**
 * @function pop_back_vector
 * @brief Removes the last element from the vector and returns its value.
 *
 * @param vec A pointer to the vector_t object.
 * @return The removed float value, or FLT_MIN on error. Sets errno to EINVAL if the vector is empty or NULL.
 */
float pop_back_vector(vector_t* vec);
// --------------------------------------------------------------------------------

/**
 * @function pop_front_vector
 * @brief Removes the first element from the vector and returns its value.
 *
 * Shifts all remaining elements to the left to fill the gap.
 *
 * @param vec A pointer to the vector_t object.
 * @return The removed float value, or FLT_MIN on error. Sets errno to EINVAL if the vector is empty or NULL.
 */
float pop_front_vector(vector_t* vec);
// --------------------------------------------------------------------------------

/**
 * @function pop_any_vector
 * @brief Removes the element at the specified index from the vector and returns its value.
 *
 * Shifts all subsequent elements to the left to fill the gap.
 *
 * @param vec A pointer to the vector_t object.
 * @param index The index of the element to remove.
 * @return The removed float value, or FLT_MIN on error. Sets errno to EINVAL or ERANGE.
 */
float pop_any_vector(vector_t* vec, size_t index);
// --------------------------------------------------------------------------------

/**
 * @function get_vector
 * @brief Retrieves the float value at a specified index in the vector.
 *
 * @param vec A pointer to the vector_t object.
 * @param index The index of the element to retrieve.
 * @return The float value at the specified index, or FLT_MIN on error. Sets errno to EINVAL or ERANGE.
 */
float get_vector(vector_t* vec, size_t index);
// --------------------------------------------------------------------------------

/**
 * @function vector_size
 * @brief Retrieves the current number of elements in the vector.
 *
 * @param vec A pointer to the vector_t object.
 * @return The number of elements in the vector, or 0 if the vector is NULL.
 */
const size_t vector_size(vector_t* vec);
// --------------------------------------------------------------------------------

/**
 * @function vector_alloc
 * @brief Retrieves the total allocated capacity of the vector.
 *
 * @param vec A pointer to the vector_t object.
 * @return The allocated capacity of the vector, or 0 if the vector is NULL.
 */
const size_t vector_alloc(vector_t* vec);
// --------------------------------------------------------------------------------

/**
 * @function free_vector
 * @brief Frees all memory associated with the vector_t object.
 *
 * Releases memory for the internal data array and the vector_t structure itself.
 *
 * @param vec A pointer to the vector_t object.
 * @return void. Logs an error if `vec` is NULL.
 */
void free_vector(vector_t* vec);
// --------------------------------------------------------------------------------

/**
 * @function _free_vector
 * @brief A helper function for use with cleanup attributes to free vector_t objects.
 *
 * Frees the memory of a vector_t object and sets the pointer to NULL.
 *
 * @param vec A double pointer to the vector_t object to be freed.
 * @return void.
 */
void _free_vector(vector_t** vec);
// --------------------------------------------------------------------------------

#if defined(__GNUC__) || defined (__clang__)
    /**
     * @macro VECTOR_GBC
     * @brief A macro for enabling automatic cleanup of vector_t objects.
     *
     * This macro uses the cleanup attribute to automatically call `_free_vector`
     * when the scope ends, ensuring proper memory management.
     */
    #define VECTOR_GBC __attribute__((cleanup(_free_vector)))
#endif
// --------------------------------------------------------------------------------
/**
 * @function copy_vector
 * @brief Copies the contents of one vector into another
 *
 * @param vec A pointer to a vector_t data type
 * @return A pointer to a dynamically allocated vector_t object
 */
vector_t* copy_vector(vector_t* vec);
// ================================================================================
// ================================================================================ 
// DICTIONARY PROTOTYPES

/**
 * @typedef dict_t
 * @brief Opaque struct representing a dictionary.
 *
 * This structure encapsulates a hash table that maps string keys to float values.
 * The details of the struct are hidden from the user and managed internally.
 */
typedef struct dict_t dict_t;
// --------------------------------------------------------------------------------

/**
 * @brief Initializes a new dictionary.
 *
 * Allocates and initializes a dictionary object with a default size for the hash table.
 *
 * @return A pointer to the newly created dictionary, or NULL if allocation fails.
 */
dict_t* init_dict();
// --------------------------------------------------------------------------------

/**
 * @brief Inserts a key-value pair into the dictionary.
 *
 * Adds a new key-value pair to the dictionary. If the key already exists, the function
 * does nothing and returns false. If the dictionary's load factor exceeds a threshold,
 * it automatically resizes.
 *
 * @param dict Pointer to the dictionary.
 * @param key The key to insert.
 * @param value The value associated with the key.
 * @return true if the key-value pair was inserted successfully, false otherwise.
 */
bool insert_dict(dict_t* dict, char* key, float value);
// --------------------------------------------------------------------------------

/**
 * @brief Removes a key-value pair from the dictionary.
 *
 * Finds the specified key in the dictionary, removes the associated key-value pair,
 * and returns the value.
 *
 * @param dict Pointer to the dictionary.
 * @param key The key to remove.
 * @return The value associated with the key if it was found and removed; FLT_MAX otherwise.
 */
float pop_dict(dict_t* dict,  char* key);
// --------------------------------------------------------------------------------

/**
 * @brief Retrieves the value associated with a key.
 *
 * Searches the dictionary for the specified key and returns the corresponding value.
 *
 * @param dict Pointer to the dictionary.
 * @param key The key to search for.
 * @return The value associated with the key, or FLT_MAX if the key is not found.
 */
const float get_dict_value(const dict_t* dict, char* key);
// --------------------------------------------------------------------------------

/**
 * @brief Frees the memory associated with the dictionary.
 *
 * Releases all memory allocated for the dictionary, including all key-value pairs.
 *
 * @param dict Pointer to the dictionary to free.
 */
void free_dict(dict_t* dict);
// --------------------------------------------------------------------------------

/**
 * @brief Safely frees a dictionary and sets the pointer to NULL.
 *
 * A wrapper around `free_dict` that ensures the dictionary pointer is also set to NULL
 * after being freed. Useful for preventing dangling pointers.
 *
 * @param dict Pointer to the dictionary pointer to free.
 */
void _free_dict(dict_t** dict);
// --------------------------------------------------------------------------------

#if defined(__GNUC__) || defined (__clang__)
    /**
     * @macro DICT_GBC
     * @brief A macro for enabling automatic cleanup of dict_t objects.
     *
     * This macro uses the cleanup attribute to automatically call `_free_vector`
     * when the scope ends, ensuring proper memory management.
     */
    #define DICT_GBC __attribute__((cleanup(_free_dict)))
#endif
// --------------------------------------------------------------------------------

/**
 * @brief Updates the value associated with a key in the dictionary.
 *
 * Searches for the specified key in the dictionary and updates its value.
 * If the key does not exist, the function takes no action.
 *
 * @param dict Pointer to the dictionary.
 * @param key The key to update.
 * @param value The new value to associate with the key.
 */
void update_dict(dict_t* dict, char* key, float value);
// --------------------------------------------------------------------------------

/**
 * @brief Gets the number of non-empty buckets in the dictionary.
 *
 * Returns the total number of buckets in the hash table that contain at least one key-value pair.
 *
 * @param dict Pointer to the dictionary.
 * @return The number of non-empty buckets.
 */
const size_t dict_size(const dict_t* dict);
// --------------------------------------------------------------------------------

/**
 * @brief Gets the total capacity of the dictionary.
 *
 * Returns the total number of buckets currently allocated in the hash table.
 *
 * @param dict Pointer to the dictionary.
 * @return The total number of buckets in the dictionary.
 */
const size_t dict_alloc(const dict_t* dict);
// --------------------------------------------------------------------------------

/**
 * @brief Gets the total number of key-value pairs in the dictionary.
 *
 * Returns the total number of key-value pairs currently stored in the dictionary.
 *
 * @param dict Pointer to the dictionary.
 * @return The number of key-value pairs.
 */
const size_t dict_hash_size(const dict_t* dict);
// ================================================================================
// ================================================================================

/**
 * @macro size
 * @brief Retrieves the current size (number of elements) of a dynamic data structure.
 *
 * This macro uses `_Generic` to dispatch the appropriate size retrieval function
 * based on the type of the input data structure.
 *
 * Supported types and their corresponding functions:
 *  - `xsec_t*`: Calls `xsec_size`
 *  - `string_t*`: Calls `string_size`
 *  - `vector_t*`: Calls `vector_size`
 *  - `dict_t*`: Calls `dict_size` 
 *
 * @param d_struct A pointer to the data structure (`xsec_t*`, `string_t*`, or `vector_t*`).
 * @return The size of the data structure, as returned by the corresponding function.
 *
 * Example:
 * @code
 * vector_t* vec = init_vector(5);
 * size_t vec_size = size(vec);  // Calls vector_size
 * @endcode
 */
#define size(d_struct) _Generic((d_struct), \
    xsec_t*: xsec_size, \
    string_t*: string_size, \
    vector_t*: vector_size, \
    dict_t*: dict_size) (d_struct)
// --------------------------------------------------------------------------------

/**
 * @macro alloc
 * @brief Retrieves the total allocated capacity of a dynamic data structure.
 *
 * This macro uses `_Generic` to dispatch the appropriate capacity retrieval function
 * based on the type of the input data structure.
 *
 * Supported types and their corresponding functions:
 *  - `xsec_t*`: Calls `xsec_alloc`
 *  - `string_t*`: Calls `string_alloc`
 *  - `vector_t*`: Calls `vector_alloc`
 *  - `dict_t*`: Calls `dict_alloc`
 *
 * @param d_struct A pointer to the data structure (`xsec_t*`, `string_t*`, or `vector_t*`).
 * @return The allocated capacity of the data structure, as returned by the corresponding function.
 *
 * Example:
 * @code
 * vector_t* vec = init_vector(5);
 * size_t vec_alloc = alloc(vec);  // Calls vector_alloc
 * @endcode
 */
#define alloc(d_struct) _Generic((d_struct), \
    xsec_t*: xsec_alloc, \
    string_t*: string_alloc, \
    vector_t*: vector_alloc, \
    dict_t*: dict_alloc) (d_struct)
// --------------------------------------------------------------------------------

/**
 * @macro free_data
 * @brief Frees the memory associated with a dynamic data structure.
 *
 * This macro uses `_Generic` to dispatch the appropriate free function
 * based on the type of the input data structure. If the type does not match
 * one of the supported types, it defaults to the standard `free` function.
 *
 * Supported types and their corresponding functions:
 *  - `xsec_t*`: Calls `free_xsec`
 *  - `string_t*`: Calls `free_string`
 *  - `vector_t*`: Calls `free_vector`
 *  - `dict_t*`: Calls `free_dict`
 *  - Default: Calls `free`
 *
 * @param d_struct A pointer to the data structure (`xsec_t*`, `string_t*`, `vector_t*`, or other pointer).
 * @return void.
 *
 * Example:
 * @code
 * vector_t* vec = init_vector(5);
 * free_data(vec);  // Calls free_vector
 * @endcode
 */
#define free_data(d_struct) _Generic((d_struct), \
    xsec_t*: free_xsec, \
    string_t*: free_string, \
    vector_t*: free_vector, \
    dict_t*: free_dict, \
    default: free) (d_struct)
// ================================================================================
// ================================================================================ 
#ifdef __cplusplus
}
#endif /* cplusplus */
#endif /* dstructures_H */
// ================================================================================
// ================================================================================
// eof
