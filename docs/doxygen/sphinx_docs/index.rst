.. Core Utilities documentation master file, created by
   sphinx-quickstart
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to CENDF!
=================
The `Evaluated Nuclear Data File (ENDF) <https://www.oecd-nea.org/dbdata/data/manual-endf/endf102.pdf>`_ 
system is the United States' primary 
repository for nuclear reaction data, containing comprehensive information 
about nuclear cross sections, decay data, and related nuclear physics 
parameters. It serves as a standardized format and database that is 
essential for nuclear science, engineering, and applications ranging 
from reactor design to medical physics.  The ENDF/A file system contains raw 
data collected from experiementation, and the ENDF/B file system contains 
reduced data as applicable for engineering calculations.  This code repository 
is developed to read data from the ENDF/B file system.

The `ENDF <https://www.oecd-nea.org/dbdata/data/manual-endf/endf102.pdf>`_ 
database includes detailed information about how neutrons, 
protons, and other particles interact with different atomic nuclei across a 
wide range of energies. This includes capture cross sections, fission 
cross sections, scattering data, and other nuclear reaction parameters. 
The data is carefully evaluated by nuclear physicists and experts who 
analyze experimental measurements, theoretical calculations, and models to 
provide the most reliable values and uncertainties.

The format of `ENDF <https://www.oecd-nea.org/dbdata/data/manual-endf/endf102.pdf>`_ 
files is highly structured and hierarchical, organized 
into different sections called "MF" (Master File) numbers that contain 
specific types of nuclear data. For instance, MF=3 contains cross section 
data, while MF=4 contains angular distribution data. This standardized 
structure allows for efficient data processing by various nuclear codes and 
applications, making it a cornerstone resource for the nuclear science 
community.  In addition the `ENDF <https://www.oecd-nea.org/dbdata/data/manual-endf/endf102.pdf>`_ file contains Material Types (MT), where
MT numbers such as 1 represent total cross section, MT = 2 represents 
elastic scattering cross sections, and so on.

The `ENDF <https://www.oecd-nea.org/dbdata/data/manual-endf/endf102.pdf>`_ system is maintained by the National Nuclear Data Center at 
Brookhaven National Laboratory, and it is part of an international effort 
to provide reliable nuclear data. The database is regularly updated as new 
experimental data becomes available and evaluation methods improve. Similar 
databases exist in other countries, such as JEFF in Europe and JENDL in Japan, 
and these various libraries often collaborate and share data to improve the 
overall quality of nuclear data available to the scientific community.

However, the `ENDF <https://www.oecd-nea.org/dbdata/data/manual-endf/endf102.pdf>`_ file structure can be failry complicated which may make 
it difficult to access data.  This software package is developed to simplify 
the process of accessing ENDF data, as well as atomic level data from the 
periodic table.  This allows a user to more easily extract data for plotting 
of direct use in engineering software as C code; however, this library is
developed with the appropriate headers to allow for it to be compiled as 
C++ code as well.

.. note:: This library is mainted with a C++ compiler; however, this code should be capable of compilation with any compiler from C11 on.

.. note:: Some aspects of this code are written specifically for gcc and clang compilers; however, the code will comile in other C compilers without some of the functionality inherent with gcc and clan compilation.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   Cross Section Data Type <XSec>
   Vector Data Type <Vector>
   Generic Macros <Macros>

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

Installation
============
This project is covered under a basic MIT license which allows anyone to use 
this code base or to contribute to it with the express permsission of the 
git project owner.

Use Code Base 
-------------
In order to download this repository from github, follow these instructions

#. Ensure you have ``.git`` installed on your computer

#. Ensure you have ``cmake`` installed on your computer.  This code-base requires 
   cmake version 3.31.2 or later versions.

#. Download this repository to your preferred directory with the following command;

   .. code-block:: bash 

      git clone https://github.com/Jon-Webb-79/cendf.git cendf 

#. Navigate to either the bash or zshell scripts directory depending on your 
   environment with one of the following commands.

   .. code-block:: bash 

      cd csalt/scripts/bash 
      cd csalt/scripts/zsh 

#. Build the code base with the following command for debug options

   .. code-block:: bash 

      # If using bash
      bash debug.sh  
      # If using zsh 
      zsh debug.zsh

#. Compile the code base with the following command for a final release

   .. code-block:: bash 

      # If using bash 
      bash release.sh 
      # If using zsh 
      zsh release.zsh

#. Navigate to ``cendf/cendf/build/type`` where type is build or release to run unit tests 

   .. code-block:: bash 

      valgrind ./unit_tests 
      
#. If all unit tests do not pass with no memory leaks, you may need to contact 
   the administrator of this git repository.  If they do pass, then you are set 
   to transform this code-base into a static or dynamic library, or just 
   copy the ``.c`` and ``.h`` files to your project.

Contribute to Code Base 
-----------------------
#. Establish a pull request with the git repository owner.

#. Once the package has been downloade, you will also need to install
   Python3.10 or later version to support documentation with Sphinx.

#. Navigate to the ``cendf/docs/doxygen`` directory.

#. Create a Python virtual environment with the following command.

   .. code-block:: bash 

      python -m venv .venv 

#. Activate the virtual environment with the following command.

.. table:: Activation Commands for Virtual Environments

   +----------------------+------------------+-------------------------------------------+
   | Platform             | Shell            | Command to activate virtual environment   |
   +======================+==================+===========================================+
   | POSIX                | bash/zsh         | ``$ source <venv>/bin/activate``          |
   +                      +------------------+-------------------------------------------+
   |                      | fish             | ``$ source <venv>/bin/activate.fish``     |
   +                      +------------------+-------------------------------------------+
   |                      | csh/tcsh         | ``$ source <venv>/bin/activate.csh``      |
   +                      +------------------+-------------------------------------------+
   |                      | Powershell       | ``$ <venv>/bin/Activate.ps1``             |
   +----------------------+------------------+-------------------------------------------+
   | Windows              | cmd.exe          | ``C:\> <venv>\\Scripts\\activate.bat``    |
   +                      +------------------+-------------------------------------------+
   |                      | PowerShell       | ``PS C:\\> <venv>\\Scripts\\Activate.ps1``|
   +----------------------+------------------+-------------------------------------------+

#. Install packages to virtual environments from ``requirements.txt`` file

   .. code-block:: bash 

      pip install -r requirements.txt

#. At this point you can build the files in the same way described in the 
   previous section and contribute to documentation.


Contributing
============
Pull requests are welcome.  For major changes, please open an issue first to discuss what
you would like to change.  Please make sure to include and update tests as well
as relevant cod-strings and sphinx updates.

License
=======
This project uses a basic MIT license
