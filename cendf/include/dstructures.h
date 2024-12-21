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
 * struct will be encapsulated, preventing a user from dirrectly accessing it.
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
float get_xsec(xsec_t* cross_section, size_t index);
// --------------------------------------------------------------------------------

/**
 * @function get_xsec_energy
 * @brief Retrieves the energy value at a specified index.
 *
 * @param cross_section Pointer to the `xsec_t` structure.
 * @param index The index of the desired energy value.
 * @return The energy value at the given index, or -1.0f on error (sets `errno` to EINVAL).
 */
float get_xsec_energy(xsec_t* cross_section, size_t index);
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
const xsecData get_xsec_data(xsec_t* cross_section, size_t index);
// --------------------------------------------------------------------------------

/**
 * @function xsec_size
 * @brief Retrieves the current number of elements in the `xsec` structure.
 *
 * @param cross_section Pointer to the `xsec_t` structure.
 * @return The number of elements stored in the structure, or 0 if the structure is NULL.
 */
size_t xsec_size(xsec_t* cross_section);
// --------------------------------------------------------------------------------

/**
 * @function xsec_alloc
 * @brief Retrieves the allocated capacity of the `xsec_t` structure.
 *
 * @param cross_section Pointer to the `xsec` structure.
 * @return The allocated capacity of the structure, or 0 if the structure is NULL.
 */
size_t xsec_alloc(xsec_t* cross_section);
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

void _free_xsec(xsec_t** cross_section); 

#if defined(__GNUC__) || defined(__clang__)
    #define XSEC_GBC __attribute__((cleanup(_free_xsec)))
#endif
// ================================================================================
// ================================================================================ 
#ifdef __cplusplus
}
#endif /* cplusplus */
#endif /* dstructures_H */
// ================================================================================
// ================================================================================
// eof
