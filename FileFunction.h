#pragma once
#include <fstream>
#include <string>
#include <filesystem>

class FileFunction
{

public://基本関数
	FileFunction( );
	~FileFunction( );

	//初期化
	template <typename T>
	void WriteToFile(const T& data,const std::string& filename);

	template <typename T>
	T ReadFromFile(const std::string& filename);

	//ファイルがあるかどうか
	bool IsFileExist(const std::string& name);

private://メンバ関数
	

public://Setter

public://Getter

private://クラス関連

private://イーナムクラス

private://別クラスから値をもらう

private://クラス変数

};
