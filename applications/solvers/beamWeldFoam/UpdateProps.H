    cp=(epsilon1*(alpha1*cp1 + alpha2*cp2)) + ((1.0-epsilon1)*(alpha1*cp1solid + alpha2*cp2solid));
    kappa=(epsilon1*(alpha1*kappa1 + alpha2*kappa2)) + ((1.0-epsilon1)*(alpha1*kappa1solid + alpha2*kappa2solid));
    TSolidus=alpha1*Tsolidus1 + alpha2*Tsolidus2;
    TLiquidus=alpha1*Tliquidus1 + alpha2*Tliquidus2;
    LatentHeat=alpha1*LatentHeat1 + alpha2*LatentHeat2;
    beta=alpha1*beta1 + alpha2*beta2;

    rhok=(1.0-max(epsilon1*(beta)*(Temperature-TSolidus),0.0));
    rhok.correctBoundaryConditions();

    volVectorField gradepsilon1=fvc::grad(epsilon1);
    gradepsilon1.correctBoundaryConditions();

    nneps1=fvc::grad(epsilon1)/((mag(gradepsilon1) + deltaN));
    nneps1.correctBoundaryConditions();

    gh=epsilon1*(g & mesh.C());
    ghf= fvc::interpolate(epsilon1)*(g & mesh.Cf());
