#ifndef TARGET_H
#define TARGET_H

#include <string>
#include <set>
#include "Poco/File.h"
#include "configurationFile.h"

class Target
{
private:
	const std::string name;
	const std::string globalSection;
	const std::vector<std::string> includes;
	const std::vector<std::string> excludes;
	const unsigned int priority;
	const unsigned int minBackups;
	const unsigned int maxBackups;
	const std::string preferredOrder;
	const std::string distribution;

	std::vector<std::string> globExcludes;
	std::set<std::string> pathExcludes;

	void backupPath(const Poco::File& path) const;
public:
	Target(std::string sectionName, const ConfigurationFile& config);

	void start();
};

#endif
