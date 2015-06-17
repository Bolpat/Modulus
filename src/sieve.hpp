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

#include <iostream>

#include <vector>
#include <list>
#include <unordered_map>

#include <functional>

namespace Modulus
{

using std::string;

using std::cerr;
using std::cout;
using std::cin;
using std::endl;

using std::ostringstream;
using std::istringstream;

using std::vector;
using std::list;
using std::unordered_map;

[[ noreturn ]]
void ERROR(string msg) { cerr << "Error: " << msg << endl; exit(1); }

template <typename T, typename... Ts> [[ noreturn ]]
void ERROR(string msg, T arg, Ts... args)
{
    ostringstream os(msg);
    os << arg;
    ERROR(os.str(), args...);
}


// Returns the vector of non-trivial additive decompositions of the given number.
// A positive integer number n can be written as a sum of positive values.
// Every decomposition is stored as vector of addends.
// The decomposition of n as just n is the trivial decomposition.
// The function only returns different decompositions respecting that permutations of addends provide the same decomposition.
// The only decomposition of zero is the empty vector of addends.
vector<vector<unsigned>> decomp(unsigned n)
{
    // (Haskell with ViewPatterns)
    // deComp :: Integer -> [[Integer]]
    // deComp x = d x x where
    //    d 0  _           = [[]]
    //    d n (min n -> m) = [ i : dec | i <- [1 .. m], dec <- d (n - i) i ]
    //
    using vvu = vector<vector<unsigned>>;
    std::function<vvu(unsigned, unsigned)> d = 
        [&d] (unsigned n, unsigned limit)
        {
            if (n == 0) return vvu(1);

            vvu result;
            unsigned m = std::min(n, limit);
            for (unsigned i = 1; i <= m; ++i)
            for (auto & dec : d(n - i, i)) // recursion
            {
                dec.push_back(i);
                result.push_back(dec);
            }
            return result;
        };

    auto dec = d(n, n);
    dec.pop_back(); // the last one is always n itself. We don't need it, and it is IMPORTANT to drop it.
    return dec;
}



// Calculates the irreducible Polynomials of (Z/pZ)[x] with degree up to n.
// Return type is vector<list<KPoly>>.
template<unsigned p>
auto getPolynomials(unsigned n)
{
    using K     = Z<p>;
    using KPoly = Polynomial<K>;

    // For each degree there is a list of polynomials.
    // First we generate all and then eliminate the reducible ones.
    // The reducible polynomials have a factorization of lower degree irreducible polynomials.
    // polys[d] will end up as the list with all the irreducible polynomials of degree d.
    vector<list<KPoly>> polys(n);
    using Iterator = decltype(polys.front().begin());

    // Like a p-aric number increment. If the p-aric digits are interpreted as coeffs, we'll get all polynomials of the degree <= size of the vector.
    // Note, that the "p-ary digits" have reverse order: The lower digits are first.
    auto increment = [](vector<K> & coeffs) -> vector<K> &
        {
            for (auto & coeff : coeffs) if (++coeff != 0) break;
            return coeffs;
        };

    polys[0].push_back(KPoly(1)); // Generate the polynomials of degree 0.
    // coeffs is the vector of the NON-TRIVIAL coefficients the polynomial is being built of.
    // non-trivial in this case means, that the leading coefficient is not saved (zero coefficients indeed are).
    vector<K> coeffs(n-1), zerocoeffs(n-1);
    do
    {
        KPoly poly = KPoly::fromCoeffVector(coeffs);
        for (unsigned d = deg(poly) + 1; d < n; ++d)
            polys[d].push_back(poly.with_monic(d));
    }
    while (increment(coeffs) != zerocoeffs);

    for (unsigned k = 2; k < n; ++k) // k is the degree of the polynomials we want to eliminate. (remember: max degree == n-1)
    {
        for (auto const & dc : decomp(k))
        {
            vector<Iterator> begs, itrs, ends;
            begs.reserve(dc.size());
            itrs.reserve(dc.size());
            ends.reserve(dc.size());
            for (unsigned d : dc)
            {
                begs.push_back(polys[d].begin());
                itrs.push_back(polys[d].begin());
                ends.push_back(polys[d].end()  );
            }

            do
            {
                KPoly prod = K(1);
                for (auto & it : itrs) prod *= *it;
                polys[k].remove(prod);
            }
            while (iterator_multi_increment_delta(itrs, begs, ends));
        }
    }
    return polys;
}

// For given n it returns the unordered_map which any polynomial of (Z/pZ)[x] which is reducible is mapped on the canonical decomposition
// of irreducible polynomials. The return type is unordered_map<KPoly, vector<KPoly>>, but is unnecessary complex to read since KPoly is defined inside.
// That means especially that deg(f) <= n implies:
// let  decomp = getPolynomialsDecomposition(n);
//      result = decomp[f];
// result.second is the vector which contains the prime factors of the factorization of f.
// if result is decomp.end() then f is irreducible.
template<unsigned p>
auto getPolynomialsDecomposition(unsigned n)
{
    using K      = Z<p>;
    using KPoly  = Polynomial<K>;
    unordered_map<KPoly, vector<KPoly>> result; // [[!] added line]

    // For each degree there is a list of polynomials.
    // First we generate all and then eliminate the reducible ones.
    // The reducible polynomials have a factorization of lower degree irreducible polynomials.
    // polys[d] will end up as the list with all the irreducible polynomials of degree d.
    vector<list<KPoly>> polys(n);
    using Iterator = decltype(polys.front().begin());

    // Like a p-ary number increment. If the p-ary digits are interpreted as coeffs, we'll get all polynomials of the degree <= size of the vector.
    // Note, that the "p-ary digits" have reverse order: The lower digits are first.
    auto increment = [](vector<K> & coeffs) -> vector<K> &
        {
            for (auto & coeff : coeffs) if (++coeff != 0) break;
            return coeffs;
        };

    auto dereference_vector = [](vector<Iterator> const & its) // [[!] added function]
        {
            vector<KPoly> res; res.reserve(its.size());
            for (auto & it : its) res.push_back(*it);
            return res;
        };

    polys[0].push_back(KPoly(1)); // Generate the polynomials of degree 0.
    // coeffs is the vector of the NON-TRIVIAL coefficients the polynomial is being built of.
    // non-trivial in this case means, that the leading coefficient is not saved (zero coefficients indeed are).
    vector<K> coeffs(n-1), zerocoeffs(n-1);
    do
    {
        KPoly poly = KPoly::fromCoeffVector(coeffs);
        for (unsigned d = deg(poly) + 1; d < n; ++d)
            polys[d].push_back(poly.with_monic(d));
    }
    while (increment(coeffs) != zerocoeffs);

    for (unsigned k = 2; k < n; ++k) // k is the degree of the polynomials we want to eliminate. (remember: max degree == n-1)
    {
        for (auto const & dc : decomp(k))
        {
            vector<Iterator> begs, itrs, ends;
            begs.reserve(dc.size());
            itrs.reserve(dc.size());
            ends.reserve(dc.size());
            for (unsigned d : dc)
            {
                begs.push_back(polys[d].begin());
                itrs.push_back(polys[d].begin());
                ends.push_back(polys[d].end()  );
            }

            do
            {
                KPoly prod = K(1);
                for (auto & it : itrs) prod *= *it;
                polys[k].remove(prod);
                result[std::move(prod)] = dereference_vector(itrs); // [[!] added line]
            }
            while (iterator_multi_increment_delta(itrs, begs, ends));
        }
    }
    return result; // [[!] polys --> result]
}



template<unsigned p>
void printPolynomials(unsigned n, std::ostream & out)
{
    auto polys = getPolynomials<p>(n + 1);
    unsigned total_count = 0;
    for (auto & deg_d_polys : polys) total_count += deg_d_polys.size();

    out << "Irreducible Polynomials Modulus " << p << " of degree up to " << n << " (" << total_count << "):" << endl;
    for (unsigned d = 0; d < polys.size(); ++d)
    {
        out << "Degree " << d << " (" << polys[d].size() << "):" << endl;
        for (auto & poly : polys[d]) out << poly << endl;
        out << endl;
    }
}


template <unsigned p>
void testPolynomials(char** argv, std::ostream & out)
{
    using KPoly = Polynomial<Z<p>>;
    
    if (*argv == nullptr) return;

    vector<KPoly> inputs;
    unsigned      max_deg = 0;
    do
    {
        istringstream iss(*argv);
        KPoly         inp;
        if (not(iss >> inp)) ERROR("polynomial '", *argv, "' not well formed.");
        if (max_deg < deg(inp)) max_deg = deg(inp);
        inputs.push_back(inp);
    }
    while (*(++argv) != nullptr);

    auto decompositions = getPolynomialsDecomposition<p>(max_deg + 1);
    for (auto & input : inputs)
    {
        auto dec = decompositions.find(input);
        if (dec == decompositions.end()) out << input << " is irreducible." << endl;
        else                             out << input << "  =  (" << contnr_str(dec->second, ") * (") << ")" << endl;
    }
}

} // namespace Modulus
