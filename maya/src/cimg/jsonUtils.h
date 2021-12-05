
#ifndef jsonUtils_H
#define jsonUtils_H

#include "json.hpp"

class jsonUtils
{

public:
	static MStatus getValue(const nlohmann::json &j, const std::string &key, int &result);

	static MStatus getValue(const nlohmann::json &j, const std::string &key, std::string &result);

	static MStatus getValue(const nlohmann::json &j, const std::string &key, nlohmann::json &result);
};

#endif
