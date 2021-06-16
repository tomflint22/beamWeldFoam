{

    // if(Q_cond.value()>1e-12){

//RAY==0.0;
sourceTerm*=0.0;
//SET SOURCETERM TO ZERO

//          int N_RAYS=num_unique_x_coords*num_unique_z_coords;
////          std::vector<double> RAY_number(N_RAYS,1.0);//for numbering reflections, can be int later
//


    scalar Y_R(readScalar(meltingDict.lookup("Y_R")));
    scalar focY(readScalar(meltingDict.lookup("focY")));

    scalar foc_shift_vel(readScalar(meltingDict.lookup("foc_shift_vel")));
    scalar Q_ramp_rate(readScalar(meltingDict.lookup("Q_ramp_rate")));
    scalar tshift(readScalar(meltingDict.lookup("tshift")));

    // scalar HS_DIM(readScalar(meltingDict.lookup("HS_DIM")));


int Rayn=0;
for(int z_ray=0;z_ray<num_unique_z_coords;z_ray++){
for(int x_ray=0;x_ray<num_unique_x_coords;x_ray++){//start loop over all rays //for(int Rayn=0;Rayn<N_RAYS;Rayn++){//start loop over all rays
//intensity[Rayn]=0.0;
double miny=1.0e6;
for(int y_ray=0;y_ray<num_unique_y_coords;y_ray++){


point pnow(xlist[x_ray], ylist[y_ray], zlist[z_ray]);
label cellpoint = mesh.findCell(pnow);
//find location of smallest y coordinate

//&&alphametallic[cellpoint]>0.5

if(
alpha1[cellpoint]>HS_deposition_cutoff
){ //HS_deposition_cutoff 0.99 for conduction mode, 0.01 for keyhole mode
miny=std::min(miny,ylist[y_ray]);
}

}
//Info<<miny<<endl;

point phit(xlist[x_ray], miny, zlist[z_ray]);
label cellpointhit = mesh.findCell(phit);

/*
point phit2(xlist[x_ray], miny+yDim[cellpointhit], zlist[z_ray]);
label cellpointhit2 = mesh.findCell(phit2);

point phit3(xlist[x_ray], miny+2.0*yDim[cellpointhit2], zlist[z_ray]);
label cellpointhit3 = mesh.findCell(phit3);

point phit4(xlist[x_ray], miny+3.0*yDim[cellpointhit3], zlist[z_ray]);
label cellpointhit4 = mesh.findCell(phit4);
*/
// (a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focY))/Y_R),2.0)));


if(yDim[cellpointhit]>1e-12){

// double remain = condensate[cellpointhit2]*(1.0-condensate[cellpointhit]);
// double remain2 = condensate[cellpointhit3]*(1.0-remain);
// double remain3 = condensate[cellpointhit4]*(1.0-remain2);
// double remain4 = 1.0-condensate[cellpointhit4];


if(time.value()>tshift){
// Info<<"test"<<endl;

double focnow=(focY-(foc_shift_vel*(time.value()-tshift)));
double Qnow=max((Q_cond.value()-(Q_ramp_rate*(time.value()-tshift))),0.0);
// Info<<focnow<<endl;



sourceTerm[cellpointhit]=(1.0/yDim[cellpointhit])*((2.0*Qnow)/
((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focnow)/Y_R),2.0)))*(a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focnow)/Y_R),2.0)))*pi.value()))
*Foam::exp(-2.0*(
    Foam::pow(((xcoord[cellpointhit]-b_g.value()-(Osc_Amp.value()*Foam::cos(2.0*pi.value()*Osc_freq.value()*time.value())))/((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focnow)/Y_R),2.0))))),2.0)
    +Foam::pow((zcoord[cellpointhit]-(v_arc.value()*time.value())-lg.value()/*-(Osc_Amp.value()*Foam::sin(2.0*pi.value()*Osc_freq.value()*time.value()))*/)/((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focnow)/Y_R),2.0)))),2.0)));


// double TotalQ=gSum(sourceTerm)*yDim[5]*yDim[5]*yDim[5];


}

else{

    //         Info<<"\n \n \n \n"<<endl;
    // Info<<HS_DIM<<endl;
    //     Info<<"\n \n \n \n"<<endl;
sourceTerm[cellpointhit]=(1.0/yDim[cellpointhit])*((2.0*Q_cond.value())/
((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focY)/Y_R),2.0)))*(a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focY)/Y_R),2.0)))*pi.value()))
*Foam::exp(-2.0*(
    Foam::pow(((xcoord[cellpointhit]-b_g.value()-(Osc_Amp.value()*Foam::cos(2.0*pi.value()*Osc_freq.value()*time.value())))/((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focY)/Y_R),2.0))))),2.0)
    +Foam::pow((zcoord[cellpointhit]-(v_arc.value()*time.value())-lg.value()/*-(Osc_Amp.value()*Foam::sin(2.0*pi.value()*Osc_freq.value()*time.value()))*/)/((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focY)/Y_R),2.0)))),2.0)));


}
/*
sourceTerm[cellpointhit2]=(cellarea[cellpointhit2]/mesh.V()[cellpointhit2])*((2.0*Q_cond.value())/
((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focY)/Y_R),2.0)))*(a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focY)/Y_R),2.0)))*pi.value()))
*Foam::exp(-2.0*(
    Foam::pow(((xcoord[cellpointhit2]-b_g.value()-(Osc_Amp.value()*Foam::cos(2.0*pi.value()*Osc_freq.value()*time.value())))/((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focY)/Y_R),2.0))))),2.0)
    +Foam::pow((zcoord[cellpointhit2]-(v_arc.value()*time.value())-lg.value()-(Osc_Amp.value()*Foam::sin(2.0*pi.value()*Osc_freq.value()*time.value())))/((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focY)/Y_R),2.0)))),2.0)));


sourceTerm[cellpointhit3]=(cellarea[cellpointhit3]/mesh.V()[cellpointhit3])*((2.0*Q_cond.value())/
((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focY)/Y_R),2.0)))*(a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focY)/Y_R),2.0)))*pi.value()))
*Foam::exp(-2.0*(
    Foam::pow(((xcoord[cellpointhit3]-b_g.value()-(Osc_Amp.value()*Foam::cos(2.0*pi.value()*Osc_freq.value()*time.value())))/((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focY)/Y_R),2.0))))),2.0)
    +Foam::pow((zcoord[cellpointhit3]-(v_arc.value()*time.value())-lg.value()-(Osc_Amp.value()*Foam::sin(2.0*pi.value()*Osc_freq.value()*time.value())))/((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focY)/Y_R),2.0)))),2.0)));

sourceTerm[cellpointhit4]=(cellarea[cellpointhit4]/mesh.V()[cellpointhit4])*((2.0*Q_cond.value())/
((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focY)/Y_R),2.0)))*(a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focY)/Y_R),2.0)))*pi.value()))
*Foam::exp(-2.0*(
    Foam::pow(((xcoord[cellpointhit4]-b_g.value()-(Osc_Amp.value()*Foam::cos(2.0*pi.value()*Osc_freq.value()*time.value())))/((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focY)/Y_R),2.0))))),2.0)
    +Foam::pow((zcoord[cellpointhit4]-(v_arc.value()*time.value())-lg.value()-(Osc_Amp.value()*Foam::sin(2.0*pi.value()*Osc_freq.value()*time.value())))/((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((miny-focY)/Y_R),2.0)))),2.0)));
*/

}


Rayn+=1;
}//End loop over all rays x
}//End loop over all rays z

//RAY.correctBoundaryConditions();
sourceTerm.correctBoundaryConditions();





    forAll( mesh.C(), celli)
{
    vector XYZ = mesh.C()[celli];

if(time.value()>tshift){
// 
double focnow=(focY-(foc_shift_vel*(time.value()-tshift)));
double Qnow=max((Q_cond.value()-(Q_ramp_rate*(time.value()-tshift))),0.0);



BeamProfile[celli]=((2.0*Qnow)/
((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((XYZ.y()-focnow)/Y_R),2.0)))*(a_cond.value()*Foam::sqrt(1.0+Foam::pow(((XYZ.y()-focnow)/Y_R),2.0)))*pi.value()))
*Foam::exp(-2.0*(
    Foam::pow(((XYZ.x()-b_g.value()-(Osc_Amp.value()*Foam::cos(2.0*pi.value()*Osc_freq.value()*time.value())))/((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((XYZ.y()-focnow)/Y_R),2.0))))),2.0)
    +Foam::pow((XYZ.z()-(v_arc.value()*time.value())-lg.value()/*-(Osc_Amp.value()*Foam::sin(2.0*pi.value()*Osc_freq.value()*time.value()))*/)/((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((XYZ.y()-focnow)/Y_R),2.0)))),2.0)));


}
else{


BeamProfile[celli]=((2.0*Q_cond.value())/
((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((XYZ.y()-focY)/Y_R),2.0)))*(a_cond.value()*Foam::sqrt(1.0+Foam::pow(((XYZ.y()-focY)/Y_R),2.0)))*pi.value()))
*Foam::exp(-2.0*(
    Foam::pow(((XYZ.x()-b_g.value()-(Osc_Amp.value()*Foam::cos(2.0*pi.value()*Osc_freq.value()*time.value())))/((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((XYZ.y()-focY)/Y_R),2.0))))),2.0)
    +Foam::pow((XYZ.z()-(v_arc.value()*time.value())-lg.value()/*-(Osc_Amp.value()*Foam::sin(2.0*pi.value()*Osc_freq.value()*time.value()))*/)/((a_cond.value()*Foam::sqrt(1.0+Foam::pow(((XYZ.y()-focY)/Y_R),2.0)))),2.0)));
 
}


   }




/*
double Qnow=((Q_cond.value()-max(Q_ramp_rate*(time.value()-tshift),0.0)));
double TotalQ=gSum(sourceTerm)*yDim[5]*yDim[5]*yDim[5];

// sourceTerm*=
// Info<<"Q/Q \t"<<(Qnow/TotalQ)<<endl;;

// Info<<"Qnow \t"<<(Qnow)<<endl;;
if(TotalQ>0.0){
sourceTerm*=(Qnow/TotalQ);
}
Info<<"sourcesum: \t"<<
gSum(sourceTerm)*yDim[5]*yDim[5]*yDim[5]
<<endl;

*/
// }
// else{
// sourceTerm*=0.0;
// }


}




