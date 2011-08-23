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

#ifndef EXCLUSION_RULE_H
#define EXCLUSION_RULE_H

#include <list>
#include "Poco/File.h"
#include "ExclusionSubRule.h"

class ExclusionRule
{
public:
	ExclusionRule(const ExclusionRule& rule);
	ExclusionRule();
	virtual ~ExclusionRule();

	bool match(const Poco::File& inputFile) const;

	/**
	 * When adding an ExclusionSubRule, it is owned by the ExclusionRule from
	 * that moment on and will thus be freed by it. 
	 */
	void addSubRule(ExclusionSubRule* sr);

	
private:
	std::list<ExclusionSubRule*> subRules;
};

#endif
