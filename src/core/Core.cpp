/**
\file
\brief ��������
\author SavaLione
*/
#include <Windows.h>
#include <fstream>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <cstdlib>
#include <cstdio>
#include <direct.h>
#include <vector>

#include "Yenot.h"
#include "Core.h"
#include "..\io\Logger.h"

///////////////////////////////////////////////////////////////////////////////
//	Core
///////////////////////////////////////////////////////////////////////////////
void noiseRemoval(const cv::Mat& mat_in, cv::Mat& mat_out) {
	if (getSettings((char*)yenot::settings_block_core, (char*)yenot::settings_noiseReduction, yenot::settings_noiseReduction_value_int)) {
		// If no fast
		if (!getSettings((char*)yenot::settings_block_core, (char*)yenot::settings_fastmode, yenot::settings_fastmode_value_int)) {
			bilateral(mat_in, mat_out);
		} else {
			gaussianblur(mat_in, mat_out);
			logger((char*)yenot::logger_level_warning, (char*)yenot::logger_message_fMode);
		}
	} else {
		logger((char*)yenot::logger_level_warning, (char*)yenot::logger_message_noiseRemoval);
	}
}

void lineDetection(const cv::Mat& mat_in, cv::Mat& mat_out) {
	if (getSettings((char*)yenot::settings_block_core, (char*)yenot::settings_lineDetection, yenot::settings_lineDetection_value_int)) {
		if (!getSettings((char*)yenot::settings_block_core, (char*)yenot::settings_fastmode, yenot::settings_fastmode_value_int)) {
			canny(mat_in, mat_out);
		} else {
			/*===============================================================================================================================================================================*/
			logger((char*)yenot::logger_level_warning, (char*)yenot::logger_message_fMode);
		}
	} else {
		logger((char*)yenot::logger_level_warning, (char*)yenot::logger_message_lDetection);
	}
}

void databaseAdd(std::string filename) {
	std::vector<std::string> stringVector;
	cv::FileStorage fsIn;
	fsIn.open((yenot::database_name + std::string("\\") + yenot::database_file_name), cv::FileStorage::READ);
	fsIn[yenot::database_name] >> stringVector;
	fsIn.release(); //idk

	stringVector.insert(stringVector.end(), filename);

	cv::FileStorage fsOut((yenot::database_name + std::string("\\") + yenot::database_file_name), cv::FileStorage::WRITE);
	fsOut << yenot::database_name << stringVector;
	fsOut.release();
}

void databaseAddMember(std::string name) {
	databaseAdd(name + std::string(yenot::extensions_database_member));

	if (!check_file(yenot::database_name + std::string("\\") + (name + std::string(yenot::extensions_database_member)))) {
		int number_members = 0;

		cv::FileStorage fsOut((yenot::database_name + std::string("\\") + (name + std::string(yenot::extensions_database_member))), cv::FileStorage::WRITE);
		fsOut << "numbermembers" << number_members;
		fsOut.release();
	}

	createDir(yenot::database_name + std::string("\\") + name);
}

void AddMemberPhoto(std::string name, const cv::Mat& photo) {
	if (!check_file(yenot::database_name + std::string("\\") + (name + std::string(yenot::extensions_database_member)))) {
		logger((char*)yenot::logger_level_error, "Database member not found");
	} else {
		int number_members = 0;

		cv::FileStorage fsIn;
		fsIn.open((yenot::database_name + std::string("\\") + (name + std::string(yenot::extensions_database_member))), cv::FileStorage::READ);
		fsIn["numbermembers"] >> number_members;
		fsIn.release();

		if (number_members == 0) {
			//error. length not found
		} else {
			number_members++;

			cv::imwrite((yenot::database_name + std::string("\\") + name + std::string("\\") + std::to_string(number_members) + std::string(yenot::extensions_database_member_photo)), photo);

			cv::FileStorage fsOut((yenot::database_name + std::string("\\") + (name + std::string(yenot::extensions_database_member))), cv::FileStorage::WRITE);
			fsOut << "numbermembers" << number_members;
			fsOut.release();
		}
	}
}

void clearning(std::string filename, std::string variable) {
	std::vector<std::string> stringVector;
	cv::FileStorage fsIn;
	fsIn.open(filename, cv::FileStorage::READ);
	fsIn[variable] >> stringVector;
	fsIn.release(); //idk

	std::sort(stringVector.begin(), stringVector.end());
	stringVector.resize(std::unique(stringVector.begin(), stringVector.end()) - stringVector.begin());

	cv::FileStorage fsOut(filename, cv::FileStorage::WRITE);
	fsOut << variable << stringVector;
	fsOut.release();
}

///////////////////////////////////////////////////////////////////////////////
//	Filters
///////////////////////////////////////////////////////////////////////////////
void bilateral(const cv::Mat& mat_in, cv::Mat& mat_out) {
	//mat_out = mat_in.clone();

	/**
	src � Source 8-bit or floating-point, 1-channel or 3-channel image.
	dst � Destination image of the same size and type as src .
	d � Diameter of each pixel neighborhood that is used during filtering. If it is non-positive, it is computed from sigmaSpace.
	sigmaColor � Filter sigma in the color space. A larger value of the parameter means that farther colors within the pixel neighborhood (see sigmaSpace ) will be mixed together, resulting in larger areas of semi-equal color.
	sigmaSpace � Filter sigma in the coordinate space. A larger value of the parameter means that farther pixels will influence each other as long as their colors are close enough (see sigmaColor ). When d>0 , it specifies the neighborhood size regardless of sigmaSpace . Otherwise, d is proportional to sigmaSpace .
	*/
	bilateralFilter(mat_in, mat_out, yenot::diameter_each_pixel, yenot::sigmaColor, yenot::sigmaSpace);
}

void blur(const cv::Mat& mat_in, cv::Mat& mat_out) {
	cv::blur(mat_in, mat_out, cv::Size(yenot::blur_kernel_x, yenot::blur_kernel_y));
}

void fastNoiseRemovalGrey(const cv::Mat& mat_in, cv::Mat& mat_out) {
	cv::fastNlMeansDenoising(mat_in, mat_out, 3.0f, 7, 21);
}

void gaussianblur(const cv::Mat& mat_in, cv::Mat& mat_out) {
	cv::GaussianBlur(mat_in, mat_out, cv::Size(yenot::gaussianblur_kernel_x, yenot::gaussianblur_kernel_y), 0, 0);
}

void fastNoiseRemoval(const cv::Mat& mat_in, cv::Mat& mat_out) {
	cv::fastNlMeansDenoising(mat_in, mat_out, 3.0f, 7, 21);
}

///////////////////////////////////////////////////////////////////////////////
//	Settings
///////////////////////////////////////////////////////////////////////////////
std::string getSettingsString(char *block, char *value) {
	char text[yenot::buffer_size];
	GetPrivateProfileString(block, value, yenot::settings_block_default, text, yenot::buffer_size, yenot::settings_file_name);
	return text;
}

std::string getSettingsString(char *block, char *value, char *ch_return_default) {
	char text[yenot::buffer_size];
	GetPrivateProfileString(block, value, ch_return_default, text, yenot::buffer_size, yenot::settings_file_name);
	return text;
}

int getSettings(char *block, char *value) {
	return GetPrivateProfileInt(block, value, -1, yenot::settings_file_name);
}

int getSettings(char *block, char *value, int i_return_default) {
	return GetPrivateProfileInt(block, value, i_return_default, yenot::settings_file_name);
}

void setSettings(char *block, char *value, char *text) {
	WritePrivateProfileString(block, value, text, yenot::settings_file_name);
}

bool check_file(char *filename) {
	bool b_return = false;
	std::ifstream file;
	file.open(filename);
	if (file) {
		b_return = true;
	}
	return b_return;
}
bool check_file(std::string filename) {
	bool b_return = false;
	std::ifstream file;
	file.open(filename);
	if (file) {
		b_return = true;
	}
	return b_return;
}

void settings_initialization() {
	if ((_mkdir((char*)yenot::database_name)) == 0) {
		logger((char*)yenot::logger_level_warning, (char*)yenot::logger_message_cDir);
	} else {
		logger((char*)yenot::logger_level_warning, (char*)yenot::logger_message_cDir_not);
	}
	if (!check_file((char*)yenot::settings_file_name)) {
		createFile((char*)yenot::settings_file_name);

		setSettings((char*)yenot::settings_block_core, (char*)yenot::settings_fastmode, (char*)yenot::settings_fastmode_value);
		setSettings((char*)yenot::settings_block_core, (char*)yenot::settings_noiseReduction, (char*)yenot::settings_noiseReduction_value);
		setSettings((char*)yenot::settings_block_core, (char*)yenot::settings_machineLearning, (char*)yenot::settings_machineLearning_value);

		setSettings((char*)yenot::settings_block_logger, (char*)yenot::settings_log, (char*)yenot::settings_log_value);
		setSettings((char*)yenot::settings_block_logger, (char*)yenot::settings_logTime, (char*)yenot::settings_logTime_value);

		setSettings((char*)yenot::settings_block_carModel, (char*)yenot::settings_carModel_example, (char*)yenot::settings_carModel_example_description);

	}
	if (!check_file(yenot::database_name + std::string("\\") + yenot::database_file_name)) {
		std::vector<std::string> stringVector;
		cv::FileStorage fsOut((yenot::database_name + std::string("\\") + yenot::database_file_name), cv::FileStorage::WRITE);
		fsOut << yenot::database_name << stringVector;
		fsOut.release();
	}
	if (!check_file(yenot::database_name + std::string("\\") + yenot::settings_carModel_example_file)) {

		databaseAdd(yenot::settings_carModel_example_file);
	}
}

void createFile(char *file_name) {
	std::ofstream fout(file_name);
	fout.close();
}

void createDir(std::string namedir) {
	if ((_mkdir(namedir.c_str())) == 0) {
		logger((char*)yenot::logger_level_warning, (char*)yenot::logger_message_cDir);
	} else {
		logger((char*)yenot::logger_level_warning, (char*)yenot::logger_message_cDir_not);
	}
	//_mkdir(namedir.c_str());
}

///////////////////////////////////////////////////////////////////////////////
//	Line detector
///////////////////////////////////////////////////////////////////////////////
void canny(const cv::Mat& mat_in, cv::Mat& mat_out) {
	cv::Mat gray, edge, draw;
	cvtColor(mat_in, gray, CV_BGR2GRAY);
	Canny(gray, edge, 50, 150, 3);
	edge.convertTo(mat_out, CV_8U);
}

///////////////////////////////////////////////////////////////////////////////
//	Test
///////////////////////////////////////////////////////////////////////////////

void v_test() {
	for (int i = 1; i <= 128; i++) {
		unsigned int start_time = clock(); // ��������� �����

		///////////////////////////////////////////////////////////////////////////////
		cv::Mat image, out;
		image = cv::imread("a.png", 1);
		canny(image, out);
		///////////////////////////////////////////////////////////////////////////////

		unsigned int end_time = clock(); // �������� �����
		unsigned int search_time = end_time - start_time; // ������� �����

		double d = (double)search_time / 1000.0;

		logger_xy(d, i);
	}
}