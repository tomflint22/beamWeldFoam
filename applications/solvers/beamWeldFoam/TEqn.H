
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
dimensionedScalar pi = constant::mathematical::pi;

dimensionedScalar a_cond("a_cond",dimensionSet(0, 1, 0, 0, 0),HS_a);//
// dimensionedScalar c_cond("c_cond",dimensionSet(0, 1, 0, 0, 0),HS_c);//
dimensionedScalar b_g("b_g",dimensionSet(0, 1, 0, 0, 0),HS_bg);//
dimensionedScalar v_arc("v_arc",dimensionSet(0, 1, -1, 0, 0),HS_velocity);//
dimensionedScalar Q_cond("Q_cond",dimensionSet(1, 2, -3, 0, 0),HS_Q);//
dimensionedScalar lg("lg",dimensionSet(0, 1, 0, 0, 0),HS_lg);//
dimensionedScalar time("time",dimensionSet(0, 0, 1, 0, 0),runTime.time().value());//

dimensionedScalar Osc_Amp("Osc_Amp",dimensionSet(0, 1, 0, 0, 0),Oscillation_Amplitude);//
dimensionedScalar Osc_freq("Osc_freq",dimensionSet(0, 0, -1, 0, 0),Oscillation_Frequency);//

////////    Heat Source Selection   ////////

//  #include "newHS.H"
#include "DivergingOscillatingGaussian.H"
int iter = 0;

dimensionedScalar residual("residual",dimensionSet(0, 0, 0, 0, 0),scalar(1.0));
dimensionedScalar meanResidual("meanResidual",dimensionSet(0, 0, 0, 0, 0),scalar(1.0));
dimensionedScalar Tresidual("Tresidual",dimensionSet(0, 0, 0, 0, 0),scalar(1.0));

TRHS*=0.0;

volScalarField rhoCp = rho*cp;
rhoCp.correctBoundaryConditions();
surfaceScalarField kByCpf = fvc::interpolate(kappa/rhoCp);
surfaceScalarField phiCp = fvc::interpolate(cp)*phi;
surfaceScalarField rhoPhiCp=fvc::interpolate(cp)*rhoPhi;
surfaceScalarField kByCp = fvc::interpolate(kappa/cp);

surfaceScalarField rhophicp(fvc::interpolate(cp)*rhoPhi);

surfaceScalarField rhoPhiL=fvc::interpolate(LatentHeat)*rhoPhi;
do{
    iter++;
    epsilon1.storePrevIter();
    Temperature.storePrevIter();
    TRHS = LatentHeat*((fvc::ddt(rho,epsilon1) + fvc::div(rhoPhi, epsilon1)));
    TRHS.correctBoundaryConditions();

    volTensorField gradU = fvc::grad(U);
    volTensorField tau = ((mixture.mu()*gradU) + (mixture.mu()*gradU.T()))-((2.0/3.0)*mixture.mu()*(fvc::div(phi))*I);

    Qv = min(max((Temperature-(Tvap-(TSmooth/2.0)))/TSmooth,0.0),1.0)*0.82*LatentHeatVap*Mm*p0*Foam::exp(LatentHeatVap*Mm*((Temperature-Tvap)/(R*Temperature*Tvap)))/Foam::pow(2*M_PI*Mm*R*Temperature,0.5);
    Qv.correctBoundaryConditions();

    if(damperSwitch)
    {
        thermalDamper = 2.0*(rhoCp)/(rho1*cp1 + rho2*cp2);
    }
   
    ViscousDissipation=(tau && gradU);

    fvScalarMatrix  TEqn
    (
        fvm::ddt(rhoCp,Temperature)
        + fvm::div(rhophicp, Temperature)
        - fvm::Sp(fvc::ddt(rhoCp) + fvc::div(rhophicp), Temperature)
        - fvm::laplacian(kappa, Temperature)
        -(tau && gradU) //Viscous dissipation
        ==
        fvOptions(rhoCp,Temperature) + (sourceTerm) - Qv*mag(gradAlpha)*thermalDamper - (TRHS)
    );

    TEqn.relax();
    fvOptions.constrain(TEqn);

    TEqn.solve();

    fvOptions.correct(Temperature);

    Tcorr=(TLiquidus-TSolidus)*epsilon1+TSolidus;
    Tcorr.correctBoundaryConditions();

    epsilon1=max(min(epsilon1+(epsilonRel*cp/LatentHeat)*(Temperature-Tcorr),scalar(1)),scalar(0));
    epsilon1.correctBoundaryConditions();

    residual = gMax(mag(epsilon1.primitiveField()-epsilon1.prevIter().primitiveField()));

    meanResidual = gSum(mag(epsilon1.primitiveField()-epsilon1.prevIter().primitiveField())*mesh.V())/gSum(mesh.V().field());


    Info << "Correcting epsilon1, mean residual = " << meanResidual.value()
         << ", max residual = " << residual.value()
	     << endl;

    ddte1=fvc::ddt(epsilon1);
    ddte1.correctBoundaryConditions();
}while((iter < minTCorr || (residual > epsilonTol) ) && iter <= maxTCorr );

Temperature.correctBoundaryConditions();
gradT=fvc::grad(Temperature);
gradT.correctBoundaryConditions();
}




