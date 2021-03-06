/**
	@file
    @brief 
	
	Основной задачей нашего проекта является разработка самообучающегося приложения для выделения и распознавания логотипов автомобилей по фото.
	
	@author SavaLione
	@date 28 Mar 2018
*/
/**
	@defgroup yenotcpp Yenot.cpp
	@ingroup core
	@{
*/
#include "Yenot.h"
#include "Core.h"
#include "Logger.h"
#include "Settings.h"

using namespace cv;
using namespace std;
using namespace yenot;

int main(int argc, char* argv[]) {
	bool error = false;
	int i_return = 0;
	Mat src, dst;
	
	char* imagename;
	
	try {
		settingsInitialization();
	} catch (string& e) {
		cerr << e;
		error = true;
		i_return = ERROR_INIT;
	}
	
	if (!error)
		try {
			if (argc < 2) {
				cerr << ("Usage: " + (string)argv[0] + " image.png");
				throw ("Usage: " + (string)argv[0] + " image.png");
			} else {
				imagename = argv[1];
			}
		} catch (string& e) {
			logger((char*)LOGGER_LEVEL_ERROR, (char*)e.c_str());
			error = true;
			i_return = ERROR_IMAGE;
		}

	if (!error) {
		src = imread(imagename, 1);
	}
	
	if (!error)
		try {
			clearning((NAME_DATABASE + std::string("\\") + FILE_NAME_DATABASE), NAME_DATABASE);
		} catch (string& e) {
			logger((char*)LOGGER_LEVEL_ERROR, (char*)e.c_str());
			error = true;
			i_return = ERROR_CLEARNING;
		}
	
	if (!error)
		try {
			int pSize = getSettings((char*)BLOCK_CORE, (char*)SETTINGS_SIZE_PHOTO, SIZE_PHOTO);
			resize(src, dst, Size(pSize, pSize));
			if (dst.data) { src = dst.clone(); }
		} catch (string& e) {
			logger((char*)LOGGER_LEVEL_ERROR, (char*)e.c_str());
			error = true;
			i_return = ERROR_RESIZE;
		}
	
	if (!error)
		try {
			noiseRemoval(src, dst);
			if (dst.data) { src = dst.clone(); }
		} catch (string& e) {
			logger((char*)LOGGER_LEVEL_ERROR, (char*)e.c_str());
			error = true;
			i_return = ERROR_NOISE_REMOVAL;
		}

	if (!error)
		try {
			lineDetection(src, dst);
			if (dst.data) { src = dst.clone(); }
		} catch (string& e) {
			logger((char*)LOGGER_LEVEL_ERROR, (char*)e.c_str());
			error = true;
			i_return = ERROR_LINE_DETECTION;
		}

	if (!error)
		try {
			detection(src);
		} catch (string& e) {
			logger((char*)LOGGER_LEVEL_ERROR, (char*)e.c_str());
			error = true;
			i_return = ERROR_DETECTION;
		}

	if (!error)
		if (getSettings((char*)BLOCK_CORE, (char*)SETTINGS_SAVE_PROCESSED_IMAGE, SETTINGS_SAVE_PROCESSED_IMAGE_VALUE_INT)) {
			imwrite((char*)SETTINGS_SAVE_PROCESSED_IMAGE_NAME, src);
		}
	
	return i_return;
}
/** @} */