UntitledFoam - Work in Progress for publication in SoftwareX

Intention:
This solver will be Toms solver, FusionFoam, with the addition of the phenomenalogical recoil pressure and evaportive cooling terms from Gowthamans solver.

There are no manufacturing solvers out there with the Marangoni term and recoil pressure terms implemented. Also we have a really versatile implementation of the heat source here.

Plan:

I've added the recoil term to the mommentum equation, Gowthaman is going to add the evaporative cooling term to the energy equation. Then we should be able to write a short SoftwareX papaer and get some academic credit. I think I'll generate a zenodo DOI before we submit to SoftwareX too to give an alternate reference before it's published
