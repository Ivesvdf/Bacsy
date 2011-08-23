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
#include "BackupEngine.h"

class SourceNameToSourceter
{
public:
	SourceNameToSourceter(const CascadingFileConfiguration& configuration): configuration(configuration) {}
	Source* operator()(const std::string& sourceName)
	{
		return new Source(sourceName, configuration);
	}

private:
	const CascadingFileConfiguration& configuration;
};

std::vector<Source*> sourceNamesToSources(
		const std::list<std::string>& sourceStrings,
		const CascadingFileConfiguration& configuration)
{
	std::vector<Source*> sources;
	sources.resize(sourceStrings.size());


	SourceNameToSourceter sourceNameToSource(configuration); 

	std::transform(
			sourceStrings.begin(),
			sourceStrings.end(),
			sources.begin(),
			sourceNameToSource);
			

	return sources;
}

BackupEngine::BackupEngine(const CascadingFileConfiguration& configuration):
	configuration(configuration),
	sources(sourceNamesToSources(configuration.getSources(), configuration))
{
}

BackupEngine::~BackupEngine()
{
	for(std::vector<Source*>::iterator it = sources.begin();
			it != sources.end();
			it++)
	{
		delete *it; 
	}
}

void BackupEngine::start()
{
	for(std::vector<Source*>::iterator it = sources.begin();
			it != sources.end();
			it++)
	{
		(*it)->start();
	}
}
