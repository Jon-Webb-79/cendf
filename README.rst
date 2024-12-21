*****
cendf
*****

The `Evaluated Nuclear Data File (ENDF)<https://www.nndc.bnl.gov/endf/>`_ 
is a library created and maintained by the Los Alamos National Laboratory 
(LANL) that contains a repository of all measured atomic and nuclear cross 
sections that are necessary for the calcuation of various nuclear reactions.  
The ENDF library was written to interact with a FORTAN capatable tape file, 
and implements a fairly complex data lookup method.  This library provide a 
C and C++ interface to the ENDF library that greatly simplifies the data lookup 
process.

Overview
########

The functionality for this library is still in development

Development Status
##################

This repository is currently under development.

Contributing
############

This project is archived for internal purposes. However, if you wish to 
contribute, please contact the author to request authorization for a 
pull request.

License
#######

This software is developed under the MIT license.

Requirements
############

This library is developed and tested on macOS and Arch Linux operating systems. 
It requires the following tools and versions:

- `g++ 14.1.1` or `clang 16.0.6` compiler
- `CMake 3.31.2` or later
- `C11` or later.
- `valgrind` for memory checking

Note: While the code should be compliant with `C11` or later, it is 
currently being compiled with `C17`.

Installation
############

This project is distributed under the MIT license, allowing anyone to use 
or contribute to the codebase with the express permission of the repository 
owner.

To use this repository, follow these steps:

1. Ensure that `git` and `cmake` are installed on your computer. The required 
   CMake version is 3.31.2 or later.

2. Clone the repository to your preferred directory with the following command:

   .. code-block:: bash

      git clone https://github.com/Jon-Webb-79/cendf.git cendf

3. Navigate to the appropriate script directory depending on your environment 
   (bash or zsh):

   .. code-block:: bash

      # For bash
      cd cendf/scripts/bash 
      
      # For zsh
      cd cendf/scripts/zsh 

4. Build the codebase with the following command:

   .. code-block:: bash

      # For bash
      bash debug.sh  

      # For zsh
      zsh debug.zsh

5. Navigate to the `cendf/build/release` directory to run the debug 
   version of the application:

   .. code-block:: bash

      ./UnitTest

6. If all unit tests pass and no memory leaks are detected, you may proceed 
   to transform this codebase into a static or dynamic library, or copy the 
   `.c` and `.h` files to your own project.

7. If the debug build works correctly, create a release version by running 
   the following commands from the `scripts` directory:

   .. code-block:: bash

      zsh release.zsh
      ./UnitTest

