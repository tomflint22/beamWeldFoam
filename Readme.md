## beamWeldFoam

## Overview
Presented here is the extensible open-source volume-of-fluid (VOF) solver beamWeldFoam, for studying high energy density advanced manufacturing processes. In this implementation the metallic substrate, and shielding gas phase, are treated as in-compressible. The solver fully captures the fusion/melting state transition of the metallic substrate. For the vapourisation of the substrate, the explicit volumetric dilation due to the vapourisation state transition is neglected, instead, a phenomenological recoil pressure term is used to capture the contribution to the momentum and energy fields due to vaporisation events. beamWeldFoam also captures surface tension effects, the temperature dependence of surface tension (Marangoni) effects, latent heat effects due to melting/fusion (and vapourisation), buoyancy effects due to the thermal expansion of the phases using a Boussinesq approximation, momentum damping due to solidification, and a representative heat source description of an incident laser/electron beam heat source. The heat source can also be modified to be representative of arc-welding processes.
The solver approach is based on the adiabatic two-phase interFoam code developed by [OpenCFD Ltd.](http://openfoam.com/). Target applications for beamWeldFoam include:

* Laser Welding
* Electron Beam Welding
* Arc Welding
* Additive Manufacturing

## Installation

The current version of the code utilises the [OpenFoam6 libraries](https://openfoam.org/version/6/) or the [OpenFoam10 libraries](https://openfoam.org/version/10/). The code has been developed and tested using an Ubuntu installation, but should work on any operating system capable of installing OpenFoam. To install the beamWeldFoam solver, first follow the instructions on this page: [OpenFoam 6 Install](https://openfoam.org/download/6-ubuntu/) to install the OpenFoam 6 (or 10) libraries.

All the cases in the paper were run using the OpenFoam6 version of the code. Not all cases have been converted to the OpenFoam10 style.

Then navigate to a working folder in a shell terminal, clone the git code repository, and build.

```
$ git clone https://github.com/tomflint22/beamWeldFoam.git beamWeldFoam
$ cd beamWeldFoam/applications/solvers/beamWeldFoam/
$ wclean
$ wmake
```
The installation can be tested using the tutorial cases described below.

## Tutorial cases
To run any of the tutorials in serial mode:
```
delete any old simulation files, e.g:
$ rm -r 0* 1* 2* 3* 4* 5* 6* 7* 8* 9*
Then:
$ cp -r initial 0
$ blockMesh
$ setFields
$ beamWeldFoam
```
For parallel deployment, using MPI, following the setFields command:
```
$ decomposePar
$ mpirun -np 6 beamWeldFoam -parallel >log &
```
for deployment on 6 cores.

### Gallium Melting Case
A commonly used validation case for heat and mass transfer where melting and solidification is involved, is the simulation of Gallium melting in an enclosed container. In this example the beamWeldFoam solver is used to simulate the melting of the Gallium and the subsequent flow due to buoyancy. as time progresses the hot wall on the left-hand-side of the computational domain causes the Gallium in the local vicinity to melt. As the melt volume increases, buoyancy driven flow begings to dominate as the hot liquid Gallium rises and generates vortical flow structures in the liquid. The predicted melt profiles are in excellent agreement with those reported elsewhere, both numerically and experimentally [1].

### Marangoni Flow (Sen and Davies) Case
Another useful validation case for the solver is one in which a 2D cavity is partially filled such that the interface between the phases is initially flat. A temperature gradient is then developed across the domain. This temperature gradient induces a flow tangential to the interface due to the dependence on temperature of the surface tension, aka Marangoni flow. An analytical steasy-state solution for the free surface deformation exists for this case [2]. excellent agreement between the beamWeldFoam solver and the analytical solution is observed.

### Arc Welding Case
In this example a surface heat flux is applied to an Aluminium substrate representative of an arc-welding process. In this scenario, a metallic substrate is present in the domain, between two regions of Argon gas. The heat source is applied at t=0s, and at t=0.25s the power begins to ramp down until at t=0.35s the heat source is fully extinguished.Shortly following the extinction of the heat source the domain fully solidifies. The effect of Marangoni driven flow can clearly be seen in this example, as the surface flows are driven from regions of higher temperature to regions of lower temperature (due to the decrease in surface tension with temperature). Furthermore, once the weld-pool has fully penetrated the domain, surface tension prevents the material from falling out of the bottom of the substrate.

### Beam Welding Case
In this example beamWeldFoam is applied to simulate the power beam welding of a titanium alloy substrate. In this case, Ti6Al4V butt joints welded by a laser beam is simulated and the results are validated with the experimental study [3].

## Algorithm

Initially the solver loads the mesh, reads in fields and boundary conditions, reads certain mesh information into arrays (for the heat source application), selects the turbulence model (if specified). The main solver loop is then initiated. First, the time step is
dynamically modified to ensure numerical stability. Next, the two-phase fluid mixture properties and turbulence quantities are updated. The discretized phase-fraction equation is then solved for a user-defined number of subtime steps (typically 3) using the multidimensional universal limiter with explicit solution solver [MULES](https://openfoam.org/release/2-3-0/multiphase/). This solver is included in the OpenFOAM library, and performs conservative solution of hyperbolic convective transport equations with defined bounds (0 and 1 for α1). Once the updated phase field is obtained, the program enters the pressure–velocity loop, in which p and u are corrected in an alternating fashion. In this loop T is also solved for, such that he buoyancy predictions are correct for the U and p fields. The process of correcting the pressure and velocity fields in sequence is known as pressure implicit with splitting of operators (PISO). In the OpenFOAM environment, PISO is repeated for multiple iterations at each time step. This process is referred to as merged PISO- semi-implicit method for pressure-linked equations (SIMPLE), or the pressure-velocity loop (PIMPLE) process, where SIMPLE is an iterative pressure–velocity solution algorithm. PIMPLE continues for a user specified number of iterations. 
The main solver loop iterates until program termination. A summary of the simulation algorithm is presented below:
* beamWeldFoam Simulation Algorithm Summary:
  * Initialize simulation data and mesh 
  * WHILE t<t_end DO
  * 1. Update delta_t for stability
  * 2. Phase equation sub-cycle
  * 3. Update interface location for heat source application
  * 4. Update fluid properties
  * 5. PISO Loop
    * 1. Form u equation
    * 2. Energy Transport Loop
      * 1. Solve T equation
      * 2. Update fluid fraction field
      * 3. Re-evaluate source terms due to latent heat
    * 3. PISO
        * 1. Obtain and correct face fluxes
        * 2. Solve p-Poisson equation
        * 3. Correct u
  * 6. Write Fields
  
Two sample tutorial cases, i.e. Gallium Meliing, and Sen and Davies cases are in strong agreement with experimental and analytical data available in the literature and serve as the validation cases for the implementation in beamWeldFoam.

## License
OpenFoam, and by extension the beamWeldFoam application, is licensed free and open source only under the [GNU General Public Licence version 3](https://www.gnu.org/licenses/gpl-3.0.en.html). One reason for OpenFOAM’s popularity is that its users are granted the freedom to modify and redistribute the software and have a right of continued free use, within the terms of the GPL.

## Acknowledgements
The work was generously supported by the Engineering and Physical Sciences Research Council (EPSRC) under the ''Cobalt-free Hard-facing for Reactor Systems'' grant EP/T016728/1, and Science Foundation Ireland (SFI), co-funded under European Regional Development Fund and by I-Form industry partners, grant 16/RC/3872.

## Citing This Work
If you use beamWeldFoam in your work. Please use the following to cite our work:

Thomas F. Flint, Gowthaman Parivendhan, Alojz Ivankovic, Michael C. Smith, Philip Cardiff,
beamWeldFoam: Numerical simulation of high energy density fusion and vapourisation-inducing processes,
SoftwareX,
Volume 18,
2022,
101065,
ISSN 2352-7110,
https://doi.org/10.1016/j.softx.2022.101065

## References
* Kay Wittig and Petr A Nikrityuk 2012 IOP Conf. Ser.: Mater. Sci. Eng. 27 012054
* Sen, A., & Davis, S. (1982). Steady thermocapillary flows in two-dimensional slots. Journal of Fluid Mechanics, 121, 163-186. doi:10.1017/S0022112082001840
* Sabina L. Campanelli, Giuseppe Casalino, Michelangelo Mortello, Andrea Angelastro, Antonio Domenico Ludovico, Microstructural Characteristics and Mechanical Properties of Ti6Al4V Alloy Fiber Laser Welds


![visitors](https://visitor-badge.deta.dev/badge?page_id=tomflint22.beamWeldFoam)


