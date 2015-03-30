// Compile with clang++-3.5 -std=c++14 -o "../bin/Poly_test" Polynomial.cpp

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
#include "../src/Polynomial.hpp"
#include "../src/utility.hpp"

using namespace Modulus;
using namespace std;

namespace
{
template <typename T>
string const str = "unknown";

template <unsigned p>
string const str<Z<p>> = concat("Z/", p, "Z");

template <unsigned p>
string const str<ZPoly<p>> = concat("(Z/", p, "Z)[x]");

template <>
string const str<Polynomial<unsigned>> = "Z[x]";
}

int main()
{
    using K    = Z<2>;
    using Poly = Polynomial<K>;
    constexpr auto X = Poly::X;
    
    cout << "Testing " << str<Poly> << " class." << endl;
    
    Poly P = (X^2) + X + K(1),
         S =         X + K(1);
    
    bool const ch = choice_yn("Use default values?");
    
    if (not ch)
    {
        cout << "Type in two polynomials P and S." << endl;
        // because !!cin is different from (bool)cin.
        if (cin >> P >> S) { } else { cout << "Error in input." << endl; return 0; }
    }
    
    cout << "P  =  " << P;  if (ch) cout << ";   should be x^2 + x + 1";  cout << endl;
    cout << "S  =  " << S;  if (ch) cout << ";   should be x + 1";  cout << endl;
    
    cout << "deg(P)  =  " << deg(P);  if (ch) cout << ";   should be 2";  cout << endl;
    cout << "deg(S)  =  " << deg(S);  if (ch) cout << ";   should be 1";  cout << endl;
    
    cout << "Is P == S ?  " << (P == S ? "yes" : "no");  if (ch) cout << ";   should be no";  cout << endl;
    cout << "Is P != S ?  " << (P != S ? "yes" : "no");  if (ch) cout << ";   should be yes";  cout << endl;
    
    cout << "P + S  =  " << P + S;  if (ch) cout << ";   should be x^2 + 2x + 2";  cout << endl;
    cout << "P - S  =  " << P - S;  if (ch) cout << ";   should be x^2";  cout << endl;
    cout << "P * S  =  " << P * S;  if (ch) cout << ";   should be x^3 + 2x^2 + 2x + 1";  cout << endl;
    
    if (S.is_zero())
    {
        cout << "Cannot test division: Divisor is 0." << endl;
    }
    else
    {
        cout << "let Q, R be given by P  =  Q*S + R, with deg(R) < deg(S)." << endl;
        Poly Q, R; Poly::divmod(P, S, Q, R);
        cout << "Q  =  " << Q;  if (ch) cout << ";   should be x";  cout << endl;
        cout << "R  =  " << R;  if (ch) cout << ";   should be 1";  cout << endl;
    }
    /*
    constexpr unsigned p = 3;
    using Modulus::Eks::X;
    using KPoly = ZPoly<p>;
    
    cout << "Testing " << str<KPoly> << " class." << endl;
    cout << "Type in two polynomials P and S." << endl;
    if (p == 2) cout << "You cannot use coefficients, except 0 and 1 for constant polynomials! Just x^2 and not 1x^2." << endl;
    
    KPoly P, S;
    // because !!cin is different from (bool)cin.
    if (cin >> P >> S) { } else { cout << "Error in input." << endl; return 0; }
        
    cout << "deg(P)  =  " << deg(P) << endl;
    cout << "deg(S)  =  " << deg(S) << endl;
    
    cout << "P == S ?  ->  " << (P == S ? "yes" : "no") << endl;
    cout << "P != S ?  ->  " << (P != S ? "yes" : "no") << endl;
    
    cout << "P + S  =  " << P + S << endl;
    cout << "P - S  =  " << P - S << endl;
    cout << "P * S  =  " << P * S << endl;
    KPoly Q, R; KPoly::divmod(P, R, Q, R);
    cout << "let Q, R be given by P  =  Q*S + R, with deg(R) < deg(S), then Q  =  " << Q << " and R  =  " << R << endl;
    */
    return 0;
}
