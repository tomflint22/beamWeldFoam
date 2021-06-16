epsilon1=max(min((Temperature-TSolidus)/(TLiquidus-TSolidus),scalar(1)),scalar(0));

cp=alpha1*cp1 + alpha2*cp2;
// cp=(alpha1*rho1*cp1 + alpha2*cp2*rho2)/rho;
//Tvap=alpha1*Tvap1 + alpha2*Tvap2;
//Mm=alpha1*Mm1 + alpha2*Mm2;
//LatentHeatVap=alpha1*LatentHeatVap1 + alpha2*LatentHeatVap2;
kappa=alpha1*kappa1 + alpha2*kappa2;
TSolidus=alpha1*Tsolidus1 + alpha2*Tsolidus2;
TLiquidus=alpha1*Tliquidus1 + alpha2*Tliquidus2;
LatentHeat=alpha1*LatentHeat1 + alpha2*LatentHeat2;
beta=alpha1*beta1 + alpha2*beta2;

rhok=(1.0-max(epsilon1*(beta)*(Temperature-TSolidus),0.0));
rhok.correctBoundaryConditions();




volVectorField gradepsilon1=fvc::grad(epsilon1);
gradepsilon1.correctBoundaryConditions();
// volScalarField testone=mag(fvc::grad(epsilon1));
// testone.correctBoundaryConditions();

//     forAll( mesh.C(), celli)
// {
// if(testone[celli]>1.0e-12){

// nneps1[celli]=gradepsilon1[celli]/testone[celli];//alphaairinterface[celli]*
// }
// else{
// nneps1[celli]=vector::zero;
// }
// }



nneps1=fvc::grad(epsilon1)/((mag(gradepsilon1) + deltaN));
nneps1.correctBoundaryConditions();

gh=epsilon1*(g & mesh.C());
ghf= fvc::interpolate(epsilon1)*(g & mesh.Cf());



