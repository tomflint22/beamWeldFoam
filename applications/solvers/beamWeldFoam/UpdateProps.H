    cp=(epsilon1*(alpha1*cp1 + alpha2*cp2)) + ((1.0-epsilon1)*(alpha1*cp1solid + alpha2*cp2solid));
    kappa=(epsilon1*(alpha1*kappa1 + alpha2*kappa2)) + ((1.0-epsilon1)*(alpha1*kappa1solid + alpha2*kappa2solid));
    //TSolidus=alpha1*Tsolidus1 + alpha2*Tsolidus2;
    //TLiquidus=alpha1*Tliquidus1 + alpha2*Tliquidus2;
    LatentHeat=alpha1*LatentHeat1 + alpha2*LatentHeat2;
    beta=alpha1*beta1 + alpha2*beta2;

    rhok=(1.0-max(epsilon1*(beta)*(Temperature-TSolidus),0.0));
    rhok.correctBoundaryConditions();

    volVectorField gradepsilon1=fvc::grad(epsilon1);
    gradepsilon1.correctBoundaryConditions();

    nneps1=fvc::grad(epsilon1)/((mag(gradepsilon1) + deltaN));
    nneps1.correctBoundaryConditions();




forAll( mesh.C(), celli)
{
if(alpha1[celli]>1e-4){
TSolidus[celli]=Tsolidus1.value();
TLiquidus[celli]=Tliquidus1.value();
}
else{
TSolidus[celli]=alpha1[celli]*Tsolidus1.value() + alpha2[celli]*Tsolidus2.value();
TLiquidus[celli]=alpha1[celli]*Tliquidus1.value() + alpha2[celli]*Tliquidus2.value();
}
}

/*
forAll( mesh.C(), celli)
{
if(alpha1[celli]<0.99){
LatentHeat[celli]=LatentHeat2.value();

}
else{
LatentHeat[celli]=LatentHeat1.value();
}
}
*/

nneps1=fvc::grad(epsilon1)/((mag(gradepsilon1) + deltaN));
nneps1.correctBoundaryConditions();

epsilon1mask==1.0;
epsilon1mask=1.0-max(min((1.0-epsilon1)+mag(nneps1),scalar(1)),scalar(0));
epsilon1mask*=epsilon1;
epsilon1mask.correctBoundaryConditions();

forAll( mesh.C(), celli)
{
if(epsilon1[celli]<0.5){
const labelList& neighbour = mesh.cellCells()[celli];
forAll(neighbour,i)
{
epsilon1mask[neighbour[i]]=0.0;
}}}
epsilon1mask*=epsilon1;
epsilon1mask.correctBoundaryConditions();

forAll( mesh.C(), celli)
{
if(epsilon1mask[celli]>0.95){
epsilon1mask[celli]=1.0;
}
}
epsilon1mask.correctBoundaryConditions();


    gh=epsilon1*(g & mesh.C());
    ghf= fvc::interpolate(epsilon1)*(g & mesh.Cf());
