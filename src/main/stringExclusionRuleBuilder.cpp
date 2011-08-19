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

#include "stringExclusionRuleBuilder.h"
#include "pathGlobExclusionSubRule.h"
#include "fileGlobExclusionSubRule.h"
#include "pathExclusionSubRule.h"
#include "excludeStringParser.h"

ExclusionRule StringExclusionRuleBuilder::build(const std::string& source)
{
	ExclusionRule rule;

	typedef std::list<std::string> StringList;
	StringList parsed = ExcludeStringParser::parse(source);

	enum Type { STRING, AND, NOT, NONE };
	Type prev = NONE;
	bool negated = false;

	for(StringList::const_iterator it = parsed.begin();
			it != parsed.end();
			++it)
	{
		const std::string& subject = *it;
		if(subject.find('*') != std::string::npos || subject.find('?') != std::string::npos)
		{
			// Probable glob found ... (no problem if it turns
			// out this wasn't a glob; except for the performance hit
			// obviously)
			if(isPath(subject))
			{
				rule.addSubRule(new PathGlobExclusionSubRule(subject, negated));
			}
			else
			{
				// If the glob is not a path glob, it's a file glob
				rule.addSubRule(new FileGlobExclusionSubRule(subject, negated));
			}
		}
		else
		{
			// No signs this is a glob, interpret as path
			rule.addSubRule(new PathExclusionSubRule(subject, negated));
		}
	}
	return rule;
}

bool StringExclusionRuleBuilder::isPath(std::string s)
{
	return s.find_first_of("/\\") != std::string::npos;
}
