/**
	@file
	@brief Ядро проекта. Содержит все главные и вспомогательные функции для определения марки автомобиля
	@author SavaLione
	@date 12 Apr 2018
*/
#ifndef CORE_H
#define CORE_H

#include <opencv2/opencv.hpp>

///////////////////////////////////////////////////////////////////////////////
//	Core
///////////////////////////////////////////////////////////////////////////////
void noiseRemoval(const cv::Mat& mat_in, cv::Mat& mat_out);
void lineDetection(const cv::Mat& mat_in, cv::Mat& mat_out);
void databaseAdd(std::string filename);
void clearning(std::string filename, std::string variable);
bool detectionLogo(const cv::Mat& mat_logo, std::string cascadefile);
void detection(const cv::Mat& mat_logo);
void help();

///////////////////////////////////////////////////////////////////////////////
//	Filters
///////////////////////////////////////////////////////////////////////////////
void blur(const cv::Mat& mat_in, cv::Mat& mat_out);
void fastNoiseRemovalGrey(const cv::Mat& mat_in, cv::Mat& mat_out);
void fastNoiseRemoval(const cv::Mat& mat_in, cv::Mat& mat_out);

///////////////////////////////////////////////////////////////////////////////
//	Line detector
///////////////////////////////////////////////////////////////////////////////
void canny(const cv::Mat& mat_in, cv::Mat& mat_out);

///////////////////////////////////////////////////////////////////////////////
//	Settings
///////////////////////////////////////////////////////////////////////////////
std::string getSettingsString(char *block, char *value);
std::string getSettingsString(char *block, char *value, char *ch_return_default);
int getSettings(char *block, char *value);
int getSettings(char *block, char *value, int i_return_default);
void setSettings(char *block, char *value, char *text);
bool check_file(char *filename);
bool check_file(std::string filename);
void settings_initialization();
void createFile(char *file_name);
void createDir(std::string namedir);
std::string description(std::string value);

///////////////////////////////////////////////////////////////////////////////
//	Test
///////////////////////////////////////////////////////////////////////////////

void v_test();

#endif // CORE_H