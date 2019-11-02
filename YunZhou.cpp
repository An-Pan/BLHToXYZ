#include "stdafx.h"
#include "YunZhou.h"


#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include <opencv2/opencv.hpp>

#include <fstream>

using namespace cv;

static const vector<string>& scan_files_recursion(const string& rootPath, vector<string>& container = *(new vector<string>())) {
	namespace fs = boost::filesystem;
	fs::path fullpath(rootPath);
	vector<string> &ret = container;

	if (!fs::exists(fullpath)) { return ret; }
	fs::directory_iterator end_iter;
	for (fs::directory_iterator iter(fullpath); iter != end_iter; iter++) {
		try {
			if (fs::is_directory(*iter)) {
				//ret.push_back(iter->path().string());
				scan_files_recursion(iter->path().string(), ret);
			}
			else {
				if(iter->path().string().find(".txt") != std::string::npos)
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


YunZhou::YunZhou()
{
	pConvert = new CoordinateConvert();

	if (!pConvert->Init("+proj=latlong +ellps=WGS84  +datum=WGS84 +no_defs", "+proj=tmerc +lat_0=0 +lon_0=117 +k=1 +x_0=39500000 +y_0=0 +ellps=GRS80 +units=m +no_defs")) {
		std::cerr << "Can not init proj.4 func,return." << std::endl;
		return;
	}
}


YunZhou::~YunZhou()
{
}


void YunZhou::CreateNewFile(const string& base_path)
{
	std::vector<string> txt_list;
	scan_files_recursion(base_path, txt_list);
	
	
	for (int i = 0; i < txt_list.size(); i++) {
		
		ifstream ins_file(txt_list[i]);
		if (!ins_file.is_open())
			continue;
		boost::filesystem::path ph(txt_list[i]);

		string file_name = ph.stem().string();
		string folder_path = ph.parent_path().string();
		file_name += "ex.txt";
		file_name = folder_path + "\\"+file_name;
		ofstream ins_ex_file(file_name);
		if (!ins_ex_file.is_open()) {
			cout << "can not open the output file" << endl;
		}

		char buf[1024] = { 0 };
		while (ins_file.getline(buf, 1024)) {
			string strbuf = buf;
			vector<string> items;
			boost::split(items, strbuf, boost::is_any_of(" "));
			if (items.size() != 17)
				continue;

			double geo_x = boost::lexical_cast<double>(items[8]);
			double geo_y = boost::lexical_cast<double>(items[7]);
			double geo_z = boost::lexical_cast<double>(items[9]);
			cv::Point3d gauss_point;
			pConvert->ProjSinglePoint(cv::Point3d(geo_x, geo_y, geo_z), gauss_point);
			ins_ex_file << strbuf << " " << std::setprecision(16)<<gauss_point.x << " " << std::setprecision(16) << gauss_point.y << " " << std::setprecision(16)<<gauss_point.z << std::endl;
		}

		ins_file.close();
		ins_ex_file.close();

	}
}

void YunZhou::GPSFillter(const string& file_path)
{
	ifstream file_in("E:\\01Yunzhou\\10.19数据\\下午有目标GPS的数据\\south.txt");

	char buf[2048] = { 0 };

	ofstream file_out("E:\\01Yunzhou\\10.19数据\\下午有目标GPS的数据\\gps_filter.txt");

	while (file_in.getline(buf, 2048)) {
		string strbuf = buf;
		boost::trim(strbuf);
		auto begin = strbuf.find("$GPGGA");
		if (begin == string::npos) {
			continue;
		}

		string gps_items = std::string(strbuf.begin()+ begin, strbuf.end());

		vector<string> items;
		boost::split(items, gps_items, boost::is_any_of(","));
		if (items.size() < 12)
			continue;

		if (items[1].empty() || items[2].empty() || items[4].empty() || items[11].empty())
			continue;
		double gps_time = boost::lexical_cast<double>(items[1]);
		double northing = boost::lexical_cast<double>(items[2]);

		double northing_minut = northing - 22.0 * 100.0;

		double northing_cvt = 22.0  + northing_minut / 60.0;
		double easting = boost::lexical_cast<double>(items[4]);
		double eastring_minut = easting - 113 * 100.0;

		double eastring_cvt = 113  + eastring_minut / 60.0;


		double height = boost::lexical_cast<double>(items[11]);



		cv::Point3d gauss_point;
		pConvert->ProjSinglePoint(cv::Point3d(eastring_cvt, northing_cvt, height), gauss_point);
		file_out << std::setprecision(16) <<gps_time <<","<< std::setprecision(16) << eastring_cvt << "," << std::setprecision(16) << northing_cvt << "," << std::setprecision(16) << height << ","
			<< std::setprecision(16) << gauss_point.x << "," << std::setprecision(16) << gauss_point.y << "," << std::setprecision(16) << gauss_point.z << std::endl;
	


	}
	file_in.close();
	file_out.close();


}