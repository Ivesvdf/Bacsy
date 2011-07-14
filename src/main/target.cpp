#include <limits>
#include "target.h"

template<typename T>
T getCascadingValue(const ConfigurationFile& globalConfig, const std::string& globalSection, 
		const ConfigurationFile& localConfig, const std::string& localSection,
		const std::string& keyname, const T& defaultValue = T())
{
	T rv = defaultValue;

	rv = globalConfig.get<T>(globalSection, keyname, rv);
	rv = localConfig.get<T>(localSection, keyname, rv);

	return rv;
}

Target::Target(std::string section, const ConfigurationFile& config, const ConfigurationFile& globalConfig)
{
	const std::string globalSection = "global";

	const std::string includes = getCascadingValue<std::string>(globalConfig,
			globalSection, config, section, "Include");
	const std::string excludes = getCascadingValue<std::string>(globalConfig,
			globalSection, config, section, "Exclude");
	const unsigned int priority = getCascadingValue<unsigned int>(globalConfig,
			globalSection, config, section, "Priority");
	const unsigned int minBackups = getCascadingValue<unsigned int>(globalConfig,
			globalSection, config, section, "MinBackups", 1);
	const unsigned int maxBackups = getCascadingValue<unsigned int>(globalConfig,
			globalSection, config, section, "MaxBackups", std::numeric_limits<unsigned int>::max());
	const std::string preferredOrder = getCascadingValue<std::string>(globalConfig,
			globalSection, config, section, "PreferredOrder", "this, other");
	const std::string distribution = getCascadingValue<std::string>(globalConfig,
			globalSection, config, section, "Distribution", "focus");
}


