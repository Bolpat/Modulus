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
#include <sstream>

#include <limits>

#include <vector>
#include <map>

#include <algorithm>

//#include "Z.hpp" // for Polynomial<Z2> specialisattion.

namespace Modulus
{
// Forward declaration of the types and non-inline template friend functions.

template <typename T, typename deg_type = size_t>   class Polynomial;

template <typename T, typename deg_type>   deg_type                 deg         (Polynomial<T, deg_type> const &);
template <typename T, typename deg_type>   Polynomial<T, deg_type>  operator +  (Polynomial<T, deg_type> const & p,  Polynomial<T, deg_type> const & q);
template <typename T, typename deg_type>   Polynomial<T, deg_type>  operator *  (Polynomial<T, deg_type> const & p,  Polynomial<T, deg_type> const & q);
template <typename T, typename deg_type>   std::ostream &           operator << (std::ostream &,                     Polynomial<T, deg_type> const &);
template <typename T, typename deg_type>   std::istream &           operator >> (std::istream &,                     Polynomial<T, deg_type>       &);

}



namespace Modulus
{

template <typename T, typename deg_type>
class Polynomial // non-literal type (non-trivial destructor)
{
    static_assert(std::numeric_limits<deg_type>::is_integer and not std::numeric_limits<deg_type>::is_signed,
            "deg_type must be an unsigned integer.");
    
private:
    std::map<deg_type, T> coeffs;
    
public:
    Polynomial() { }
    Polynomial(T const &, deg_type deg = 0);
    static  Polynomial fromCoeffVector(std::vector<T> const & coeffs);
    
    friend  deg_type     deg<>(Polynomial const &);
            Polynomial & with_monic(deg_type dg = 0) &;
    
    friend  bool operator == (Polynomial const & p, Polynomial const & q) { return p.coeffs == q.coeffs; }
    friend  bool operator != (Polynomial const & p, Polynomial const & q) { return p.coeffs != q.coeffs; }
    friend  bool operator <  (Polynomial const & p, Polynomial const & q) { return p.coeffs <  q.coeffs; }
    
            Polynomial      operator <<(deg_type d) const;
            Polynomial      operator >>(deg_type d) const;
            Polynomial &    operator<<=(deg_type d) & { return *this = *this << d; }
            Polynomial &    operator>>=(deg_type d) & { return *this = *this >> d; }
    
    static  std::pair<Polynomial, Polynomial> divmod(Polynomial         a, Polynomial const & b);
    static  void                              divmod(Polynomial const & a, Polynomial const & b, Polynomial & q, Polynomial & r);
    
            Polynomial      operator +      () const { return *this; }
            Polynomial      operator -      () const;
    
    friend  Polynomial      operator *      (T          const & t,   Polynomial         q) { return q *= t; }
    friend  Polynomial      operator *      (Polynomial         p,   T          const & t) { return p *= t; }
    
    // Auch wenn es nicht nötig ist, warum funtioniert er nicht??
  //friend  Polynomial      operator + <>   (Polynomial const & p,   Polynomial const & q);
    friend  Polynomial      operator * <>   (Polynomial const & p,   Polynomial const & q);
    friend  Polynomial      operator -      (Polynomial const & p,   Polynomial const & q) { return p + (-q); }
    friend  Polynomial      operator /      (Polynomial const & p,   Polynomial const & q) { return divmod(p, q).first; }
    friend  Polynomial      operator %      (Polynomial const & p,   Polynomial const & q) { return divmod(p, q).second; }
    
            Polynomial &    operator +=     (Polynomial const & q) &;
            Polynomial &    operator -=     (Polynomial const & q) & { return *this += (-q); }
            Polynomial &    operator *=     (Polynomial const & q) & { return *this = *this * q; }
            Polynomial &    operator /=     (Polynomial const & q) & { return *this = *this / q; }
            Polynomial &    operator %=     (Polynomial const & q) & { return *this = *this % q; }
    
            T const &       at              (deg_type) const &;
            T               at              (deg_type)      &&;
    
    friend  std::ostream &  operator << <>  (std::ostream &, Polynomial const &);
    // operator >> erzeugt einen analogen Fehler.
  //friend  std::istream &  operator >> <>  (std::istream &, Polynomial       &);
    
            size_t       hash() const noexcept;
};

/*
// Represents a Polynomial of Z<2> using vector<bool>.
// Provides usual arithmetic, equality comparison, plugging in.
// Maybe like usual Polynomial class with std::set<deg_type> instead of std::vector<bool>.
template<typename deg_type>
class Polynomial<Z<2>, deg_type> // non-literal class (non-trivial destructor)
{
private:
    using K = Z<2>;
    std::vector<bool> coeffs;

    // Dangerous constructor. Use only when sure that highest coefficient is set true!
    Polynomial(std::vector<bool>      && coeffs) : coeffs(coeffs) {  }
    Polynomial(std::vector<bool> const & coeffs) : coeffs(coeffs) {  }

    // Calculates v += (w >> offset)
    Polynomial & add_with_offset(Polynomial const & p, size_t offset);

public:
    Polynomial(K const & z = K(0), deg_type deg = 0);

    static  Polynomial      fromCoeffVector(std::vector<K> const & coeffs);
    
    friend  deg_type        deg(Polynomial const & p)    { return p.coeffs.empty() ? 0 : p.coeffs.size() - 1; }
            Polynomial &    with_monic(deg_type dg = 0) &;

    friend  bool            operator ==     (Polynomial const & p, Polynomial const & q)    { return p.coeffs == q.coeffs; }
    friend  bool            operator !=     (Polynomial const & p, Polynomial const & q)    { return p.coeffs != q.coeffs; }
    friend  bool            operator <      (Polynomial const & p, Polynomial const & q)    { return p.coeffs <  q.coeffs; }
    
            Polynomial      operator <<     (deg_type d);
            Polynomial      operator >>     (deg_type d);
            Polynomial &    operator<<=     (deg_type d) & { return *this = *this << d; }
            Polynomial &    operator>>=     (deg_type d) & { return *this = *this >> d; }
    
    static  std::pair<Polynomial, Polynomial> divmod(Polynomial         a, Polynomial const & b);
    static  void                              divmod(Polynomial const & a, Polynomial const & b, Polynomial & q, Polynomial & r);
    
            Polynomial      operator +      () const { return Polynomial(*this); }
            Polynomial      operator -      () const { return Polynomial(*this); }

            Polynomial      operator *      (K const & t) const { return t == K(0) ?         Polynomial(0) : *this; }
            Polynomial &    operator *=     (K const & t)       { return t == K(0) ? *this = Polynomial(0) : *this; }
    friend  Polynomial      operator *      (K const & t, Polynomial const & p) { return p.operator*(t); }
    
    friend  Polynomial      operator +      (Polynomial         p, Polynomial const & q) { return p += q; }
    friend  Polynomial      operator -      (Polynomial const & p, Polynomial const & q) { return p + q; }
    friend  Polynomial<K, deg_type>      operator *    (Polynomial<K, deg_type> const & p, Polynomial<K, deg_type> const & q);
    
            Polynomial &    operator +=     (Polynomial const & p) { return add_with_offset(p, 0); }
            Polynomial &    operator -=     (Polynomial const & p) { return *this += p; }
            Polynomial &    operator *=     (Polynomial const & p) { return *this = *this * p; }

    friend  Polynomial      operator /      (Polynomial const & a, Polynomial const & b) { return divmod(a, b).first; }
    friend  Polynomial      operator %      (Polynomial const & a, Polynomial const & b) { return divmod(a, b).second; }

    friend  std::ostream &  operator <<   (std::ostream & os, Polynomial<K, deg_type> const & p);
    friend  std::istream &  operator >>   (std::istream & is, Polynomial<K, deg_type>       & p);
    
            size_t hash() const noexcept { return std::hash<std::vector<bool>>() (coeffs); }
};

*/
/*
namespace X
{

/// If you need X with some type other than default, it is strongly advised to use  typedef Modulus::X<your_type> X; .
/// You can use X^deg with this:  Modulus::X::X X; Polynomial<T> p = X^2;
template<typename deg_type = size_t>
class X
{
    static_assert(std::numeric_limits<deg_type>::is_integer and not std::numeric_limits<deg_type>::is_signed,
            "deg_type must be an unsigned integer.");
    
    deg_type deg;
    constexpr X(deg_type deg) : deg(deg) {  }
    
public:
    constexpr X() : deg(1) { }
    
    template <typename T>
    operator Polynomial<T, deg_type>() const noexcept
    {
        return Polynomial<T, deg_type>(T(1), deg);
    }
    
    constexpr X   operator ^ (deg_type d) const { return X(deg + d); }
    constexpr X & operator ^=(deg_type d) const { deg += d; return *this; }
};

} // namespace X
*/
} // namespace Modulus

namespace std
{

template<typename T, typename deg_type>
struct hash<Modulus::Polynomial<T, deg_type>>
{
    inline size_t operator()(Modulus::Polynomial<T, deg_type> const & p) const noexcept
    { return p.hash(); }
};

} // namespace std


// As polynomial is a temlate class, it is necessary to include the cpp-file as well.
#include "Polynomial_defs.hpp"
