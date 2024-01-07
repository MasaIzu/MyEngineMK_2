#include "FileFunction.h"

FileFunction::FileFunction()
{
}

FileFunction::~FileFunction()
{
}

bool FileFunction::IsFileExist(const std::string& name)
{
	return std::filesystem::is_regular_file(name);
}

template<typename T>
void FileFunction::WriteToFile(const T& data,const std::string& filename)
{
	std::ofstream file(filename,std::ios::binary);
	if ( file.is_open() )
	{
		data.writeToStream(file);
		file.close();
	}
}

template<typename T>
T FileFunction::ReadFromFile(const std::string& filename)
{
	T data;
	std::ifstream file(filename);
	if ( file.is_open() )
	{
		data.readFromStream(file);
		file.close();
	}
	return data;
}
