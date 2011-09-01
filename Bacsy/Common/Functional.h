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

#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <functional>

namespace Bacsy
{

template <class S, class A>
class fun1_ref_t : public std::unary_function<A,S>
{
	S (*pmem)(A);
	public:
	explicit fun1_ref_t ( S (*p)(A) ) : pmem (p) {}
	S operator() (A x) const
	{ return (*pmem)(x); }
};

template <class S, class A>
fun1_ref_t<S,A> fun_ref (S (*f)(A))
{ 
	return fun1_ref_t<S,A>(f); 
}

}

#endif
