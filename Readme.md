## beamWeldFoam

## Overview
Presented here is the extensible open-source volume-of-fluid (VOF) solver beamWeldFoam, for studying high energy density advanced manufacturing processes. In this implementation the metallic substrate, and shielding gas phase, are treated as in-compressible. The solver fully captures the fusion/melting state transition of the metallic substrate. For the vapourisation of the substrate, the explicit volumetric dilation due to the vapourisation state transition is neglected, instead, a phenomenological recoil pressure term is used to capture the contribution to the momentum and energy fields due to vaporisation events. beamWeldFoam also captures surface tension effects, the temperature dependence of surface tension (Marangoni) effects, latent heat effects due to melting/fusion (and vapourisation), buoyancy effects due to the thermal expansion of the phases using a Boussinesq approximation, momentum damping due to solidification, and a representative heat source description of an incident laser/electron beam heat source. The heat source can also be modified to be representative of arc-welding processes.
The solver approach is based on the adiabatic two-phase interFoam code developed by [OpenCFD Ltd.](http://openfoam.com/). Target applications for beamWeldFoam include:

* Laer Welding
* Electron Beam Welding
* Arc Welding
* Additive Manufacturing

## Installation

The current version of the code utilises the [OpenFoam6 libraries](https://openfoam.org/version/6/). The code has been developed and tested using an Ubuntu installation, but should work on any operating system capable of installing OpenFoam. To install the beamWeldFoam solver, first follow the instructions on this page: [OpenFoam 6 Install](https://openfoam.org/download/6-ubuntu/) to install the OpenFoam 6 libraries.



Then navigate to a working folder in a shell terminal, clone the git code repository, and build.

```
$ git clone https://github.com/tomflint22/beamWeldFoam.git beamWeldFoam
$ cd beamWeldFoam/applications/solvers/beamWeldFoam/
$ wclean
$ wmake
```
The installation can be tested using the tutorial cases described below.

## Tutorial cases
### Gallium Melting Case

### Marangoni Flow (Sen and Davies) Case

### Arc Welding Case

### Beam Welding Case

## License
OpenFoam, and by extension the beamWeldFoam application, is licensed free and open source only under the [GNU General Public Licence version 3](https://www.gnu.org/licenses/gpl-3.0.en.html). One reason for OpenFOAM’s popularity is that its users are granted the freedom to modify and redistribute the software and have a right of continued free use, within the terms of the GPL.

## Acknowledgements
The work was generously supported by the Engineering and Physical Sciences Research Council (EPSRC) under the ``Cobalt-free Hard-facing for Reactor Systems'' grant EP/T016728/1.

![visitors](https://visitor-badge.glitch.me/badge?page_id=https://github.com/tomflint22/beamWeldFoam)


