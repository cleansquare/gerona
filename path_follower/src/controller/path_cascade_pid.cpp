/**
    Copyright (C) 2015 Karsten Bohlmann E&K AUTOMATION GmbH
    All rights reserved.

    @year   2015
    @author Karsten Bohlmann
    @email  bohlmann@gmail.com
    @file   path_simple_pid.cpp


*/
#include <math.h>
#include <iomanip>
#include "path_cascade_pid.h"

PathCascadePid::PathCascadePid()
{

    fwd_angle_pid_ = PidController<1>(opt_.fwd_csca_kp(), opt_.fwd_csca_ki(), opt_.fwd_csca_kd(), opt_.pid_ta());
    fwd_dist_pid_ = PidController<1>(opt_.fwd_cscd_kp(), opt_.fwd_cscd_ki(), opt_.fwd_cscd_kd(), opt_.pid_ta());
    bwd_angle_pid_ = PidController<1>(opt_.bwd_csca_kp(), opt_.bwd_csca_ki(), opt_.bwd_csca_kd(), opt_.pid_ta());
    bwd_dist_pid_ = PidController<1>(opt_.bwd_cscd_kp(), opt_.bwd_cscd_ki(), opt_.bwd_cscd_kd(), opt_.pid_ta());



}


bool PathCascadePid::execute(double dist_error, double angle_error, double velocity, const std::vector<double> &target_u, std::vector<double> &u)
{
    clipErrors(angle_error,dist_error);
    double v = fabs(velocity);
    if (v>=1.0) {
        // reduce error for high speeds
        angle_error /=  fabs(v);
        dist_error /= fabs(v);
    }
    float u_angle, u_dist;
    bool do_ctrl = true;
    if (velocity>1e-3) {
         fwd_angle_pid_.execute(angle_error, &u_angle);
         fwd_dist_pid_.execute(dist_error, &u_dist);
    } else {
         bwd_angle_pid_.execute(angle_error, &u_angle);
         bwd_dist_pid_.execute(dist_error, &u_dist);

    }
    ROS_INFO_STREAM_THROTTLE(0.2, std::setprecision(2) << "e_angle "<<angle_error*180.0/M_PI <<"deg e_dist "
                              << dist_error <<"u_angle "<< u_angle*180.0/M_PI << "u_dist"<<u_dist);


    u.resize(1);
    u[0] = u_angle+u_dist;
    return true;
}



void PathCascadePid::clipErrors(double &angle_error, double &dist_error)
{
    if (angle_error>opt_.clip_angle_error()) {
        angle_error = opt_.clip_angle_error();
    } else if (angle_error<-opt_.clip_angle_error()) {
        angle_error = -opt_.clip_angle_error();

    }
    if (dist_error>opt_.clip_dist_error()) {
        dist_error = opt_.clip_dist_error();
    } else if (dist_error<-opt_.clip_dist_error()) {
        dist_error = -opt_.clip_dist_error();

    }
}