#pragma once

// Compile with clang++-3.5 -std=c++14

// There is no Z.cpp file as it is not needed.

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

namespace Modulus
{

// The extended Euclidean algorithm.
// for r  :=  ext_euclid(a, b, u, v) holds:
//    r  =  ggt(a, b);  r  =  a * u  +  b * v;
constexpr
long ext_euclid(long a, long b, long & u, long & v) noexcept
{
    int s = 0, t = 1;
    u = 1;
    v = 0;
    while (b > 0)
    {
        int q = a / b,
            r = a % b;  a = b;  b = r;
        r = u - q * s;  u = s;  s = r;
        r = v - q * t;  v = t;  t = r;
    }
    return a;
}

/// For any p > 0, Z<p> objects represent numbers of Z/pZ. They aim to behave like these.
/// If you divide through some zero divisor, the result will be zero; without exception being thrown.
template<unsigned p>
class Z
{
    static_assert(p > 0, "p must be > 0.");

private:
    /// Stores the value. The class is designed to keep 0 <= x < p.
    unsigned x;

    /// Reduces the signed value modulo p. However, this cannot be inlined in code by the constructor.
    constexpr static
    unsigned reduce(signed n) noexcept
    {
        return (n %= static_cast<signed>(p)) < 0  ?  p + n  :  n;
    }
    
    /// Returns the multimlicative inverse of the Z object.
    /// Those inverses don't exist for zero divisors; those will be mapped to zero.
    friend constexpr
    Z inv(Z z)
    {
        long inverse = 0, dummy = 0;
        if (1 != ext_euclid(z.x, p, inverse, dummy)) return Z(0);
        return Z(static_cast<signed int>(inverse));
    }

    /// Fast unsafe constructor (with blind parameter).
    /// Used when caller can prove that x % p == x.
    constexpr
    Z(unsigned x, bool) : x(x) { }

public:
            constexpr   Z(unsigned n = 0u) : x(n % p) {  }
            constexpr   Z(  signed n     ) : x(reduce(n)) {  }

    friend  constexpr   bool operator ==(Z z1, Z z2) { return z1.x == z2.x; }
    friend  constexpr   bool operator < (Z z1, Z z2) { return z1.x <  z2.x; }
    friend  constexpr   bool operator !=(Z z1, Z z2) { return z1.x != z2.x; }

            constexpr   Z    operator + () const { return Z(x,     true); }
            constexpr   Z    operator - () const { return Z(p - x, true); }

    friend  constexpr    Z    operator + (Z z1, Z z2) { return Z(z1.x + z2.x); }
    friend  constexpr    Z    operator - (Z z1, Z z2) { return z1 + (-z2); }
    friend  constexpr    Z    operator * (Z z1, Z z2) { return Z(z1.x * z2.x); }
    friend  constexpr    Z    operator / (Z z1, Z z2) { return z1 * inv(z2); }

            constexpr    Z &  operator +=(Z const & z) { return *this = *this + z; }
            constexpr    Z &  operator -=(Z const & z) { return *this = *this - z; }
            constexpr    Z &  operator *=(Z const & z) { return *this = *this * z; }
            constexpr    Z &  operator /=(Z const & z) { return *this = *this / z; }

            constexpr    Z    operator ++(   ) { if (++x == p) x = 0;   return *this; }
            constexpr    Z    operator --(   ) { if (x-- == 0) x = p-1; return *this; }
            constexpr    Z    operator ++(int) { Z res = *this; ++(*this); return res; }
            constexpr    Z    operator --(int) { Z res = *this; --(*this); return res; }

    friend     std::ostream & operator <<(std::ostream & os, Z const & z) { return os << z.x; }
    friend     std::istream & operator >>(std::istream & is, Z       & z) { signed int x;  is >> x;  z = Z(x);  return is; }

    constexpr explicit operator unsigned() const { return x; }
};

template<>
class Z<2>
{
private:
    unsigned x;

    /// Fast unsafe constructor (with blind parameter).
    /// Used when caller can prove that x & 1 == x.
    constexpr Z(unsigned n, bool) : x(n) {  }

public:
    constexpr Z(unsigned n = 0u) : x(n & 1u) {  }
    constexpr Z(  signed n)      : x(n & 1 ) {  }

    friend  constexpr   bool operator ==(Z z1, Z z2) { return z1.x == z2.x; }
    friend  constexpr   bool operator < (Z z1, Z z2) { return not z1.x and z2.x; }
    friend  constexpr   bool operator !=(Z z1, Z z2) { return z1.x != z2.x; }

            constexpr   Z    operator + () const { return Z(x, true); }
            constexpr   Z    operator - () const { return Z(x, true); }

    friend  constexpr   Z    operator + (Z z1, Z z2) { return Z(z1.x ^ z2.x, true); }
    friend  constexpr   Z    operator - (Z z1, Z z2) { return z1 + z2; }
    friend  constexpr   Z    operator * (Z z1, Z z2) { return Z(z1.x & z2.x, true); }
    friend  constexpr   Z    operator / (Z z1, Z z2) { return z1 * z2; }

            constexpr   Z &  operator +=(Z const & z) { return *this = *this + z; }
            constexpr   Z &  operator -=(Z const & z) { return *this = *this - z; }
            constexpr   Z &  operator *=(Z const & z) { return *this = *this * z; }
            constexpr   Z &  operator /=(Z const & z) { return *this = *this / z; }

            constexpr   Z    operator ++(   ) { return x ^= 1u; }
            constexpr   Z    operator --(   ) { return ++x; }
            constexpr   Z    operator ++(int) { Z res = *this; ++(*this); return res; }
            constexpr   Z    operator --(int) { Z res = *this; --(*this); return res; }

    friend    std::ostream & operator <<(std::ostream & os, Z const & z) { return os << z.x; }
    friend    std::istream & operator >>(std::istream & is, Z       & z) { signed int x;  is >> x;  z = Z(x);  return is; }

    constexpr explicit operator unsigned() const noexcept { return x; }
};

} // namespace Modulus

namespace std
{

template<unsigned p>
struct hash<Modulus::Z<p>>
{
    size_t operator()(Modulus::Z<p> const & z) const noexcept
    {
        return std::hash<unsigned>() ( static_cast<unsigned>(z) );
    }
};

} // namespace std
