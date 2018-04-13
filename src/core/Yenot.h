/**
	\file
	\brief ������������ ���� � ��������___.
	\author SavaLione
*/
#ifndef YENOT_H
#define YENOT_H

namespace yenot {
	const char settings_file_name[] = "./config.ini";
	const char logger_file_name[] = "Yenot.log";

	const int i_return = -1;
	const char ch_default_block[] = "General";

	const char settings_logger[] = "log";

	// Diameter of each pixel neighborhood that is used during filtering. If it is non-positive, it is computed from sigmaSpace.
	const int diameter_each_pixel = 20;

	// sigmaColor � Filter sigma in the color space. A larger value of the parameter means that farther colors within the pixel neighborhood (see sigmaSpace ) will be mixed together, resulting in larger areas of semi-equal color.
	const int sigmaColor = 40;

	// sigmaSpace � Filter sigma in the coordinate space. A larger value of the parameter means that farther pixels will influence each other as long as their colors are close enough (see sigmaColor ). When d>0 , it specifies the neighborhood size regardless of sigmaSpace . Otherwise, d is proportional to sigmaSpace.
	const int sigmaSpace = 40;

	const int gaussianblur = 20;

	const char ch_default_value[] = "Default";

	const int buffer_size = 128;

	///////////////////////////////////////////////////////////////////////////////
	//	Core
	///////////////////////////////////////////////////////////////////////////////
	const char settings_block_core[] = "Core";
	const char settings_value_fastmode[] = "fastmode";
	const char settings_value_noiseReduction[] = "noiseReduction";
	const char settings_value_machineLearning[] = "machineLearning";

	///////////////////////////////////////////////////////////////////////////////
	//	Logger
	///////////////////////////////////////////////////////////////////////////////
	const char settings_block_logger[] = "Logger";
	const char settings_value_log[] = "log";
	const char settings_value_logTime[] = "logTime";
}

#endif // YENOT_H