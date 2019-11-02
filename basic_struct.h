#ifndef NAVINFO_COMMON_BASIC_STRUCT_H
#define NAVINFO_COMMON_BASIC_STRUCT_H


#include "opencv2/opencv.hpp"
namespace navinfo
{
	extern int g_index;

	struct PostT
	{
		int SeqNum;
		double GPSTime;
		double Northing;
		double Easting;
		double Height;
		double Latitude;
		double Longitude;
		double HzSpeed;
		double Roll;
		double Pitch;
		double Heading;
		//std::string ProjectName;
		int Q;

	};

	enum class POINT_TYPE
	{
		POINT_UNKNOWN = -1,
		POINT_LANE,
		POINT_SIGN,
		POINT_BAD
	};

	struct FusionPoint
	{
		FusionPoint(double _x, double _y, double _z, int _u, int _v, size_t _id = 0, int _intensity = 0, POINT_TYPE _type = POINT_TYPE::POINT_UNKNOWN) :
			u(_u), v(_v), intensity(_intensity), x(_x), y(_y), z(_z), id(_id), type(_type) {};

// 		uint8_t u;
// 		uint8_t v;
// 		uint8_t intensity;
		int u;
		int v;
		int intensity;
		POINT_TYPE type;
		size_t id;	// instance id or cluster id [3/16/2018 panan]
		float x;
		float y;
		float z;

	};

	struct LaserPoint
	{
		LaserPoint(double _x, double _y, double _z, int _i, double _t) :
			x(_x), y(_y), z(_z), intensity(_i), time(_t) {
		};
		double x;
		double y;
		double z;
		int intensity;
		double time;
	};


	struct IMU_POS
	{
		cv::Point3d position;			//x-Easting,y-Northing,z-H
		cv::Point3d posture;			//x-Roll,y-Pitch,z-Heading

		IMU_POS() {};
		IMU_POS(cv::Point3d _position, cv::Point3d _posture)
			:position(_position), posture(_posture) {};

		~IMU_POS() {};

	};

	struct GeoPoint3D
	{
		double geo_x;
		double geo_y;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
		double geo_z;

		GeoPoint3D() {};
		GeoPoint3D(double x, double y, double z) :
			geo_x(x), geo_y(y), geo_z(z) {};

// 		bool operator<(const GeoPoint3D& pt) const {
// 			if (this->geo_x == pt.geo_x) {
// 				if (this->geo_y == pt.geo_y) {
// 					return this->geo_z < pt.geo_z;
// 				}
// 				else {
// 					return this->geo_y<pt.geo_y;
// 				}
// 			}
// 			else {
// 				return this->geo_x < pt.geo_x;
// 			}
// 		}

		bool operator<(const GeoPoint3D& pt) const {
			if (this->geo_x == pt.geo_x) {
				if (this->geo_y == pt.geo_y ) {
					return this->geo_z < pt.geo_z;
				}
				else {
					return this->geo_y < pt.geo_y;
				}
			}
			else {
				return this->geo_x < pt.geo_x;
			}
		}


	};
// 	struct GeoPoint3DHash
// 	{
// 		std::size_t operator()(const GeoPoint3D& pt)const {
// 			return (pt.geo_x*100)
// 		}
// 	};


}


#endif  