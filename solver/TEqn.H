
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



//#include "UpdateVapourNormalVector.H"




////////    Heat Source Selection   ////////

//  #include "newHS.H"
 #include "DivergingOscillatingGaussian.H"





    int iter = 0;

    dimensionedScalar residual("residual",dimensionSet(0, 0, 0, 0, 0),scalar(1.0));
    dimensionedScalar meanResidual("meanResidual",dimensionSet(0, 0, 0, 0, 0),scalar(1.0));
    dimensionedScalar residual2("residual2",dimensionSet(0, 0, 0, 0, 0),scalar(1.0));
    dimensionedScalar meanResidual2("meanResidual2",dimensionSet(0, 0, 0, 0, 0),scalar(1.0));

//
//
// cp.storePrevIter();





TRHS*=0.0;//=TRHSzero;


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
Le1 = epsilon1*LatentHeat;



TRHS=          ((
    fvc::ddt(rho,Le1)
              + fvc::div(rhoPhi, Le1)
              )


)
// -((epsilon1)*mDot*(LatentHeatGas))
                ;
TRHS.correctBoundaryConditions();





// tensor Ei(1,0,0,0,1,0,0,0,1);
volTensorField gradU = fvc::grad(U);
volTensorField tau = ((mixture.mu()*gradU) + (mixture.mu()*gradU.T()))-((2.0/3.0)*mixture.mu()*(fvc::div(phi))*I);



ViscousDissipation=(tau && gradU);

 fvScalarMatrix  TEqn
(
        fvm::ddt(rhoCp,Temperature)
      + fvm::div(rhophicp, Temperature)
      - fvm::Sp(fvc::ddt(rhoCp) + fvc::div(rhophicp), Temperature)
      - fvm::laplacian(kappa, Temperature)
       -(tau && gradU) //Viscous dissipation
    //   +fvc::ddt(rho, K) + fvc::div(mixture.rhoPhi(), K)
// + (
//             // fvc::div(fvc::absolute(phi, U), p)

//         )
     ==
 fvOptions(rhoCp,Temperature)
             +
 (sourceTerm)//epsilon1
-(TRHS)

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

}while((iter < minTCorr || (residual > epsilonTol)) && iter <= maxTCorr );


// #include "UpdateProps.H"



Temperature.correctBoundaryConditions();

gradT=fvc::grad(Temperature);
gradT.correctBoundaryConditions();

}





