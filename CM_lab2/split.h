#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "Point.h"


namespace Com_Methods
{

void RegularSplit(std::vector<Com_Methods::Point>& Points, double a, double b, int n);//build regular split

void RegularSplit(std::vector<Com_Methods::Point>& Points, const std::string PATH); //PATH to file with segment and number of segments

void AdaptiveSplit(std::vector<Com_Methods::Point>& Points, double a, double b, int n, double r);//build regular split

void AdaptiveSplit(std::vector<Com_Methods::Point>& Points, const std::string PATH); //PATH to file with segment, number of segments and factor
}