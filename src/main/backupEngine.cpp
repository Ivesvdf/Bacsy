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

#include <list>
#include <functional>
#include <fstream>
#include <algorithm>
#include "backupEngine.h"

class TargetNameToTargetter
{
public:
	TargetNameToTargetter(const CascadingFileConfiguration& configuration): configuration(configuration) {}
	Target* operator()(const std::string& targetName)
	{
		return new Target(targetName, configuration);
	}

private:
	const CascadingFileConfiguration& configuration;
};

std::vector<Target*> targetNamesToTargets(
		const std::list<std::string>& targetStrings,
		const CascadingFileConfiguration& configuration)
{
	std::vector<Target*> targets;
	targets.resize(targetStrings.size());


	TargetNameToTargetter targetNameToTarget(configuration); 

	std::transform(
			targetStrings.begin(),
			targetStrings.end(),
			targets.begin(),
			targetNameToTarget);
			

	return targets;
}

BackupEngine::BackupEngine(const CascadingFileConfiguration& configuration):
	configuration(configuration),
	targets(targetNamesToTargets(configuration.getTargets(), configuration))
{
}

BackupEngine::~BackupEngine()
{
	for(std::vector<Target*>::iterator it = targets.begin();
			it != targets.end();
			it++)
	{
		delete *it; 
	}
}

void BackupEngine::start()
{
	for(std::vector<Target*>::iterator it = targets.begin();
			it != targets.end();
			it++)
	{
		(*it)->start();
	}
}
