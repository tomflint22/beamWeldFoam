IOdictionary solutionDict
(
    IOobject
    (
        "fvSolution",
        runTime.system(), //folder where the fvSolution file is located
        runTime,
        IOobject::MUST_READ
    )
);



dictionary& meltingDict = solutionDict.subDict("MELTING");

int minTCorr(readLabel(meltingDict.lookup("minTempCorrector")));
int maxTCorr(readLabel(meltingDict.lookup("maxTempCorrector")));

dimensionedScalar epsilonTol(readScalar(meltingDict.lookup("epsilonTolerance")));
dimensionedScalar epsilonRel(readScalar(meltingDict.lookup("epsilonRelaxation")));

scalar HS_a(readScalar(meltingDict.lookup("HS_a")));
    scalar HS_bg(readScalar(meltingDict.lookup("HS_bg")));
    scalar HS_velocity(readScalar(meltingDict.lookup("HS_velocity")));
    scalar HS_lg(readScalar(meltingDict.lookup("HS_lg")));
    scalar HS_Q(readScalar(meltingDict.lookup("HS_Q")));

    scalar HS_deposition_cutoff(readScalar(meltingDict.lookup("HS_deposition_cutoff")));
    scalar Oscillation_Amplitude(readScalar(meltingDict.lookup("Oscillation_Amplitude")));
    scalar Oscillation_Frequency(readScalar(meltingDict.lookup("Oscillation_Frequency")));



bool damperSwitch = meltingDict.lookupOrDefault<bool>("damperSwitch", false);

//needed for cases with spherical particles or spurious currents will smear out the particle interface un-physically
    const Switch PowderSim(meltingDict.lookupOrDefault<Switch>("PowderSim", false));
