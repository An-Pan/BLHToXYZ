//
// Created by panan on 18-3-5.
//

#include "Read_LasFormat.h"
//#include "common.h"

#include <iostream>
using std::cout;
using std::endl;

void Read_LasFormat::Read_LasFile(const std::string &filepath, vector<navinfo::LaserPoint>& vecLaserPoint,  bool fake )
{
    FILE*FilePointer = fopen(filepath.c_str(), "rb");

    if (FilePointer == NULL)
    {
        /*exit(1);*/
		return;
    }
    Public_Header_Block Block_Header;
    Point_Data Point_Struct;

    size_t ReadL, pos = 0;
    unsigned short Header_length = 227;
    double X_c, Y_c, Z_c, Tem_intnsity;
    fseek(FilePointer, 0, SEEK_END);
    size_t file_size = ftell(FilePointer);
    fseek(FilePointer, 0, SEEK_SET);
    fread(&Block_Header, Header_length, 1, FilePointer);
    file_size = file_size - Header_length;

    unsigned short Data_length = Block_Header.Point_Data_Record_Length;

    do {
        size_t fuck = fread(&Point_Struct, Data_length, 1, FilePointer);
        if (fuck != 1)
            break;
        pos = pos + Data_length;
        X_c = Point_Struct.X*Block_Header.X_scale_factor + Block_Header.X_offset;
        Y_c = Point_Struct.Y*Block_Header.Y_scale_factor + Block_Header.Y_offset;
        Z_c = Point_Struct.Z*Block_Header.Z_scale_factor + Block_Header.Z_offset;
        Tem_intnsity = Point_Struct.Intensity;
        if (fake) {
            if (Point_Struct.Classification == 1) {
                vecLaserPoint.push_back(navinfo::LaserPoint(X_c, Y_c, Z_c, -100, Point_Struct.GPS_Time));
            }
            else if(Point_Struct.Classification == 2){
                vecLaserPoint.push_back(navinfo::LaserPoint(X_c, Y_c, Z_c, -200, Point_Struct.GPS_Time));
            }

        }
        else {

            vecLaserPoint.push_back(navinfo::LaserPoint(X_c, Y_c, Z_c, Tem_intnsity, Point_Struct.GPS_Time));
        }


    } while (pos <= file_size);
    fclose(FilePointer);

}


void Read_LasFormat::Save_LasFile(const std::string &outpath)
{
    Public_Header_Block Header_struct;
    Header_struct.FileSignature[0] = 'L';
    Header_struct.FileSignature[1] = 'A';
    Header_struct.FileSignature[2] = 'S';
    Header_struct.FileSignature[3] = 'F';
    Header_struct.File_Source_ID = 0;
    Header_struct.Global_Encoding = 0;
    Header_struct.Project_ID_GUID_data_1 = 0;
    Header_struct.Project_ID_GUID_data_2 = 0;
    Header_struct.Project_ID_GUID_data_3 = 0;
    for (int i = 0; i < 8; i++)
    {
        Header_struct.Project_ID_GUID_data_4[i] = '\0';
    }
    Header_struct.Version_Major = 1;
    Header_struct.Version_Minor = 2;
    Header_struct.System_Identifier[0] = 'l';
    Header_struct.System_Identifier[1] = 'i';
    Header_struct.System_Identifier[2] = 'b';
    Header_struct.System_Identifier[3] = 'L';
    Header_struct.System_Identifier[4] = 'A';
    Header_struct.System_Identifier[5] = 'S';
    for (int i = 6; i < 32; i++)
        Header_struct.System_Identifier[i] = '\0';
    Header_struct.Generating_Software[0] = 'l';
    Header_struct.Generating_Software[1] = 'i';
    Header_struct.Generating_Software[2] = 'b';
    Header_struct.Generating_Software[3] = 'L';
    Header_struct.Generating_Software[4] = 'A';
    Header_struct.Generating_Software[5] = 'S';
    Header_struct.Generating_Software[6] = '1';
    Header_struct.Generating_Software[7] = '.';
    Header_struct.Generating_Software[8] = '8';
    Header_struct.Generating_Software[9] = '.';
    Header_struct.Generating_Software[10] = '0';
    for (int i = 11; i < 32; i++)
        Header_struct.Generating_Software[i] = '\0';
    Header_struct.File_Creation_DRy_of_Year = 22;
    Header_struct.File_Creation_Year = 2017;
    Header_struct.Header_Size = 227;
    Header_struct.Offset_to_point_data = 227;
    Header_struct.Number_of_Variable_Length_Records = 0;
    Header_struct.Point_Data_Format_ID = 3;
    Header_struct.Point_Data_Record_Length = 34;
    Header_struct.X_scale_factor = 0.01;
    Header_struct.Y_scale_factor = 0.01;
    Header_struct.Z_scale_factor = 0.01;
    Header_struct.X_offset = 455524.75;
    Header_struct.Y_offset = 4443216.08;
    Header_struct.Z_offset = 26.98;
    FILE* fp = fopen(outpath.c_str(), "wb");
    fseek(fp, Header_struct.Header_Size, SEEK_SET);
    std::ifstream putin("G:\\Las2txt.txt");
    if (!putin)
        return;
    std::string line;
    double x_g, y_g, z_g;
    unsigned short Intnsiy;
    Point_Data input_point;
    input_point.Number_of_Returns = '\0';
    input_point.Return_Number = '\0';
    input_point.Scan_Direction_Flag = '\0';
    input_point.Edge_of_Flight_Line = '\0';
    input_point.Classification = '\0';
    input_point.Scan_Angle_Rank = '\0';
    input_point.User_Data = '\0';
    input_point.Point_Source_ID = 0;
    input_point.GPS_Time = 0;
    input_point.Red = 0;
    input_point.Green = 0;
    input_point.Blue = 0;
    int num_record_point = 0;
    double MRx_x, MRx_y, MRx_z, Min_x, Min_y, Min_z;
    while (!putin.eof())
    {
        num_record_point++;
        getline(putin, line);
        std::stringstream ss;
        ss << line;
        ss >> x_g;
        ss >> y_g;
        ss >> z_g;
        ss >> Intnsiy;
        if (num_record_point == 1)
        {
            MRx_x = x_g;
            Min_x = x_g;
            MRx_y = y_g;
            Min_y = y_g;
            MRx_z = z_g;
            Min_z = z_g;
        }
        else
        {
            if (x_g > MRx_x)
                MRx_x = x_g;
            if (y_g > MRx_y)
                MRx_y = y_g;
            if (z_g > MRx_z)
                MRx_z = z_g;
            if (x_g < Min_x)
                Min_x = x_g;
            if (y_g < Min_y)
                Min_y = y_g;
            if (z_g < Min_z)
                Min_z = z_g;
        }
        input_point.X = (x_g - Header_struct.X_offset) / Header_struct.X_scale_factor;
        input_point.Y = (y_g - Header_struct.Y_offset) / Header_struct.Y_scale_factor;
        input_point.Z = (z_g - Header_struct.Z_offset) / Header_struct.Z_scale_factor;
        input_point.Intensity = Intnsiy;
        size_t write_Length = fwrite(&input_point, 34, 1, fp);
    }
    Header_struct.MRx_X = MRx_x;
    Header_struct.Min_X = Min_x;
    Header_struct.MRx_Y = MRx_y;
    Header_struct.Min_Y = Min_y;
    Header_struct.MRx_Z = MRx_z;
    Header_struct.Min_Z = Min_z;
    Header_struct.Number_of_point_records = num_record_point;
    Header_struct.Number_of_points_by_return[0] = num_record_point;
    Header_struct.Number_of_points_by_return[1] = 0;
    Header_struct.Number_of_points_by_return[2] = 0;
    Header_struct.Number_of_points_by_return[3] = 0;
    Header_struct.Number_of_points_by_return[4] = 0;
    fseek(fp, 0, SEEK_SET);
    fwrite(&Header_struct, Header_struct.Header_Size, 1, fp);
}


void Read_LasFormat::Save_LasFile(const std::string outpath, std::vector<cv::Point3d>& vecPoints,std::vector<int>& vecIntensity)
{
	Public_Header_Block Header_struct;
	Header_struct.FileSignature[0] = 'L';
	Header_struct.FileSignature[1] = 'A';
	Header_struct.FileSignature[2] = 'S';
	Header_struct.FileSignature[3] = 'F';
	Header_struct.File_Source_ID = 0;
	Header_struct.Global_Encoding = 0;
	Header_struct.Project_ID_GUID_data_1 = 0;
	Header_struct.Project_ID_GUID_data_2 = 0;
	Header_struct.Project_ID_GUID_data_3 = 0;
	for (int i = 0; i < 8; i++)
	{
		Header_struct.Project_ID_GUID_data_4[i] = '\0';
	}
	Header_struct.Version_Major = 1;
	Header_struct.Version_Minor = 2;
	Header_struct.System_Identifier[0] = 'l';
	Header_struct.System_Identifier[1] = 'i';
	Header_struct.System_Identifier[2] = 'b';
	Header_struct.System_Identifier[3] = 'L';
	Header_struct.System_Identifier[4] = 'A';
	Header_struct.System_Identifier[5] = 'S';
	for (int i = 6; i < 32; i++)
		Header_struct.System_Identifier[i] = '\0';
	Header_struct.Generating_Software[0] = 'l';
	Header_struct.Generating_Software[1] = 'i';
	Header_struct.Generating_Software[2] = 'b';
	Header_struct.Generating_Software[3] = 'L';
	Header_struct.Generating_Software[4] = 'A';
	Header_struct.Generating_Software[5] = 'S';
	Header_struct.Generating_Software[6] = '1';
	Header_struct.Generating_Software[7] = '.';
	Header_struct.Generating_Software[8] = '8';
	Header_struct.Generating_Software[9] = '.';
	Header_struct.Generating_Software[10] = '0';
	for (int i = 11; i < 32; i++)
		Header_struct.Generating_Software[i] = '\0';
	Header_struct.File_Creation_DRy_of_Year = 22;
	Header_struct.File_Creation_Year = 2017;
	Header_struct.Header_Size = 227;
	Header_struct.Offset_to_point_data = 227;
	Header_struct.Number_of_Variable_Length_Records = 0;
	Header_struct.Point_Data_Format_ID = 3;
	Header_struct.Point_Data_Record_Length = 34;
	Header_struct.X_scale_factor = 0.01;
	Header_struct.Y_scale_factor = 0.01;
	Header_struct.Z_scale_factor = 0.01;
	Header_struct.X_offset = -23935.34;
	Header_struct.Y_offset = -91152.87;
	Header_struct.Z_offset = 28.72;
	FILE* fp = fopen(outpath.c_str(), "wb");
	fseek(fp, Header_struct.Header_Size, SEEK_SET);


	double x_g, y_g, z_g;
	unsigned short Intnsiy;
	Point_Data input_point;
	input_point.Number_of_Returns = '\0';
	input_point.Return_Number = '\0';
	input_point.Scan_Direction_Flag = '\0';
	input_point.Edge_of_Flight_Line = '\0';
	input_point.Classification = '\0';
	input_point.Scan_Angle_Rank = '\0';
	input_point.User_Data = '\0';
	input_point.Point_Source_ID = 0;
	input_point.GPS_Time = 0;
	input_point.Red = 0;
	input_point.Green = 0;
	input_point.Blue = 0;
	int num_record_point = 0;
	double MRx_x = INT_MIN;
	double MRx_y = INT_MIN;
	double MRx_z = INT_MIN;
	double Min_x = INT_MAX;
	double Min_y = INT_MAX;
	double Min_z = INT_MAX;

	for (int i = 0; i < vecPoints.size(); i++) {
		x_g = vecPoints[i].x;
		y_g = vecPoints[i].y;
		z_g = vecPoints[i].z;



		if (x_g > MRx_x)
			MRx_x = x_g;
		if (y_g > MRx_y)
			MRx_y = y_g;
		if (z_g > MRx_z)
			MRx_z = z_g;
		if (x_g < Min_x)
			Min_x = x_g;
		if (y_g < Min_y)
			Min_y = y_g;
		if (z_g < Min_z)
			Min_z = z_g;

		input_point.X = (x_g - Header_struct.X_offset) / Header_struct.X_scale_factor;
		input_point.Y = (y_g - Header_struct.Y_offset) / Header_struct.Y_scale_factor;
		input_point.Z = (z_g - Header_struct.Z_offset) / Header_struct.Z_scale_factor;
		input_point.Intensity = vecIntensity[i];
		size_t write_Length = fwrite(&input_point, 34, 1, fp);



	}


	Header_struct.MRx_X = MRx_x;
	Header_struct.Min_X = Min_x;
	Header_struct.MRx_Y = MRx_y;
	Header_struct.Min_Y = Min_y;
	Header_struct.MRx_Z = MRx_z;
	Header_struct.Min_Z = Min_z;
	Header_struct.Number_of_point_records = vecPoints.size();
	Header_struct.Number_of_points_by_return[0] = vecPoints.size();
	Header_struct.Number_of_points_by_return[1] = 0;
	Header_struct.Number_of_points_by_return[2] = 0;
	Header_struct.Number_of_points_by_return[3] = 0;
	Header_struct.Number_of_points_by_return[4] = 0;
	fseek(fp, 0, SEEK_SET);
	fwrite(&Header_struct, Header_struct.Header_Size, 1, fp);

	fclose(fp);
}