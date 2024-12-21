// ================================================================================
// ================================================================================
// - File:    rad.h
// - Purpose: Describe the file purpose here
//
// Source Metadata
// - Author:  Jonathan A. Webb
// - Date:    December 17, 2024
// - Version: 1.0
// - Copyright: Copyright 2024, Jon Webb Inc.
// ================================================================================
// ================================================================================
// Include modules here

#ifndef read_file_H
#define read_file_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
// ================================================================================ 
// ================================================================================ 

/**
 * Reads the atomic mass (second floating-point number) from the second line of 
 * a FORTRAN-delimited ENDF file.
 *
 * @param filename The path to the ENDF file to read.
 * @param neturon_mass The mass of a neutron in units of atomic mass units (amu)
 * @return The atomic mass as a float, or -1.0f if an error occurs.  The atomic 
 *          mass is in units of atomic mass units (amu)
 *
 * Possible errors:
 * - If the file cannot be opened, prints an error message with errno details and returns -1.0f.
 *   - Possible errno values:
 *     - EACCES: Permission denied.
 *     - ENOENT: No such file or directory.
 *     - ENAMETOOLONG: File name too long.
 *     - EMFILE: Too many open files by the process.
 *     - ENFILE: Too many open files in the system.
 * - If the first or second line cannot be read, prints an error message with errno details and returns -1.0f.
 *   - Possible errno value:
 *     - EIO: Input/output error.
 * - If the atomic mass cannot be parsed from the second line, prints an error message and returns -1.0f (no errno set for parsing errors).
 */
float read_amu(const char* fileName, const float neutron_mass);

// ================================================================================ 
// ================================================================================ 
#ifdef __cplusplus
}
#endif /* cplusplus */
#endif /* read_file_H */
// ================================================================================
// ================================================================================
// eof
