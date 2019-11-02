#pragma once

#include <vector>
#include <string>
#include <iostream>


#include "CoordinateConvert.h"
using namespace std;

class YunZhou
{
public:
	YunZhou();
	~YunZhou();

	void CreateNewFile(const string& base_path);

	void GPSFillter(const string& file_path);

	CoordinateConvert* pConvert = nullptr; 
};

