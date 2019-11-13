// BLHToXYZ.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
#include <string>
#include <iostream>

// Boost
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>


// OpenCV

#include "opencv2/opencv.hpp"
#include "CoordinateConvert.h"

#include "Read_LasFormat.h"
#include "basic_struct.h"
#include "YunZhou.h"


void SaveResult(std::vector<cv::Point3d>& vec_gass_point);
void SaveResult(string file_name, std::vector<cv::Point3d>& vec_gass_point, std::vector<int>& vec_instansity);

static const std::vector<std::string>& scan_files(const std::string& rootPath, std::vector<std::string>& container = *(new ::std::vector<std::string>())) {
	namespace fs = boost::filesystem;
	fs::path fullpath(rootPath);
	std::vector<std::string> &ret = container;

	if (!fs::exists(fullpath)) { return ret; }
	fs::directory_iterator end_iter;
	for (fs::directory_iterator iter(fullpath); iter != end_iter; iter++) {
		try {
			if (fs::is_directory(*iter)) {
				//ret.push_back(iter->path().string());
				//scan_files(iter->path().string(), ret);
				continue;
			}
			else {
				ret.push_back(iter->path().string());
			}
		}
		catch (const std::exception & ex) {
			std::cerr << ex.what() << std::endl;
			continue;
		}
	}
	return ret;
}

void LoadCSV(std::string str_path, std::vector<cv::Point3d>& vec_geo_point, std::vector<int>& vec_intansity);

// 经度纬度转投影坐标的使用DEMO
void ConvertShipCorrd()
{
	// 定义坐标转换对象
	CoordinateConvert* pConvert = new CoordinateConvert();
	// 定义坐标转换的参数，例如：椭球体定义，中心经线，投影方式等（此部分需要一定的地图学基础）
	if (!pConvert->Init("+proj=latlong +ellps=WGS84  +datum=WGS84 +no_defs", "+proj=tmerc +lat_0=0 +lon_0=117 +k=1 +x_0=39500000 +y_0=0 +ellps=GRS80 +units=m +no_defs")) {
		std::cerr << "Can not init proj.4 func,return." << std::endl;
		return ;
	}

	std::vector<cv::Point3d> vec_geo_point;		// 经纬度坐标
	std::vector<cv::Point3d> vec_gass_point;	// 投影坐标
	vec_gass_point.clear();
	vec_geo_point.clear();

	vec_geo_point.push_back(cv::Point3d(113.610133, 22.374701, -6.400000));	// 经纬度 搞成
	pConvert->ProjPoints(vec_geo_point, vec_gass_point);	// 投影
	cout << vec_gass_point[0] << endl;
}

int main()
{
	// GPS数据预处理，提取$GPGGA数据，并将 度/分 格式转换为 度格式 
	YunZhou solver;

	solver.GPSFillter("E:\\01Yunzhou\\10.19数据\\下午有目标GPS的数据\\south.txt");
	//solver.CreateNewFile("E:\\01Yunzhou\\10.19数据\\下午有目标GPS的数据");

	// 以下函数是经度纬度转投影坐标的使用DEMO
	ConvertShipCorrd();
	return 0;
	
	// 以下代码与云洲项目无关，不用关注
	
	CoordinateConvert* pConvert = new CoordinateConvert();

//	if (!pConvert->Init("+proj=latlong +ellps=WGS84  +datum=WGS84 +no_defs", "+proj=tmerc +lat_0=0 +lon_0=117 +k=1 +x_0=39500000 +y_0=0 +ellps=GRS80 +units=m +no_defs")) {
//		std::cerr << "Can not init proj.4 func,return." << std::endl;
//		return false;
//	}


	//if (!pConvert->Init("+proj=latlong +ellps=WGS84  +datum=WGS84 +no_defs", "+proj=tmerc +lat_0=0 +lon_0=138 +k=1 +x_0=46500000 +y_0=0 +ellps=GRS80 +units=m +no_defs")) {
	//	std::cerr << "Can not init proj.4 func,return." << std::endl;
	//	return false;
	//}

	//if (!pConvert->Init("+proj=latlong +ellps=WGS84  +datum=WGS84 +no_defs", "+proj=utm +zone=53 +ellps=GRS80 +units=m +no_defs")) {
	//	std::cerr << "Can not init proj.4 func,return." << std::endl;
	//	return false;
	//}

	CoordinateConvert* pInverseConvert = new CoordinateConvert();


	Read_LasFormat r;
	string las_path = "C:\\data\\1001-1-90005-180809\\Las\\46-1001190005180809-142527-00013.las";
	std::vector<navinfo::LaserPoint> vecLaserPoint;

	r.Read_LasFile(las_path, vecLaserPoint);

	if (!pInverseConvert->Init("+proj=tmerc +lat_0=0 +lon_0=138 +k=1 +x_0=46500000 +y_0=0 +ellps=GRS80 +units=m +no_defs","+proj=latlong +ellps=WGS84  +datum=WGS84 +no_defs")) {
		std::cerr << "Can not init proj.4 func,return." << std::endl;
		return false;
	}


	std::vector<cv::Point3d> vec_geo_point;
	std::vector<cv::Point3d> vec_gass_point;
	std::vector<int>		 vec_intansity;

	for (auto it : vecLaserPoint) {

		cv::Point3d gass_point;
		gass_point.x = it.x + 46000000;
		gass_point.y = it.y;
		gass_point.z = it.z - 37.906;

		vec_gass_point.push_back(gass_point);
		vec_intansity.push_back(it.intensity);
	}

	//vec_geo_point.push_back(cv::Point3d(46398541.6858, 3893830.0419, 42.403));

	pInverseConvert->InverseProjPoints(vec_gass_point, vec_geo_point);



	//if (!pConvert->Init("+proj=latlong +ellps=WGS84  +datum=WGS84 +no_defs", "+proj=tmerc +lat_0=36 +lon_0=137.1666666666667 +k=0.9999 +x_0=0 +y_0=0 +ellps=GRS80 +units=m +no_defs")) {
	//	std::cerr << "Can not init proj.4 func,return." << std::endl;
	//	return false;
	//}

	if (!pConvert->Init("+proj=latlong +ellps=WGS84  +datum=WGS84 +no_defs", "+proj=tmerc +lat_0=0 +lon_0=114 +k=1 +x_0=38500000 +y_0=0 +ellps=GRS80 +units=m +no_defs")) {
		std::cerr << "Can not init proj.4 func,return." << std::endl;
		return false;
	}
	vec_gass_point.clear();
	vec_geo_point.clear();

	vec_geo_point.push_back(cv::Point3d(114.28893242662, 30.66363843059, 11.917));
	pConvert->ProjPoints(vec_geo_point, vec_gass_point);
	cout << vec_gass_point[0] << endl;
	//r.Save_LasFile("cvt.las", vec_gass_point, vec_intansity);
	//SaveResult("convert.csv", vec_gass_point, vec_intansity);


	if (!pInverseConvert->Init("+proj=tmerc +lat_0=0 +lon_0=114 +k=1 +x_0=38500000 +y_0=0 +ellps=GRS80 +units=m +no_defs", "+proj=latlong +ellps=WGS84  +datum=WGS84 +no_defs")) {
		std::cerr << "Can not init proj.4 func,return." << std::endl;
		return false;
	}

	vec_geo_point.clear();
	//for (auto &it : vec_gass_point) {
	//	it.x += 38000000;
	//}

	pInverseConvert->InverseProjPoints(vec_gass_point, vec_geo_point);


	return 0;

	std::vector<string> path_list;
	scan_files("C:\\TMIcsv\\blhxy", path_list);

	
	for (int i = 0; i < path_list.size(); i++) {

		std::cout << "Processing :" << i << "th, name: " << path_list[i] << std::endl;

		vec_geo_point.clear();
		vec_gass_point.clear();
		vec_intansity.clear();
		//LoadCSV(path_list[i], vec_geo_point, vec_intansity);
		LoadCSV("C:\\TMIcsv\\Laser3D_Convert_I_0000_xyHBLh.csv", vec_geo_point, vec_intansity);
		
		pConvert->ProjPoints(vec_geo_point, vec_gass_point);



		SaveResult("C:\\TMIConveret\\" + std::to_string(i) + ".csv", vec_gass_point, vec_intansity);

	}





	//vector<cv::Point3d> vec_geo_point;
	//vector<cv::Point3d> vec_gass_point;
	//LoadCSV("C:\\TMIcsv\\blhxy\\Laser3D_Convert_I_0001_xyHBLh.csv", vec_geo_point);

	//pConvert->ProjPoints(vec_geo_point, vec_gass_point);

	//SaveResult(vec_gass_point);

	if (pConvert != nullptr) {
		delete pConvert;
		pConvert = nullptr;
	}

	return 0;
}

void LoadCSV(std::string str_path, std::vector<cv::Point3d>& vec_geo_point, std::vector<int>& vec_intansity)
{
	ifstream csv_file(str_path);
	if (!csv_file.is_open()) {
		std::cout << "can not open " << str_path << std::endl;
	}


	char buf[1024] = { 0 };
	while (csv_file.getline(buf, 1024)) {

		string str_buf = buf;

		std::vector<string> items;
		boost::split(items, str_buf, boost::is_any_of(","));

		if (items.size() != 9) {
			continue;
		}

		double x = boost::lexical_cast<double>(items[4]);
		double y = boost::lexical_cast<double>(items[3]);
		double z = boost::lexical_cast<double>(items[5]);

		int intans = boost::lexical_cast<int>(items[8]);
		vec_intansity.push_back(intans);


		vec_geo_point.push_back(cv::Point3d(x, y, z));


		memset(buf, 0, 1024);

	}

}

void SaveResult(std::vector<cv::Point3d>& vec_gass_point)
{
	ofstream gass_file("test2.txt");
	for (auto& it : vec_gass_point)
	{
		// Unsafe convert precision lost!  [12/1/2017 panan]
		string strX = std::to_string(it.x);
		if (strX.size() < 2) {
			return ;
		}
		strX = string(strX.begin() + 2, strX.end());

		it.x = boost::lexical_cast<double>(strX);


		gass_file << std::setprecision(16) << it.x << "," << std::setprecision(16) << it.y << "," <<std::setprecision(16) << it.z << std::endl;
	}
}

void SaveResult(string file_name ,std::vector<cv::Point3d>& vec_gass_point, std::vector<int>& vec_instansity)
{
	ofstream gass_file(file_name);

	if (vec_gass_point.size() != vec_instansity.size()) {
		std::cout << "data error ." << std::endl;
	}
	for (int i = 0; i < vec_gass_point.size(); i++) {
		// Unsafe convert precision lost!  [12/1/2017 panan]
		//string strX = std::to_string(vec_gass_point[i].x);
		//if (strX.size() < 2) {
		//	return;
		//}
		//strX = string(strX.begin() + 2, strX.end());

		//vec_gass_point[i].x = boost::lexical_cast<double>(strX);


		gass_file << std::setprecision(16) << vec_gass_point[i].x << "," << std::setprecision(16) << vec_gass_point[i].y << "," << std::setprecision(16) << vec_gass_point[i].z <<
			","<< vec_instansity[i] << std::endl;
	}

	gass_file.close();
}
