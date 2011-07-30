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

#ifndef STORE_H
#define STORE_H

#include <string>
#include "cascadingFileConfiguration.h"

class Store
{
public:
	Store(const std::string storeName, const CascadingFileConfiguration& configuration);

	unsigned int getMinPriorityForStoring() const { return minPriorityForStoring; }
private:
	const std::string storeName;
	const CascadingFileConfiguration& configuration;

	const std::string location;
	const bool alwaysPresent;
	const unsigned int minPriorityForStoring;
};

#endif
