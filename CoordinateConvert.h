#pragma once
// ------------------------------------------------------------------------  
//  Copyright   : Copyright(c)2009北京四维图新科技股份有限公司
//  FileName    : CoordinateConvert.h  
//  Author      : panan@navinfo.com  
//  Created     : 2017/12/7 22:21:06  
//  Revision    : $Revision: 1.0 $  
//  Memo        :   
//  Summary     :1 Project points from wgs-84 to 2000, 
//				  2 Convert points from world-coordinate to imu-coordinate   
// ------------------------------------------------------------------------  


#include "opencv2/opencv.hpp"

// proj.4 header
#include <proj_api.h> 

#include <boost/lexical_cast.hpp>

using namespace std;

typedef struct _IMU_POS
{
	cv::Point3d position;
	cv::Point3d posture;

	_IMU_POS() {};
	_IMU_POS(cv::Point3d _position, cv::Point3d _posture)
		:position(_position), posture(_posture) {};

	~_IMU_POS() {};

}IMU_POS, *PIMU_POS;

class CoordinateConvert
{
public:
	CoordinateConvert() {};
	~CoordinateConvert() {
		if (pj_merc != nullptr) {
			pj_free(pj_merc);
			pj_merc = nullptr;
		}

		if (pj_latlong != nullptr) {
			pj_free(pj_latlong);
			pj_latlong = nullptr;
		}
	};

	bool Init(const std::string& src, const std::string& dst);

	bool ProjSinglePoint(const cv::Point3d& geoPoint, cv::Point3d& gaussPoint);

	void ProjPoints(const std::vector<cv::Point3d>& vecGeoPint, vector<cv::Point3d>& vecGaussPoint);

	void InverseProjPoints(const std::vector<cv::Point3d>& vecGaussPoint, vector<cv::Point3d>& vecGeoPoint);
	
	void CvtToIMUCoordinate(const IMU_POS& pos,cv::Point3d& srcPt, cv::Point3d& dstPt);

private:
	projPJ pj_merc;
	projPJ pj_latlong;
};

