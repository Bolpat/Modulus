#pragma once

// Compile with clang++-3.5 -std=c++14

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

template <typename T> inline
std::string to_str(T const & t)
{
    return (std::ostringstream() << t).str();
}

template <typename T> inline
T from_str(std::string const & str)
{
    T t;
    std::istringstream(str) >> t;
    return t;
}

template <typename T> inline
std::ostringstream && concat_helper(std::ostringstream && o, T const & t)
{
    o << t;
    return std::move(o);
}

template <typename T, typename... Ts> inline
std::ostringstream && concat_helper(std::ostringstream && o, T const & t, Ts... ts)
{
    o << t;
    return concat_helper(std::move(o), ts...);
}

template <typename... Ts> inline
std::string concat(Ts... ts)
{
    return concat_helper<Ts...>(std::ostringstream(), ts...).str();
}

bool choice_yn(std::string const & msg)
{
    char answ;
  begin:
    std::cout << msg << " (y/n)";
    std::cin >> answ;
    if (answ == 'y') return true;
    if (answ == 'n') return false;
    std::cout << "Type in 'y' for yes, or 'n' for no.";
    goto begin;
}
