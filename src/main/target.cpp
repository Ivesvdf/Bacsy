#include "target.h"

template<typename T>
T getCascadingValue(const ConfigurationFile& globalConfig, const std::string& globalSection, 
		const ConfigurationFile& localConfig, const std::string& localSection,
		const std::string& keyname, const T& defaultValue = "")
{
	std::string rv = defaultValue;

	rv = globalConfig.get<T>(globalSection, rv);
	rv = localConfig.get<T>(localSection, rv);

	return rv;
}

Target::Target(std::string section, const ConfigurationFile& config, const ConfigurationFile& globalConfig)
{
	const std::string globalSection = "global";

	const std::string includes = getCascadingValue<std::string>(globalConfig, globalSection, config, section, "Include");
}


