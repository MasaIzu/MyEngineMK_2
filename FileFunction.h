#pragma once

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <fstream>
#include <string>
#include <filesystem>
#include <cereal/types/string.hpp>
#include <cereal/archives/json.hpp>
MY_SUPPRESS_WARNINGS_END

class FileFunction
{

public://基本関数
	FileFunction(const std::string& filename);
	~FileFunction( );

	////初期化
	//template <typename T>
	//void WriteToFile(const T& data);

	//template <typename T>
	//T ReadFromFile();

	// ファイルが存在するかどうかを確認する関数
	bool FileExists() {
		return std::filesystem::exists(FileName + JsonName);
	}

private://メンバ関数
	

public://Setter

public://Getter

private://クラス関連

private://イーナムクラス

private://別クラスから値をもらう

private://クラス変数
	std::string FileName;
	std::string JsonName;
};
