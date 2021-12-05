#include <maya/MStatus.h>
#include <string>
#include <fstream>
#include <sstream>
#include "jsonUtils.h"

MStatus jsonUtils::getValue(const nlohmann::json &j, const std::string &key, int &result)
{
	nlohmann::json::const_iterator it = j.find(key);
	if (it == j.end())
	{
		cerr << "Couldn't find key: " << key << endl;
		return MS::kUnknownParameter;
	}
	result = int(*it);
	return MS::kSuccess;
}

MStatus jsonUtils::getValue(const nlohmann::json &j, const std::string &key, std::string &result)
{
	nlohmann::json::const_iterator it = j.find(key);
	if (it == j.end())
	{
		cerr << "Couldn't find key: " << key << endl;
		return MS::kUnknownParameter;
	}
	result = std::string(*it);
	return MS::kSuccess;
}

MStatus jsonUtils::getValue(const nlohmann::json &j, const std::string &key, nlohmann::json &result)
{
	nlohmann::json::const_iterator it = j.find(key);
	if (it == j.end())
	{
		cerr << "Couldn't find key: " << key << endl;
		return MS::kUnknownParameter;
	}
	result = *it;
	return MS::kSuccess;
}
