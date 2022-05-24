    MRF.correctBoundaryVelocity(U);

    const dimensionedScalar DarcyConstantlarge
    (
        "DarcyConstantlarge", dimensionSet(1,-3,-1,0,0), scalar(1.0e6)
    ); //1e10
    const dimensionedScalar DarcyConstantsmall
    (
        "DarcyConstantsmall",
        dimensionSet(0,0,0,0,0),
        scalar(1.0e-12)
    ); //1e-6

    const dimensionedScalar TSmooth("TSmooth",dimensionSet(0, 0, 0, 1, 0),50.0);

    DC =
        (
            DarcyConstantlarge*Foam::pow((1.0 - epsilon1mask), 2)
           /(Foam::pow(epsilon1mask, 3) + DarcyConstantsmall)
        );

    const volVectorField gradAlpha(fvc::grad(alpha1));

    const volVectorField nHatM(gradAlpha/(mag(gradAlpha) + deltaN));
    Marangoni = Marangoni_Constant*(gradT-nHatM*(nHatM & gradT))*mag(gradAlpha);

    if(damperSwitch)
    {
        damper = 2.0 * rho /(rho1+rho2); //To account for high density difference
    }

    pVap = min
    (
        max((Temperature - (Tvap - (TSmooth/2.0)))/TSmooth, 0.0),
        1.0
    )*0.54*p0*Foam::exp
    (
        LatentHeatVap*Mm*((Temperature-Tvap)/(R*Temperature*Tvap))
    );

    fvVectorMatrix UEqn
    (
        fvm::ddt(rho, U) + fvm::div(/*fvc::interpolate(epsilon1mask)*/rhoPhi, U)
      + MRF.DDt(rho, U)
      + turbulence->divDevRhoReff(rho, U)
      + fvm::Sp(DC, U)
      - Marangoni*damper
     ==
        fvOptions(rho, U)
    );

    UEqn.relax();

    fvOptions.constrain(UEqn);

    if (pimple.momentumPredictor())
    {
        solve
        (
            UEqn
         ==
            fvc::reconstruct
            (
                (
                    mixture.surfaceTensionForce()*fvc::interpolate(damper)
                  + fvc::interpolate(pVap)*fvc::snGrad(alpha1)
                   *fvc::interpolate(damper)
                  - ghf*fvc::snGrad(rho*rhok)
                  - fvc::snGrad(p_rgh)
                )*mesh.magSf()
            )
        );

        fvOptions.correct(U);
    }
