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

#include "Poco/Ascii.h"
#include "Bacsy/Common/StringUtils.h"
#include "woodcutter/woodcutter.h"
#include "Bacsy/Rules/StringExclusionRuleBuilder.h"
#include "Bacsy/Rules/PathGlobExclusionSubRule.h"
#include "Bacsy/Rules/SizeExclusionSubRule.h"
#include "Bacsy/Rules/FileGlobExclusionSubRule.h"
#include "Bacsy/Rules/PathExclusionSubRule.h"
#include "Bacsy/Rules/ExclusionStringParser.h"

namespace Bacsy
{
namespace Rules
{

using namespace Common;


ExclusionRule StringExclusionRuleBuilder::build(const std::string& source)
{
	ExclusionRule rule;

	typedef std::list<std::string> StringList;
	StringList parsed = ExclusionStringParser::parse(source);

	bool negated = false;

	for(StringList::const_iterator it = parsed.begin();
			it != parsed.end();
			++it)
	{
		const std::string& subject = *it;

		if(subject == "&")
		{
			// Do nothing
		}
		else if(subject == "!")
		{
			negated = !negated;
			continue;
		}
		// Test for size subrule
		else if(subject.size() > 3 && (subject[0] == '>' || subject[0] == '<'))
		{
			const char theOperator = subject[0];
			const char possiblePrefix = subject[subject.length()-2];

			using Poco::Ascii;

			const char prefix = Poco::Ascii::isDigit(possiblePrefix) ? 0 : possiblePrefix;
			const std::string numberString = subject.substr(1, subject.length()-2 - (prefix ? 1 : 0));
			const Poco::File::FileSize sizeInUnit = StringUtils::fromString<size_t>(numberString);

			if(!Poco::Ascii::isAlpha(prefix) && prefix != 0)
			{
				LOGE("Invalid prefix in size subrule : " + subject);
				continue;
			}

			if(subject[subject.length()-1] != 'B')
			{
				LOGE("Invalid unit in subrule " + subject + ", only B is supported.");
				continue;
			}

			Poco::File::FileSize sizeInBytes = -1;
			switch(prefix)
			{
				case 0:
					sizeInBytes = sizeInUnit;
					break;
				case 'k':
					sizeInBytes = sizeInUnit * 1024;
					break;
				case 'M':
					sizeInBytes = sizeInUnit * 1024 * 1024;
					break;
				case 'G':
					sizeInBytes = sizeInUnit * 1024*1024*1024;
					break;
				default:
					sizeInBytes = 0;
					LOGE(std::string("Unrecognized prefix ") + prefix + " in subrule " + subject);
					continue;
			}


			rule.addSubRule(new SizeExclusionSubRule(sizeInBytes,
						theOperator == '>' ?
							SizeExclusionSubRule::GREATER_THAN :
							SizeExclusionSubRule::LESS_THAN,
						negated));
		}
		else if(subject.find('*') != std::string::npos || subject.find('?') != std::string::npos)
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

		negated = false;
	}
	return rule;
}

bool StringExclusionRuleBuilder::isPath(std::string s)
{
	return s.find_first_of("/\\") != std::string::npos;
}

}
}
