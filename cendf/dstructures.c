// ================================================================================
// ================================================================================
// - File:    dstructures.c
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

#include "include/dstructures.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>

const float LOAD_FACTOR_THRESHOLD = 0.7;
static const size_t XSEC_THRESHOLD = 1 * 1024 * 1024;  // 1 MB
static const size_t XSEC_FIXED_AMOUNT = 1 * 1024 * 1024;  // 1 MB
                                                          
static const size_t hashSize = 3;  //  Size fo hash map initi functions
// ================================================================================
// ================================================================================
// XSEC_T DATA TYPE 

// define xsec_t
struct xsec_t {
    float* xs;
    float* energy;
    size_t len; 
    size_t alloc;
};
// -------------------------------------------------------------------------------- 

xsec_t* init_xsec(size_t buffer_length) {
    xsec_t *struct_ptr = malloc(sizeof(xsec_t));
    if (struct_ptr == NULL) {
        errno = ENOMEM;
        fprintf(stderr, "xsec allocation failed with error %s\n", strerror(errno));
        return NULL;
    }

    float *xsec_ptr = malloc(sizeof(float) * buffer_length);
    if (xsec_ptr == NULL) {
        errno = ENOMEM;
        fprintf(stderr, "xsec allocation failed with error %s\n", strerror(errno));
        free(struct_ptr);
        return NULL;
    }
    float *energy_ptr = malloc(sizeof(float) * buffer_length);
    if (energy_ptr == NULL) {
        errno = ENOMEM;
        fprintf(stderr, "xsec allocation failed with error %s\n", strerror(errno));
        free(struct_ptr);
        free(xsec_ptr);
        return NULL;
    }
    struct_ptr->xs = xsec_ptr;
    struct_ptr->energy = energy_ptr;
    struct_ptr->len = 0;
    struct_ptr->alloc = buffer_length;
    return struct_ptr;
}
// --------------------------------------------------------------------------------

bool push_xsec(xsec_t* cross_section, float xsec, float energy) {
    if (!cross_section || !cross_section->xs || !cross_section->energy) {
        errno = EINVAL;
        fprintf(stderr, "Invalid cross_section passed to push_xsec function\n");
        return false;
    }

    // Check if reallocation is needed
    if (cross_section->alloc <= cross_section->len) {
        size_t new_alloc = cross_section->alloc == 0 ? 1 : cross_section->alloc;
        if (new_alloc < XSEC_THRESHOLD) {
            new_alloc *= 2;  // Exponential growth for smaller allocations
        } else {
            new_alloc += XSEC_FIXED_AMOUNT;  // Fixed growth beyond threshold
        }

        // Attempt to reallocate the cross-section array
        float* new_xs = realloc(cross_section->xs, new_alloc * sizeof(float));
        if (!new_xs) {
            errno = ENOMEM;
            fprintf(stderr, "Failed to reallocate xs in push_xsec\n");
            return false;
        }

        // Attempt to reallocate the energy array
        float* new_energy = realloc(cross_section->energy, new_alloc * sizeof(float));
        if (!new_energy) {
            errno = ENOMEM;
            fprintf(stderr, "Failed to reallocate energy in push_xsec\n");
            free(new_xs);  // Free successfully reallocated xs to prevent memory leak
            return false;
        }

        // Update structure only after successful reallocation
        cross_section->xs = new_xs;
        cross_section->energy = new_energy;
        cross_section->alloc = new_alloc;
    }

    // Append new data
    cross_section->xs[cross_section->len] = xsec;
    cross_section->energy[cross_section->len] = energy;
    cross_section->len++;

    return true;
}
// -------------------------------------------------------------------------------- 

static bool validate_xsec(const xsec_t* cross_section, size_t index) {
    if (!cross_section || !cross_section->xs || !cross_section->energy) {
        errno = EINVAL;
        fprintf(stderr, "Invalid cross_section passed to function\n");
        return false;
    }
    if (index >= cross_section->len) {
        errno = EINVAL;
        fprintf(stderr, "Index %zu out of bounds (len: %zu)\n", index, cross_section->len);
        return false;
    }
    return true;
}

// --------------------------------------------------------------------------------

const float get_xsec(const xsec_t* cross_section, size_t index) {
    if (!validate_xsec(cross_section, index)) {
        return -1.0f;  // Or define a constant for invalid value
    }
    return cross_section->xs[index];
}
// --------------------------------------------------------------------------------

const float get_xsec_energy(const xsec_t* cross_section, size_t index) {
    if (!validate_xsec(cross_section, index)) {
        return -1.0f;  // Or define a constant for invalid value
    }
    return cross_section->energy[index];
}
// --------------------------------------------------------------------------------

const float* get_xsec_xsArray(const xsec_t* xsec) {
    if (!xsec || !xsec->xs) {
        errno = EINVAL;
        fprintf(stderr, "Null pointer to xesc_t or arrays in get_xsec_xsArray\n");
        return NULL;
    }
    return xsec->xs;
}
// --------------------------------------------------------------------------------

const float* get_xsec_enArray(const xsec_t* xsec) {
    if (!xsec || !xsec->xs) {
        errno = EINVAL;
        fprintf(stderr, "Null pointer to xesc_t or arrays in get_xsec_xsArray\n");
        return NULL;
    }
    return xsec->energy;
}
// --------------------------------------------------------------------------------

const xsecData get_xsec_data(const xsec_t* cross_section, size_t index) {
    if (!validate_xsec(cross_section, index)) {
        return (xsecData){ .xs = -1.f, .energy = -1.f };  // Or define an invalid xsec_data value
    }
    return (xsecData){
        .xs = cross_section->xs[index],
        .energy = cross_section->energy[index]
    };
}
// --------------------------------------------------------------------------------

static bool find_indices(const float *array, size_t size, float value, 
                        size_t *lower, size_t *upper) {
    size_t low = 0, high = size - 1;

    if (value < array[low]) {
        *lower = 0;
        *upper = 1;
        errno = ERANGE;
        return false; // Below range
    }
    if (value > array[high]) {
        *lower = high - 1;
        *upper = high;
        errno = ERANGE;
        return false; // Above range
    }

    while (low <= high) {
        size_t mid = low + (high - low) / 2;

        if (array[mid] == value) {
            *lower = *upper = mid;
            return true; // Exact match found
        } else if (array[mid] < value) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    *lower = high;
    *upper = low;
    return false; // Value lies between lower and upper
}
// --------------------------------------------------------------------------------

const float interp_xsec(const xsec_t *xsec, float energy) {
    if (!xsec || !xsec->xs || !xsec->energy) {
        errno = EINVAL;
        fprintf(stderr, "Null pointer passed to inter_xsec function\n");
        return -1.0f;
    }

    if (xsec->len == 0) {
        errno = EINVAL;
        fprintf(stderr, "xsec_t data type not populated with data\n");
        return -1.0f;
    }

    size_t lower, upper;

    if (find_indices(xsec->energy, xsec->len, energy, &lower, &upper)) {
        return xsec->xs[lower]; // Exact match
    }
    if (errno == ERANGE) {
        fprintf(stderr, "Energy is out of bounds for cross section database\n");
        return -1.0f;
    }

    // Perform linear interpolation
    float E1 = xsec->energy[lower];
    float E2 = xsec->energy[upper];
    float XS1 = xsec->xs[lower];
    float XS2 = xsec->xs[upper];

    return XS1 + (XS2 - XS1) * (energy - E1) / (E2 - E1);
}
// --------------------------------------------------------------------------------

size_t xsec_size(const xsec_t* cross_section) {
    if (!cross_section || !cross_section->xs || !cross_section->energy) {
        errno = EINVAL;
        fprintf(stderr, "Invalid cross section passed to xsec_size\n");
        return 0;
    }
    return cross_section->len;
}
// --------------------------------------------------------------------------------

size_t xsec_alloc(const xsec_t* cross_section) {
    if (!cross_section || !cross_section->xs || !cross_section->energy) {
        errno = EINVAL;
        fprintf(stderr, "Invalid cross section passed to xsec_alloc\n");
        return 0;
    }
    return cross_section->alloc;
}
// --------------------------------------------------------------------------------

void free_xsec(xsec_t* cross_section) {
    if (!cross_section) {
        errno = EINVAL;
        fprintf(stderr, "Cross section NULL, possible double free\n");
    }
    if (cross_section->xs) { 
        free(cross_section->xs);
        cross_section->xs = NULL;
    }
    if (cross_section->energy) {
        free(cross_section->energy);
        cross_section->energy = NULL;
    }
    free(cross_section);
    cross_section = NULL;
}
// --------------------------------------------------------------------------------

void _free_xsec(xsec_t** cross_section) {
    if (cross_section && *cross_section) {
        free_xsec(*cross_section);
    }
}
// ================================================================================
// ================================================================================ 
// STRING_T DATA TYPE 

struct string_t {
    char* str;
    size_t len;
    size_t alloc;
};
// --------------------------------------------------------------------------------

string_t* init_string(const char* str) {
    if (str == NULL) {
        errno = EINVAL;
        fprintf(stderr, "Null value passed to init_string with error: %s\n", strerror(errno));
        return NULL;
    }
    string_t* ptr = malloc(sizeof(string_t));
    if (ptr == NULL) {
        errno = ENOMEM;
        fprintf(stderr, "Failed string_t allocation with error: %s\n", strerror(errno));
        return NULL;
    }
    size_t len = strlen(str);
    char* ptr2 = malloc(len + 1);
    if (ptr2 == NULL) {
        errno = ENOMEM;
        fprintf(stderr, "Failed string allocation with error: %s\n", strerror(errno));
        free(ptr);
        return NULL;
    }
    strcpy(ptr2, str);
    ptr->str = ptr2;
    ptr->len = len;
    ptr->alloc = len + 1;
    return ptr;
}
// --------------------------------------------------------------------------------

void free_string(string_t* str) {
    if (!str) {
        errno = EINVAL;
        fprintf(stderr, "String NULL, possible double free\n");
        return;
    }
    if (str->str) {
        free(str->str);
        str->str = NULL;
    }
    str->len = 0;
    str->alloc = 0;
    if (str) {
        free(str);
        str = NULL;
    }
}
// --------------------------------------------------------------------------------

void _free_string(string_t** str) {
    if (str && *str) {
        free_string(*str);
        *str = NULL;
    }
}
// --------------------------------------------------------------------------------

const char* get_string(const string_t* str) {
    if (!str || !str->str) {
        errno = EINVAL;
        fprintf(stderr, "string_t struct or literal is NULL with error: %s\n", strerror(errno));
        return NULL;
    }
    return str->str;
}
// --------------------------------------------------------------------------------

size_t string_size(const string_t* str) {
    if (!str || !str->str) {
        errno = EINVAL;
        fprintf(stderr, "string_t struct or literal is NULL with error: %s\n", strerror(errno));
        return -1;
    }
    return str->len;
}
// --------------------------------------------------------------------------------

size_t string_alloc(const string_t* str) {
    if (!str || !str->str) {
        errno = EINVAL;
        fprintf(stderr, "string_t struct or literal is NULL with error: %s\n", strerror(errno));
        return -1;
    }
    return str->alloc;
}
// --------------------------------------------------------------------------------

bool string_string_concat(string_t* str1, const string_t* str2) {
    if (!str1 || !str2 || !str1->str || !str2->str) {
        errno = EINVAL;
        fprintf(stderr, "Invalid input: one or both strings are NULL with error: %s\n", strerror(errno));
        return false;
    }

    // Calculate the new required length
    size_t new_len = str1->len + str2->len;

    // Check if the current buffer can hold the concatenated string
    if (new_len + 1 > str1->alloc) { // +1 for the null terminator
        // Reallocate the buffer to accommodate the new string
        char* new_buffer = realloc(str1->str, new_len + 1); // +1 for the null terminator
        if (!new_buffer) {
            errno = ENOMEM;
            fprintf(stderr, "Failed to reallocate memory for concatenated string with error: %s\n", strerror(errno));
            return false;
        }
        str1->str = new_buffer;
        str1->alloc = new_len + 1;
    }

    // Append the second string to the first
    strcat(str1->str, str2->str);

    // Update the length of the first string
    str1->len = new_len;
    return true;
}
// --------------------------------------------------------------------------------

bool string_lit_concat(string_t* str1, const char* literal) {
    if (!str1 || !str1->str || !literal) {
        errno = EINVAL;
        fprintf(stderr, "Invalid input: string_t or literal is NULL with error: %s\n", strerror(errno));
        return false;
    }

    // Calculate the new required length
    size_t literal_len = strlen(literal);
    size_t new_len = str1->len + literal_len;

    // Check if the current buffer can hold the concatenated string
    if (new_len + 1 > str1->alloc) { // +1 for the null terminator
        // Reallocate the buffer to accommodate the new string
        char* new_buffer = realloc(str1->str, new_len + 1); // +1 for the null terminator
        if (!new_buffer) {
            errno = ENOMEM;
            fprintf(stderr, "Failed to reallocate memory for concatenated string with error: %s\n", strerror(errno));
            return false;
        }
        str1->str = new_buffer;
        str1->alloc = new_len + 1;
    }

    // Append the string literal to the first string
    strcat(str1->str, literal);

    // Update the length of the first string
    str1->len = new_len;

    return true; // Indicate success
}
// --------------------------------------------------------------------------------

int compare_strings_lit(const string_t* str_struct, const char* string) {
    if (!str_struct || !string || !str_struct->str) {
        errno = EINVAL;
        fprintf(stderr, "Null pointer provided to compare_strings_lit.\n");
        return INT_MIN; // Or another designated error value
    }

    size_t string_len = strlen(string);
    size_t min_len = (str_struct->len < string_len) ? str_struct->len : string_len;

    for (size_t i = 0; i < min_len; i++) {
        if (str_struct->str[i] != string[i]) {
            return (unsigned char)str_struct->str[i] - (unsigned char)string[i];
        }
    }
    return str_struct->len - string_len;
}
// --------------------------------------------------------------------------------

int compare_strings_string(const string_t* str_struct_one, string_t* str_struct_two) {
    if (!str_struct_one || !str_struct_two || !str_struct_one->str || !str_struct_two->str) {
        errno = EINVAL;
        fprintf(stderr, "Null pointer provided to compare_strings_str.\n");
        return INT_MIN; // Or another designated error value
    } 

    size_t string_len = str_struct_two->len;
    size_t min_len = (str_struct_one->len < string_len) ? str_struct_one->len : string_len;

    for (size_t i = 0; i < min_len; i++) {
        if (str_struct_one->str[i] != str_struct_two->str[i]) {
            return (unsigned char)str_struct_one->str[i] - (unsigned char)str_struct_two->str[i];
        }
    }
    return str_struct_one->len - string_len;
}
// --------------------------------------------------------------------------------

string_t* copy_string(string_t* str) {
    if (!str || !str->str) {
        errno = EINVAL;
        fprintf(stderr, "Invalid input: string_t struct or literal is NULL with error: %s\n", strerror(errno));
        return false;
    }
    string_t* new_str = init_string(get_string(str));
    if (new_str->alloc < str->alloc) 
        reserve_string(new_str, str->alloc);
    return new_str; 
}
// --------------------------------------------------------------------------------

bool reserve_string(string_t* str, size_t len) {
    if (!str || !str->str) {
        errno = EINVAL;
        fprintf(stderr, "Invalid input: string_t struct or literal is NULL with error: %s\n", strerror(errno));
        return false;
    }

    // Ensure the requested length is greater than the current allocation
    if (len <= str->alloc) {
        errno = EINVAL;
        fprintf(stderr, "Invalid operation: reserve_string cannot reduce memory allocation. Current alloc: %zu, requested: %zu\n", str->alloc, len);
        return false;
    }

    // Attempt to reallocate memory
    char* ptr = realloc(str->str, sizeof(char) * len);
    if (!ptr) {
        errno = ENOMEM;
        fprintf(stderr, "Failed to reallocate memory with error: %s\n", strerror(errno));
        return false;
    }

    // Update the string_t structure with the new allocation
    str->str = ptr;
    str->alloc = len;

    return true;
}
// ================================================================================
// ================================================================================

struct vector_t {
    float* data;
    size_t len;
    size_t alloc;
};
// --------------------------------------------------------------------------------

vector_t* init_vector(size_t len) {
    vector_t* ptr = malloc(sizeof(vector_t));
    if (!ptr) {
        errno = ENOMEM;
        fprintf(stderr, "Vector allocation failure with error: %s\n", strerror(errno));
        return NULL;
    }
    float* ptr2 = malloc(len * sizeof(float));
    if (!ptr2) {
        errno = ENOMEM;
        fprintf(stderr, "Float vector allocation failure with error: %s\n", strerror(errno));
        free(ptr);
        return NULL;
    }
    ptr->data = ptr2;
    ptr->len = 0;
    ptr->alloc = len;
    return ptr;
}
// --------------------------------------------------------------------------------

bool push_back_vector(vector_t* vec, float dat) {
    if (!vec || !vec->data) {
        errno = EINVAL;
        fprintf(stderr, "Null pointer to vector_t or float vector with error: %s\n", strerror(errno));
        return false;
    }
    // Check if reallocation is needed
    if (vec->alloc <= vec->len) {
        size_t new_alloc = vec->alloc == 0 ? 1 : vec->alloc;
        if (new_alloc < XSEC_THRESHOLD) {
            new_alloc *= 2;  // Exponential growth for smaller allocations
        } else {
            new_alloc += XSEC_FIXED_AMOUNT;  // Fixed growth beyond threshold
        }

        // Attempt to reallocate the cross-section array
        float* ptr = realloc(vec->data, new_alloc * sizeof(float));
        if (!ptr) {
            errno = ENOMEM;
            fprintf(stderr, "Failed to reallocate for push_xsec\n");
            return false;
        }

        // Update structure only after successful reallocation
        vec->data = ptr;
        vec->alloc = new_alloc;
    } 
    vec->data[vec->len] = dat;
    vec->len++;
    return true;
}
// --------------------------------------------------------------------------------

bool push_front_vector(vector_t* vec, float dat) {
    if (!vec || !vec->data) {
        errno = EINVAL;
        fprintf(stderr, "Null pointer to vector_t or float vector with error: %s\n", strerror(errno));
        return false;
    }
    // Check if reallocation is needed
    if (vec->alloc <= vec->len) {
        size_t new_alloc = vec->alloc == 0 ? 1 : vec->alloc;
        if (new_alloc < XSEC_THRESHOLD) {
            new_alloc *= 2;  // Exponential growth for smaller allocations
        } else {
            new_alloc += XSEC_FIXED_AMOUNT;  // Fixed growth beyond threshold
        }

        // Attempt to reallocate the cross-section array
        float* ptr = realloc(vec->data, new_alloc * sizeof(float));
        if (!ptr) {
            errno = ENOMEM;
            fprintf(stderr, "Failed to reallocate for push_xsec\n");
            return false;
        }

        // Update structure only after successful reallocation
        vec->data = ptr;
        vec->alloc = new_alloc;
    }
    // Shift existing elements to the right
    if (vec->len > 0) {
        memmove(vec->data + 1, vec->data, vec->len * sizeof(float));
    }
    // Insert the new element at the front
    vec->data[0] = dat;
    vec->len++;
    return true;
}
// --------------------------------------------------------------------------------

bool insert_vector(vector_t* vec, float dat, size_t index) {
    if (!vec || !vec->data) {
        errno = EINVAL;
        fprintf(stderr, "Null pointer to vector_t or float vector with error: %s\n", strerror(errno));
        return false;
    }

    // Ensure index is within valid range
    if (index > vec->len) {  // Allow inserting at the end (index == vec->len)
        errno = ERANGE;
        fprintf(stderr, "Index value of %ld is out of range for length %ld\n", index, vec->len);
        return false;
    }

    // Handle edge cases
    if (index == 0) {
        return push_front_vector(vec, dat);  // Insert at the front
    }
    if (index == vec->len) {
        return push_back_vector(vec, dat);  // Append at the end
    }

    // Check if reallocation is needed
    if (vec->alloc <= vec->len) {
        size_t new_alloc = vec->alloc == 0 ? 1 : vec->alloc;
        if (new_alloc < XSEC_THRESHOLD) {
            new_alloc *= 2;  // Exponential growth for smaller allocations
        } else {
            new_alloc += XSEC_FIXED_AMOUNT;  // Fixed growth beyond threshold
        }

        // Attempt to reallocate the vector's array
        float* new_data = realloc(vec->data, new_alloc * sizeof(float));
        if (!new_data) {
            errno = ENOMEM;
            fprintf(stderr, "Failed to reallocate for insert_vector with error: %s\n", strerror(errno));
            return false;
        }

        // Update structure only after successful reallocation
        vec->data = new_data;
        vec->alloc = new_alloc;
    }

    // Shift elements to the right to make space for the new element
    size_t len = vec->len - index;  // Number of elements to shift
    memmove(vec->data + index + 1, vec->data + index, len * sizeof(float));

    // Insert the new element
    vec->data[index] = dat;
    vec->len++;  // Update the length of the vector

    return true;
}
// --------------------------------------------------------------------------------

float pop_back_vector(vector_t* vec) {
    if (!vec || !vec->data) {
        errno = EINVAL;
        fprintf(stderr, "Null pointer to vector_t or float vector with error: %s\n", strerror(errno));
        return FLT_MIN;
    }
    if (vec->len == 0) {
        errno = EINVAL;
        fprintf(stderr, "Vector is empty, can not pop\n");
        return FLT_MIN;
    }
    float dat = vec->data[vec->len-1];
    vec->len--;
    return dat;
}
// --------------------------------------------------------------------------------

float pop_front_vector(vector_t* vec) {
    if (!vec || !vec->data) {
        errno = EINVAL;
        fprintf(stderr, "Null pointer to vector_t or float vector with error: %s\n", strerror(errno));
        return FLT_MIN;  // Return FLT_MIN to indicate error
    }

    if (vec->len == 0) {
        errno = EINVAL;
        fprintf(stderr, "Vector is empty, cannot pop\n");
        return FLT_MIN;  // Return FLT_MIN to indicate error
    }

    // Store the first element
    float dat = vec->data[0];

    // Shift elements to the left if there are more than one
    if (vec->len > 1) {
        memmove(vec->data, vec->data + 1, (vec->len - 1) * sizeof(float));
    }

    // Decrement the length of the vector
    vec->len--;

    return dat;  // Return the popped value
}
// --------------------------------------------------------------------------------

float pop_any_vector(vector_t* vec, size_t index) {
    if (!vec || !vec->data) {
        errno = EINVAL;
        fprintf(stderr, "Null pointer to vector_t or float vector with error: %s\n", strerror(errno));
        return FLT_MIN;  // Return FLT_MIN to indicate error
    }

    if (vec->len == 0) {
        errno = EINVAL;
        fprintf(stderr, "Vector is empty, cannot pop\n");
        return FLT_MIN;  // Return FLT_MIN to indicate error
    }

    if (index >= vec->len) {  // Ensure index is within bounds
        errno = ERANGE;
        fprintf(stderr, "Index %ld is out of range for vector length %ld\n", index, vec->len);
        return FLT_MIN;  // Return FLT_MIN to indicate error
    }

    // Handle special cases for front and back
    if (index == 0) {
        return pop_front_vector(vec);  // Remove the first element
    } else if (index == vec->len - 1) {
        return pop_back_vector(vec);  // Remove the last element
    }

    // Retrieve the element to be removed
    float dat = vec->data[index];

    // Shift elements to close the gap
    memmove(vec->data + index, vec->data + index + 1, (vec->len - index - 1) * sizeof(float));

    // Update vector length
    vec->len--;

    return dat;  // Return the removed element
}
// --------------------------------------------------------------------------------

float get_vector(vector_t* vec, size_t index) {
    if (!vec || !vec->data) {
        errno = EINVAL;
        fprintf(stderr, "Null pointer to vector_t or float vector with error: %s\n", strerror(errno));
        return FLT_MAX;
    }
    if (index >= vec->len) {
        errno = EINVAL;
        fprintf(stderr, "Index %zu out of bounds (len: %zu)\n", index, vec->len);
        return FLT_MAX;
    }
    return vec->data[index]; 
}
// --------------------------------------------------------------------------------

const size_t vector_size(vector_t* vec) {
    if (!vec || !vec->data) {
        errno = EINVAL;
        fprintf(stderr, "Null pointer to vector_t or float vector with error: %s\n", strerror(errno));
        return 0;
    }
    return vec->len;
}
// --------------------------------------------------------------------------------

const size_t vector_alloc(vector_t* vec) {
    if (!vec || !vec->data) {
        errno = EINVAL;
        fprintf(stderr, "Null pointer to vector_t or float vector with error: %s\n", strerror(errno));
        return 0;
    }
    return vec->alloc;
}
// --------------------------------------------------------------------------------

void free_vector(vector_t* vec) {
    if (!vec) {
        errno = EINVAL;
        fprintf(stderr, "Vector NULL, possible double free\n");
        return;
    }
    if (vec->data) {
        free(vec->data);
        vec->data = NULL;
    }
    vec->len = 0;
    vec->alloc = 0;
    if (vec) {
        free(vec);
        vec = NULL;
    }
}
// --------------------------------------------------------------------------------

void _free_vector(vector_t** vec) {
    if (vec && *vec) {
        free_vector(*vec);
    }
}
// --------------------------------------------------------------------------------

vector_t* copy_vector(vector_t* vec) {
    if (!vec || !vec->data) {
        errno = EINVAL;
        fprintf(stderr, "Null pointer to vector_t or float vector with error: %s\n", strerror(errno));
        return NULL;
    }
    vector_t* new_vec = init_vector(vec->alloc);
    if (!new_vec) return NULL;
    for (size_t i = 0; i < vec->len; i++) {
        push_back_vector(new_vec, get_vector(vec, i));
    }
    return new_vec;
}
// ================================================================================
// ================================================================================ 
// DICTIONARY IMPLEMENTATION

typedef struct dictNode {
    char* key;
    float value;
    struct dictNode* next;
} dictNode;
// --------------------------------------------------------------------------------

struct dict_t {
    dictNode* keyValues;
    size_t hash_size;
    size_t len;
    size_t alloc;
};
// --------------------------------------------------------------------------------

static size_t hash_function(const char* key) {
    size_t hash = 5381; // Initialize hash with a prime number
    int c;

    while ((c = *key++)) { // Iterate over each character in the key
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash; // Return the computed hash value
}
// --------------------------------------------------------------------------------

static bool resize_dict(dict_t* dict, size_t new_size) {
    // Allocate a new array of dictNode structs
    dictNode* new_table = malloc(new_size * sizeof(dictNode));
    if (!new_table) {
        perror("Failed to allocate memory for resized hash table");
        return false;
    }

    // Initialize the new table
    for (size_t i = 0; i < new_size; i++) {
        new_table[i].key = NULL;  // Empty bucket
        new_table[i].value = 0;  // Default value
        new_table[i].next = NULL;
    }

    // Rehash existing key-value pairs into the new table
    for (size_t i = 0; i < dict->alloc; i++) {
        dictNode* current = dict->keyValues[i].next; // Traverse the chain in the old table
        while (current) {
            dictNode* next = current->next; // Save the next node
            size_t new_index = hash_function(current->key) % new_size;

            // Insert into the new table
            current->next = new_table[new_index].next;
            new_table[new_index].next = current;

            current = next; // Move to the next node in the chain
        }
    }

    // Free the old table
    free(dict->keyValues);

    // Update the dictionary's fields
    dict->keyValues = new_table; // Assign the new array
    dict->alloc = new_size;      // Update allocated size

    return true;
}
// --------------------------------------------------------------------------------

dict_t* init_dict() {
    dict_t* hashPtr = malloc(sizeof(*hashPtr));
    if (!hashPtr) {
        errno = ENOMEM;
        fprintf(stderr, "Failure to allocate dict_t struct in init_dict()\n");
        return NULL;
    }
    dictNode* arrPtr = malloc(hashSize * sizeof(*arrPtr));
    if (!arrPtr) {
        errno = ENOMEM;
        fprintf(stderr, "Failure to allocate dictNode in init_dict()\n");
        free(hashPtr);
        return NULL;
    }

    // Initialize each index in the keyValues array with a designated head node
    for (size_t i = 0; i < hashSize; i++) {
        arrPtr[i].key = NULL; // Set the head node's key pointer to NULL
        arrPtr[i].next = NULL; // Set the head node's next pointer to NULL
        arrPtr[i].value = 0; // Initialize value
    }
    
    hashPtr->keyValues = arrPtr;
    hashPtr->hash_size = 0;
    hashPtr->len = 0;
    hashPtr->alloc = hashSize;
    return hashPtr;
}
// --------------------------------------------------------------------------------

bool insert_dict(dict_t* dict, char* key, float value) {
    // Check if resizing is needed
    if (dict->hash_size >= dict->alloc * LOAD_FACTOR_THRESHOLD) {
        size_t new_alloc = dict->alloc < XSEC_THRESHOLD ? dict->alloc * 2 : dict->alloc + XSEC_FIXED_AMOUNT;
        resize_dict(dict, new_alloc);
    } 
    
    size_t index = hash_function(key) % dict->alloc;

    // Check if the key already exists in the hash table
    dictNode* current = dict->keyValues[index].next;
    while (current) {
        if (strcmp(current->key, key) == 0) {
            // Key already exists, return control to the calling program
            errno = EINVAL;
            fprintf(stderr, "Key already exists in dictionary, exiting insert_dict()\n");
            return false;
        }
        current = current->next;
    }

    // Allocate memory for the key
    char* new_key = malloc((strlen(key) + 1) * sizeof(char));
    if (!new_key) {
        errno = ENOMEM;
        fprintf(stderr, "Failed to allocate string for dictionary key word, exiting insert_dict()\n");
        free(current->key);
        return false;
    }
    strcpy(new_key, key); // Copy the key to the dynamically allocated memory
    
    // Insert the new key-value pair into the hash table
    dictNode* new_node = malloc(sizeof(*new_node));
    if (!new_node) {
        // Handle memory allocation failure
        free(new_key); // Free the allocated key memory
        free(current->key);
        return false;
    }
    new_node->key = new_key;
    new_node->value = value;
    new_node->next = dict->keyValues[index].next;
    dict->keyValues[index].next = new_node;
    
    dict->hash_size++; // Increment the number of key-value pairs
    
    // Update the size if the index was previously empty
    if (dict->keyValues[index].next == new_node) {
        dict->len++;
    }
    return true;
}
// --------------------------------------------------------------------------------

float pop_dict(dict_t* dict, char* key) {
    size_t index = hash_function(key) % dict->alloc;

    // Traverse the linked list at the index
    dictNode* prev = &dict->keyValues[index];
    dictNode* current = prev->next;
    while (current) {
        if (strcmp(current->key, key) == 0) {
            // Key found, unlink the node from the linked list
            prev->next = current->next;
            
            // Retrieve the value associated with the key
            float value = current->value;

            // Free the memory allocated for the key and the node
            free(current->key);
            free(current);

            // Decrement the number of key-value pairs in the hash table
            dict->len--;

            // Return the value associated with the key
            return value;
        }
        prev = current;
        current = current->next;
    }

    return FLT_MAX;
}
// --------------------------------------------------------------------------------

const float get_dict_value(const dict_t* table, char* key) {
    size_t index = hash_function(key) % table->alloc;
    // Traverse the linked list at the index
    dictNode* current = table->keyValues[index].next;
    while (current) {
        if (strcmp(current->key, key) == 0) {
            // Key found, return the corresponding value
            return current->value;
        }
        current = current->next;
    }
    fprintf(stderr, "Key: '%s' does not exist in dictionary\n", key);
    return FLT_MAX; 
}
// --------------------------------------------------------------------------------

void free_dict(dict_t* dict) {
    for (size_t i = 0; i < dict->alloc; i++) {
        dictNode* current = dict->keyValues[i].next; // Start from the head of the list
        dictNode* next = NULL;
        while (current) {      
            next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
    }
    free(dict->keyValues); 
    free(dict); 
}
// --------------------------------------------------------------------------------

void _free_dict(dict_t **dict) {
    if (dict && *dict) {
        free_dict(*dict);
    }
}
// --------------------------------------------------------------------------------

bool update_dict(dict_t* dict, char* key, float value) {
    size_t index = hash_function(key) % dict->alloc;
    dictNode* current = dict->keyValues[index].next;
    while (current) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return true;
        }
        current = current->next;
    }
    fprintf(stderr, "Key '%s' does not exist in dictionary\n", key);
    // If key is not found, no action is taken
    return false;
}
// --------------------------------------------------------------------------------

const size_t dict_size(const dict_t* dict) {
    return dict->len;
}
// --------------------------------------------------------------------------------

const size_t dict_alloc(const dict_t* dict) {
    return dict->alloc;
}
// --------------------------------------------------------------------------------

const size_t dict_hash_size(const dict_t* dict) {
    return dict->hash_size;
}
// ================================================================================
// ================================================================================
// eof
