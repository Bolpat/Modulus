#pragma once

// Compile with clang++-3.5 -std=c++14

// There is no container.cpp file as it is not needed.

/* This file is part of Modulus.
 * 
 * Modulus is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <sstream>

namespace Modulus
{

using std::string;
using std::ostringstream;

template <typename Contnr>
string contnr_str(Contnr const & cs,
                  string const & separator = ", ")
{
    ostringstream os;
    os << '[';
    if (not cs.empty())
    {
        auto it = cs.begin();
        os << *it;
        while (++it != cs.end()) os << separator << *it;
    }
    os << ']';
    return os.str();
}

template <typename Map>
string map_string(Map const & m,
                  string const & separator1 = ",",
                  string const & separator2 = ", ")
{
    ostringstream os;
    os << '[';
    if (not m.empty())
    {
        auto it = m.begin();
        os << '(' << it->first << separator1 << it->second << ')';
        while (++it != m.end()) os << separator2 << '(' << it->first << separator1 << it->second << ')';
    }
    os << ']';
    return os.str();
}

// Increments the vector of iterators in reverse p-adic style.
// On overflow (all iterators match their corresponding beginning) the function returns false, in any other case the result is true.
template<typename Iterator>
bool iterator_multi_increment(vector<Iterator>       & itrs,
                              vector<Iterator> const & begs,
                              vector<Iterator> const & ends)
{
    for (unsigned i = 0; i < itrs.size(); ++i)
    {
        if (++itrs[i] != ends[i]) return true;
        itrs[i] = begs[i];
    }
    // if we come here, we know: every iterator has been reset.
    // therefore we signalize no further iterating.
    return false;
}

// Increments the vector of iterators in reverse p-aric style, but respecting equalness of startpoints and commutativity of pointed values.
template<typename Iterator>
bool iterator_multi_increment_delta(vector<Iterator>       & itrs,
                                    vector<Iterator> const & begs,
                                    vector<Iterator> const & ends)
{
    // Here we use that the degree of the polynomials any of itrs is pointing at is fixed and that for i = 0, ..., n, deg(*(itrs[i])) is weakly increasing.
    // Therefore, and because multimplcation is commutative on polynomials over fields, we can take the previous iterator as beginning iff they iterate the same vector.
    for (unsigned i = 0; i < itrs.size(); ++i)
    {
        if (++itrs[i] != ends[i])
        {
            size_t j = 0;
            while (i > j and begs[i - ++j] == begs[i]) itrs[i-j] = itrs[i];
            return true;
        }
        itrs[i] = begs[i];
    }
    // if we come here, we know: every iterator has been reset.
    // therefore we signalize no further iterating.
    return false;
}

} // namespace Modulus
