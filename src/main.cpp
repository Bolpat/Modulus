
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
 
#include "Z.hpp"
#include "Polynomial.hpp"
#include "container.hpp"
#include "sieve.hpp"
#include "helptext.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <map>

#include <algorithm>
 
namespace Modulus
{

using std::cout;
using std::cin;
using std::endl;

using std::ostringstream;
using std::istringstream;

using std::ofstream;
using std::ifstream;

using std::vector;
using std::map;

int main2(int argc, char** argv, ostream & out);
int main (int argc, char** argv)
{
    ++argv;
    if (*argv == nullptr or
        string("-?")     == *argv or
        string("-h")     == *argv or
        string("--help") == *argv)
    {
        cout << HELP_TEXT << endl;
        return 0;
    }
    
    if (string("-o")       == *argv or
        string("--file")   == *argv or
        string("--output") == *argv)
    {
        if (*(++argv) == nullptr) ERROR("parameter 'file' missing.");
        ofstream out(*argv, ofstream::trunc);
        if (out.good()) return main2(argc, ++argv, out);
        ERROR("cannot open/write file.");
    }
    
    return main2(argc, argv, cout);
}

int main2(int argc, char** argv, ostream & out)
{
    const vector<unsigned> primes = { 2, 3, 5, 7, 11, 13, 17, 19 };

    using printPolynomials_t = decltype(printPolynomials<2>);
    using testPolynomials_t  = decltype(testPolynomials<2>);
    
    const map< unsigned, printPolynomials_t * > printPolys = 
        {
            {  2, printPolynomials< 2> }//,
            //~ {  3, printPolynomials< 3> },
            //~ {  5, printPolynomials< 5> },
            //~ {  7, printPolynomials< 7> },
            //~ { 11, printPolynomials<11> },
            //~ { 13, printPolynomials<13> },
            //~ { 17, printPolynomials<17> },
            //~ { 19, printPolynomials<19> }
        };
    
    const map < unsigned, testPolynomials_t * > testPolys = 
        {
            {  2, testPolynomials< 2> }//,
            //~ {  3, testPolynomials< 3> },
            //~ {  5, testPolynomials< 5> },
            //~ {  7, testPolynomials< 7> },
            //~ { 11, testPolynomials<11> },
            //~ { 13, testPolynomials<13> },
            //~ { 17, testPolynomials<17> },
            //~ { 19, testPolynomials<19> }
        };
    
    if (*argv == nullptr)
    {
        cout << "Nothing to do." << endl;
        return 0;
    }
    if (string("-l")     == *argv or
        string("--list") == *argv)
    {
        ++argv;
        if (*argv == nullptr) ERROR("parameter 'ds' missing.");

        string inp(*argv);

        vector<unsigned> ds, ps;
        if (inp.find(',') == string::npos)
        {
            unsigned d;
            istringstream iss(inp);
            iss >> d;
            if (not iss) ERROR("positive integer required.");
            ds = vector<unsigned>(primes.size(), d);
        }
        else
        {
            for (auto & c : inp) if (c == ',') c = ' ';
            istringstream iss(inp);
            unsigned d;
            while (iss >> d) ds.push_back(d);
            if (not iss.eof()) ERROR("positive integer(s) required.");
        }

        if (*(++argv) == nullptr) ps = primes;
        else
        {
            inp = string(*argv);
            size_t pos = inp.find('-');
            if (pos != string::npos)
            {
                inp[pos] = ' ';
                istringstream iss(inp);
                unsigned      p = primes.front(),
                              q = primes.back();
                if      (pos == 0)              { if (not(iss >>      q) or p > q) ERROR("parameter 'ps': positive integer >= ", p, " required.");      }
                else if (pos == inp.size() - 1) { if (not(iss >> p     ) or p > q) ERROR("parameter 'ps': positive integer <= ", q, " required.");      }
                else                            { if (not(iss >> p >> q) or p > q) ERROR("parameter 'ps': positive integers must be ascending."); }
                for (; p <= q; ++p) if (binary_search(primes.begin(), primes.end(), p)) ps.push_back(p);
            }
            else
            {
                for (auto & c : inp) if (c == ',') c = ' ';
                istringstream iss(inp);
                unsigned p;
                while (iss >> p) if (binary_search(primes.begin(), primes.end(), p)) ps.push_back(p);
                if (iss.bad()) ERROR("positive integer(s) required.");
            }
        }
        for (auto itd  = ds.begin(),  itp  = ps.begin();
                  itd != ds.end() and itp != ps.end();
                ++itd,              ++itp)
        {
            printPolys.at(*itp)(*itd, out);
        }
        return 0;
    }

    if (string("-t")     == *argv or
        string("--test") == *argv)
    {
        if (*(++argv) == nullptr) ERROR("parameter 'p' missing.");
        unsigned p;
        istringstream iss(*argv);
        if ((iss >> p).bad() or not binary_search(primes.begin(), primes.end(), p)) ERROR("prime number required.");
        testPolys.at(p)(++argv, out);
        return 0;
    }

    cerr << "Command line parameters could not be interpreted.";
    return 0;
}

} // namespace Modulus
