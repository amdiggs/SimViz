# SimViz
This project is an open source simulation visualization and data analysis tool for atomic simulations.
The project currently supports output files from LAMMPS, Quantum Espresso, VASP, and ASE.
There is other atomic simulation visualiation software available that is freely availble and much more refined.
However, this project was started when other projects failed to meet the needs of our current project, and were not available as source.
The SimViz project provides the foundation to perform volumetric data analysis specific to your project.

Compute local volumetric properties such as mass density or stoichiometry, and then visualize the reslults with an iso-surface or a voxel heat map.
### Iso-surface
<p align="center">
    <img src="assets/ISO-DB.png" width="500"/>
</p>

### Voxel Heat Map
<p align="center">
    <img src="assets/voxel.png" width="500"/>
</p>

### Additional Features
    - Locate crystalline surfaces and find dangling bonds
    - Compute the dipole field of H2O solvated surfaces
    - Find and compute the volume of voids

### Dependencies
    - GLFW > 3.3
    - OpenCL 1.2
    - OpenGL > 3.3
    - CMake > 3.12

### Running Examples
Upon successfully building *simviz* you can test the executable with one of the example data files provides inthe examples directory. 

To run an example

    - Run the executable.
    - Click the **Open** button on top right pannel.
    - Navigate to the **examples** directory using the pop up window.
    - Select the Pinhole.dump file, then press the **Open** button on the pop up.
    - Next click on the **Draw** drop down on the right pannel.
    - You can now select one or more of the draw calls from the menu.


