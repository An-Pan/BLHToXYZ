#include "stdafx.h"
#include "CoordinateConvert.h"

#define TRANS_ERROR_SUCCESS 0
#define PI 3.14159265

bool CoordinateConvert::Init(const string& src, const string& dst)
{
	bool bRet = false;
	if (!(this->pj_merc = pj_init_plus(dst.c_str())))
	{
		cerr << "pj_init_plus init error" << endl;
		return bRet;
	}

	if (!(pj_latlong = pj_init_plus(src.c_str())))
	{
		cerr << "pj_init_plus init error" << endl;
		return bRet;
	}

	return true;
}

bool CoordinateConvert::ProjSinglePoint(const cv::Point3d& geoPoint, cv::Point3d& gaussPoint)
{
	int nError = -1;
	gaussPoint.x = geoPoint.x;
	gaussPoint.y = geoPoint.y;
	gaussPoint.z = geoPoint.z;

	gaussPoint.x *= DEG_TO_RAD;
	gaussPoint.y *= DEG_TO_RAD;



	nError = pj_transform(pj_latlong, pj_merc, 1, 1, &gaussPoint.x, &gaussPoint.y, NULL);


	// Unsafe convert precision lost!  [12/1/2017 panan]
	string strX = std::to_string(gaussPoint.x);
	if (strX.size() < 2) {
		return false;
	}
	strX = string(strX.begin() + 2, strX.end());

	gaussPoint.x = boost::lexical_cast<double>(strX);

	return (nError == TRANS_ERROR_SUCCESS);
}

void CoordinateConvert::ProjPoints(const vector<cv::Point3d>& vecGeoPint, vector<cv::Point3d>& vecGaussPoint)
{
	vecGaussPoint.clear();

	for (const auto& it : vecGeoPint)
	{
		cv::Point3d gaussPoint = cv::Point3d(it);
		gaussPoint.x *= DEG_TO_RAD;
		gaussPoint.y *= DEG_TO_RAD;
		if (TRANS_ERROR_SUCCESS != pj_transform(pj_latlong, pj_merc, 1, 1, &gaussPoint.x, &gaussPoint.y, NULL))
		{
			cerr << "pj_transform error during ProjPoints()" << endl;
			continue;
		}
		vecGaussPoint.push_back(gaussPoint);
	}
}

void CoordinateConvert::InverseProjPoints(const std::vector<cv::Point3d>& vecGaussPoint, vector<cv::Point3d>& vecGeoPoint)
{
	vecGeoPoint.clear();

	for (const auto& it : vecGaussPoint)
	{
		cv::Point3d gaussPoint = cv::Point3d(it);

		if (TRANS_ERROR_SUCCESS != pj_transform(pj_latlong, pj_merc, 1, 1, &gaussPoint.x, &gaussPoint.y, NULL))
		{
			cerr << "pj_transform error during ProjPoints()" << endl;
			continue;
		}
		gaussPoint.x /= DEG_TO_RAD;
		gaussPoint.y /= DEG_TO_RAD;


		vecGeoPoint.push_back(gaussPoint);
	}
}

void CoordinateConvert::CvtToIMUCoordinate(const IMU_POS& pos, cv::Point3d& srcPt, cv::Point3d& dstPt)
{
	double pitch = -pos.posture.y;
	double roll = -pos.posture.z;
	double yaw = -pos.posture.z;

	double x_shift = pos.position.x;
	double y_shift = pos.position.y;
	double z_shift = pos.position.z;

	// pitch
	cv::Mat Rx = (cv::Mat_<double>(3, 3) << 1, 0, 0,
		0, cos(pitch*PI / 180), sin(pitch*PI / 180),
		0, -sin(pitch*PI / 180), cos(pitch*PI / 180));

	// row
	cv::Mat Ry = (cv::Mat_<double>(3, 3) << cos(roll*PI / 180), 0, -sin(roll*PI / 180),
		0, 1, 0,
		sin(roll*PI / 180), 0, cos(roll*PI / 180));

	// yaw
	cv::Mat Rz = (cv::Mat_<double>(3, 3) << cos(yaw*PI / 180), sin(yaw*PI / 180), 0,
		-sin(yaw*PI / 180), cos(yaw*PI / 180), 0,
		0, 0, 1
		);

	cv::Mat T = (cv::Mat_<double>(3, 1) << x_shift,
		y_shift,
		z_shift);


	cv::Mat src = (cv::Mat_<double>(3, 1) << srcPt.x, srcPt.y, srcPt.z);		
	src -= T;

	cv::Mat dst = Ry.inv()*Rx.inv()*Rz.inv()*src;

	dstPt.x = dst.at<double>(0, 0);
	// convert from world_3d cooridnate to image_3d cooridnate. [12/7/2017 panan]
	dstPt.y = -dst.at<double>(2, 0);
	dstPt.z = dst.at<double>(1, 0);	
}