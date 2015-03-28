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

using namespace Modulus;
using namespace std;

int main()
{
    constexpr unsigned p = 3;
    using Modulus::Eks::X;
    using KPoly = ZPoly<p>;
    
    cout << "Testing (Z/" << p << "Z)[x] class." << endl;
    cout << "Type in two polynomials P and S." << endl;
    if (p == 2) cout << "You cannot use coefficients, except 0 and 1 for constant polynomials! Just x^2 and not 1x^2." << endl;
    
    KPoly P, S;
    if (cin >> P >> S) { } else { cout << "Error in input." << endl; return 0; }
    
    cout << boolalpha;
    
    cout << "deg(P)  =  " << deg(P) << endl;
    cout << "deg(S)  =  " << deg(S) << endl;
    
    cout << "P == S ?  ->  " << (P == S ? "yes" : "no") << endl;
    cout << "P != S ?  ->  " << (P != S ? "yes" : "no") << endl;
    
    cout << "P + S  =  " << P + S << endl;
    cout << "P - S  =  " << P - S << endl;
    cout << "P * S  =  " << P * S << endl;
    KPoly Q, R; KPoly::divmod(P, R, Q, R);
    cout << "let Q, R be given by P  =  Q*S + R, with deg(R) < deg(S), then Q  =  " << Q << " and R  =  " << R << endl;
    
    return 0;
}
