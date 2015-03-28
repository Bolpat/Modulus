// Compile with clang++-3.5 -std=c++14 -o "../bin/Z_test" Z.cpp

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

// This file is for testing the standalone header "/src/Z.hpp", and explicitely NOT the corresponding *.cpp file.

#include <iostream>

#include "../src/Z.hpp"

using namespace std;
using namespace Modulus;

int main()
{
    constexpr unsigned p = 17;

    Z<p> f, g, h;
    cout <<  "Three Values (f, g, h) of Z/" << p << "Z: " << endl;
    cin >> f >> g >> h;
    
    cout << "In Z/" << p << "Z, f  =  " << f << ", g  =  " << g << ", h  =  " << h << endl;
    
    cout << "f + g  =  " << (f + g) << endl;
    cout << "f - g  =  " << (f - g) << endl;
    
    cout << "f * g  =  " << (f * g) << endl;
    cout << "f / g  =  " << (f / g) << endl;
    
    cout << "After setting f  :=  f + g, now f  =  " << (f += g) << endl;
    cout << "After setting g  :=  g - h, now g  =  " << (g -= h) << endl;
    cout << "After setting h  :=  h * f, now h  =  " << (h *= f) << endl;
    cout << "After setting f  :=  f / g, now f  =  " << (f /= g) << endl;
    
    cout << endl;
    
    //~ Z<2> a, b, c;
    //~ cout <<  "Three Values (a, b, c) of Z/2Z:" << endl;
    //~ cin >> a >> b >> c;
    //~ 
    //~ cout << "a + b  =  " << (a + b) << endl;
    //~ cout << "a - b  =  " << (a - b) << endl;
    //~ 
    //~ cout << "a * b  =  " << (a * b) << endl;
    //~ cout << "a / b  =  " << (a / b) << endl;
    //~ 
    //~ cout << "After setting a  :=  a + b, now a  =  " << (f += g) << endl;
    //~ cout << "After setting b  :=  b - c, now b  =  " << (g -= h) << endl;
    //~ cout << "After setting c  :=  c * a, now c  =  " << (h *= f) << endl;
    //~ cout << "After setting a  :=  a / b, now a  =  " << (f /= g) << endl;
    
    cout << "Finished." << endl << endl;
    
    return 0;
}
