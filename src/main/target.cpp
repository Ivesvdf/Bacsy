#include <limits>
#include "woodcutter/woodcutter.h"
#include "target.h"

template<typename T>
T getCascadingValue(const std::string& globalSection, 
		const ConfigurationFile& localConfig, const std::string& localSection,
		const std::string& keyname, const T& defaultValue = T())
{
	T rv = defaultValue;

	rv = localConfig.get<T>(globalSection, keyname, rv);
	rv = localConfig.get<T>(localSection, keyname, rv);

	return rv;
}

Target::Target(std::string section, const ConfigurationFile& config):
	name(section),
	globalSection("global"),
	includes(StringUtils::split(getCascadingValue<std::string>(
					globalSection, config, section, "Include"), '\n')),
	excludes(StringUtils::split(getCascadingValue<std::string>(
					globalSection, config, section, "Exclude"), '\n')),
	priority(getCascadingValue<unsigned int>(
				globalSection, config, section, "Priority")),
	minBackups(getCascadingValue<unsigned int>(
				globalSection, config, section, "MinBackups", 1)),
	maxBackups(getCascadingValue<unsigned int>(
				globalSection,
				config,
				section,
				"MaxBackups",
				std::numeric_limits<unsigned int>::max())),
	preferredOrder(getCascadingValue<std::string>(
				globalSection, config, section, "PreferredOrder", "this, other")),
	distribution(getCascadingValue<std::string>(
				globalSection, config, section, "Distribution", "focus"))
{
}

void Target::start()
{
	LOGI("Starting target " + name);
}
