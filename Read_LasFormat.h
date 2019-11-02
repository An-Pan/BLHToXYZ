#pragma once

#include <stdio.h>
#include <string>
#include <fstream>
//#include <io.h>
#include <sstream>
#include <vector>




#include "basic_struct.h"


using namespace std;


#pragma pack(1)
class Read_LasFormat {
public:
    void Read_LasFile(const std::string &filepath, vector<navinfo::LaserPoint>& vecLaserPoint,bool fake = false);
	void Save_LasFile(const std::string outpath, std::vector<cv::Point3d>& vecPoints, std::vector<int>& vecIntensity);
    void Save_LasFile(const std::string &outpath);
private:
    typedef struct {
        char FileSignature[4];
        unsigned short File_Source_ID : 16;
        unsigned short Global_Encoding : 16;
        unsigned long Project_ID_GUID_data_1 : 32;
        unsigned short Project_ID_GUID_data_2 : 16;
        unsigned short Project_ID_GUID_data_3 : 16;
        unsigned char Project_ID_GUID_data_4[8];

        unsigned char Version_Major : 8;
        unsigned char Version_Minor : 8;
        char System_Identifier[32];
        char Generating_Software[32];
        unsigned short File_Creation_DRy_of_Year : 16;
        unsigned short File_Creation_Year : 16;
        unsigned short Header_Size : 16;

        unsigned long Offset_to_point_data : 32;
        unsigned long Number_of_Variable_Length_Records : 32;
        unsigned char Point_Data_Format_ID : 8;
        unsigned short Point_Data_Record_Length : 16;
        unsigned long Number_of_point_records : 32;

        uint32_t Number_of_points_by_return[5];

        double X_scale_factor;
        double Y_scale_factor;
        double Z_scale_factor;
        double X_offset;
        double Y_offset;
        double Z_offset;

        double MRx_X;
        double Min_X;
        double MRx_Y;
        double Min_Y;
        double MRx_Z;
        double Min_Z;

    }Public_Header_Block;

    typedef struct {
        long X : 32;
        long Y : 32;
        long Z : 32;
        unsigned short Intensity : 16;

        unsigned char Return_Number : 3;
        unsigned char Number_of_Returns : 3;
        unsigned char Scan_Direction_Flag : 1;
        unsigned char Edge_of_Flight_Line : 1;

        unsigned char Classification : 8;
        unsigned char Scan_Angle_Rank : 8;
        unsigned char User_Data : 8;
        unsigned short Point_Source_ID : 16;
        double GPS_Time;
        unsigned short Red : 16;
        unsigned short Green : 16;
        unsigned short Blue : 16;
    }Point_Data;

};

#pragma pack()