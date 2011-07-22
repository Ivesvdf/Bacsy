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
#include <fstream>
#include "backupEngine.h"

BackupEngine::BackupEngine()
{
	std::ifstream filestream(".bacsy/targets.config");
	ConfigurationFile configFile(filestream);

	std::list<std::string> targetNames = configFile.sections();
	for(std::list<std::string>::const_iterator it = targetNames.begin();
			it != targetNames.end();
			it++)
	{
		if(*it != "global")
			targets.push_back(new Target(*it, configFile));
	}
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
