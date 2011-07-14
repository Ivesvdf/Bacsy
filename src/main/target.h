#ifndef TARGET_H
#define TARGET_H

#include <string>
#include "configurationFile.h"

class Target
{
private:

public:
	Target(std::string sectionName, const ConfigurationFile& config, const ConfigurationFile& globalConfig);

};

#endif
