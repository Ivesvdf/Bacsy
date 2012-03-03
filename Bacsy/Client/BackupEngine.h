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

#ifndef BACSY_BACKUP_ENGINE_H
#define BACSY_BACKUP_ENGINE_H

#include <vector>
#include "Poco/ThreadPool.h"
#include "Bacsy/Client/Source.h"
#include "Bacsy/Client/CascadingSourceConfiguration.h"
#include "Bacsy/Client/PreviousRunRecordFactory.h"

namespace Bacsy
{
namespace Client
{

using namespace Common;
using namespace Rules;

class BackupEngine
{
	private:
		const CascadingSourceConfiguration& configuration;
		std::vector<Source*> sources;
		Poco::ThreadPool threadPool;
		PreviousRunRecordFactory& previousRunRecordFactory;

	public:
		BackupEngine(const CascadingSourceConfiguration& configuration,
				PreviousRunRecordFactory& previousRunRecordFactory);
		~BackupEngine();

		void start();
		void joinAll();
};

}
}
#endif
