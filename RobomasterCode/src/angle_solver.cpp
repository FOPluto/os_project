#include "angle_solver.h"
#define DEBUG // 打开调试模式，输出yam轴pitch轴误差

AngleSolver::AngleSolver()
{

}

void AngleSolver::InitAngle()
{
    cam=Mat(3,3,CV_32FC1,Scalar::all(0));
    disCoeffD=Mat(1,5,CV_32FC1,Scalar::all(0));

    string file_path="/root/os_project/xml_path/camera.xml";
    FileStorage fr;
    fr.open(file_path,FileStorage::READ);
    while(!fr.isOpened()){
        cout<<"camera xml floading failed..."<<endl;
        fr=FileStorage(file_path,FileStorage::READ);
    }
    fr["camera-matrix"]>>cam;
    fr["distortion"]>>disCoeffD;
    fr.release();

    obj = vector<Point3f>{
            cv::Point3f(-61.5f,-30.0f,0),
            cv::Point3f(61.5f,-30.0f,0),
            cv::Point3f(61.5f,30.0f,0),
            cv::Point3f(-61.5f,30.0f,0)
    };
    rVec = cv::Mat::zeros(3,1,CV_64FC1);
    tVec = cv::Mat::zeros(3,1,CV_64FC1);
}


vector<double> AngleSolver::SolveAngle(vector<Point2f>& object_armor_points_)
{
    vector<double> res;
    if(!(object_armor_points_[0] == Point2f(0, 0) && object_armor_points_[1] == Point2f(0, 0) && object_armor_points_[2] == Point2f(0, 0) && object_armor_points_[3] == Point2f(0, 0)))
    {
        cv::solvePnP(obj,object_armor_points_,cam,disCoeffD,rVec,tVec,false,SOLVEPNP_ITERATIVE);
        _xErr = atan(tVec.at<double>(0, 0) / tVec.at<double>(2, 0)) / 2 / CV_PI * 360+5;
        _yErr = atan(tVec.at<double>(1, 0) / tVec.at<double>(2, 0)) / 2 / CV_PI * 360;

        if(_yErr < 15 && _yErr > -15)
        {
            p_y_err[0] = _xErr;
            p_y_err[1] = _yErr;
        }
        else
        {
            p_y_err[0] = 0;
            p_y_err[1] = 0;
        }
        double x_pos=tVec.at<double>(0,0)/1000;
        double y_pos=tVec.at<double>(1,0)/1000;
        double z_pos=tVec.at<double>(2,0)/1000;
        res.push_back(x_pos);
        res.push_back(y_pos);
        res.push_back(z_pos);
    }
    else
    {
        res.push_back(0);
        res.push_back(0);
        res.push_back(0);
    }


    return res;
}
int AngleSolver::shoot_get()
{
   // cout<<"shoot:"<<shoot<<endl;
    return shoot;
}

void AngleSolver::P4P_solver()
{
    double x_pos = tVec.at<double>(0, 0);
    double y_pos = tVec.at<double>(1, 0);
    double z_pos = tVec.at<double>(2, 0);

    double tan_pitch = y_pos / sqrt(x_pos*x_pos + z_pos * z_pos);
    double tan_yaw = x_pos / z_pos;
    x_pitch = -atan(tan_pitch) * 180 / CV_PI;
    y_yaw = atan(tan_yaw) * 180 / CV_PI;

    //！cout<<"x_pitch"<<x_pitch<<endl;
    //！cout<<"y_yaw"<<y_yaw<<endl;
}

void AngleSolver::gravity_comp()
{
    gra_t=distance_3d/BULLETFIRE_V;
    if(distance_3d>GRACOMP_DIS)
    {
        p_y_err[0] = _xErr;
//       p_y_err[1] = _yErr;
        p_y_err[1] = _yErr-(CAM_SUPPORT_DIS+4.9*gra_t*gra_t)*0.001;
        //p_y_err[1] = _yErr-(BULLETFIRE_V*(distance_3d/BULLETFIRE_V)*(CAM_SUPPORT_DIS/distance_3d)+0.5*9.8*gra_t*gra_t);

    }
    else
    {
        p_y_err[0] = _xErr;
        p_y_err[1] = _yErr;
    }
}
