#include "FileFunction.h"

FileFunction::FileFunction(const std::string& filename) : FileName(filename),JsonName(".json")
{
}

FileFunction::~FileFunction()
{
}
//template<typename T>
//void FileFunction::WriteToFile(const T& data)
//{
//	std::ofstream os(FileName + JsonName);
//	cereal::JSONOutputArchive archive(os);
//	archive(cereal::make_nvp(FileName,data));
//}
//
//template<typename T>
//T FileFunction::ReadFromFile() {
//	T data;
//	std::ifstream is(FileName + JsonName);
//	if ( is )
//	{
//		cereal::JSONInputArchive archive(is);
//		archive(cereal::make_nvp(FileName,data));
//	}
//	return data;
//}