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

#ifndef CONCURRENT_MAP_H
#define CONCURRENT_MAP_H

#include <map>
#include "Poco/RWLock.h"


template<typename K, typename V>
class ConcurrentMap
{
public:
	void set(const K& key, const V& value)
	{
		lock.writeLock();
		map[key] = value;
		lock.unlock();
	}

	V& get(const K& key)
	{
		lock.readLock();
		typename std::map<K, V>::iterator it = map.find(key);

		if(it != map.end())
		{
			V& rv = it->second;
			lock.unlock();
			return rv;
		}
		else
		{
			lock.unlock();
			lock.writeLock();
			V& rv = map[key];
			lock.unlock();
			return rv;
		}
	}

	size_t count(const K& key)
	{
		lock.readLock();
		const size_t rv = map.count(key);
		lock.unlock();
		return rv;
	}

	void erase(const K& key)
	{
		lock.writeLock();
		map.erase(key);
		lock.unlock();
	}

	private:
		Poco::RWLock lock;
		std::map<K,V> map;
};

#endif
