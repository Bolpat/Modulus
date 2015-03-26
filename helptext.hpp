#pragma once

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

namespace Modulus
{
const string HELP_TEXT =
    "Supported command line parameters:                                                                         \n"
    " (0a) (no params)                                                                                          \n"
    " (0b) -?                                                                                                   \n"
    " (0c) -h                                                                                                   \n"
    " (0d) --help                                                                                               \n"
    "Show this.                                                                                                 \n"
    "                                                                                                           \n"
    " (1a) --file                                                                                               \n"
    " (1b) --output                                                                                             \n"
    " (1c) -o                                                                                                   \n"
    "Specify a file to print the output to:                                                                     \n"
    "parameters: filename                                                                                       \n"
    "filename denotes some valid file name                                                                      \n"
    " If no file is specified, all output is printed on the console.                                            \n"
    "Example usage: --file \"irrPoly.txt\"                                                                      \n"
    "                                                                                                           \n"
    " (2a) -l                                                                                                   \n"
    " (2b) --list                                                                                               \n"
    "List irreducible polynomials:                                                                              \n"
    "parameters: ds [ps=2-20]                                                                                   \n"
    "ds is the list of degrees,                                                                                 \n"
    " up to which irreducible (Z/pZ)[x] polynomials of corresponding p are generated.                           \n"
    " It must have one of these formats:                                                                        \n"
    "   d1,d2,d3 (explicit list)                                                                                \n"
    "   d        (one for all)                                                                                  \n"
    "ps is the optional list of primes < 20 (others ignored).                                                   \n"
    " It must have one of these formats:                                                                        \n"
    "   p1,p2,p3 (explicit list)                                                                                \n"
    "   p-q      (explicit range)                                                                               \n"
    "    -p      (up to p)                                                                                      \n"
    "   p-       (like p-20)                                                                                    \n"
    "Example usage: -l  10,7,3  2,3,7                                                                           \n"
    " Lists all irr. Polynomials of (Z/2Z)[x] whose degree is <= 10,                                            \n"
    "                               (Z/3Z)[x]                     7, and                                        \n"
    "                               (Z/7Z)[x]                     3                                             \n"
    "Example usage: --list  3  11-17                                                                            \n"
    " Lists all irr. Polynomials of (Z/11Z)[x], (Z/13Z)[x] and (Z/17Z)[x] whose degree is <= 3.                 \n"
    "                                                                                                           \n"
    " (2c) -t                                                                                                   \n"
    " (2d) -test                                                                                                \n"
    "Test the given polynomials if they are irreducible.                                                        \n"
    "parameters: p polylist                                                                                     \n"
    "p must be a prime number and < 20.                                                                         \n"
    "polylist is a list of any polynomials of (Z/pZ)[x], separated with spaces.                                 \n"
    " Every monomial must have the format ax^d, x^d, ax or a, where a is of 0 ... p-1, d some natural number.   \n"
    "  For p = 2, the only format allowed is x^d and explicitly x and 1.                                        \n"
    " Every polynomial then is a concatination of these monomials with + or - between.                          \n"
    " You will be asked wether the interpretation of the polynomial is correct.                                 \n"
    "If the polynomial is reduclible, there will be given a decomposition,                                      \n"
    "else it will be labeled as irreducible.                                                                    \n"
    "Example usage: -t  2  x^2+x+1  x^2+1                                                                       \n"
    "Example usage: -t  7  x^4+3x^2+6  x^4+2x^3+3x+4                                                            \n"
    "                                                                                                           \n"
    "                                                                                                           \n"
    "The command line parameters may be mixed, but only one of (1) and (2) allowed; and (1) must be first.      \n";
}
