/*
 * Copyright (C) 2011  Ives van der Flaas
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

	std::vector<std::string> fileGlobExcludes;
	std::vector<std::string> pathGlobExcludes;
	std::set<std::string> pathExcludes;

	void backupPath(const Poco::File& path) const;
	bool isPath(std::string s) const;
public:
	Target(std::string sectionName, const ConfigurationFile& config);

	void start();
};

#endif
