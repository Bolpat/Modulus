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

#include "Z.hpp" // for Polynomial<Z2> specialisattion.

namespace Modulus
{
// Forward declaration of the types and non-inline template friend functions.

template <typename T, typename deg_type = size_t>   class Polynomial;

template <typename T, typename deg_type>    deg_type                deg         (Polynomial<T, deg_type> const &);
template <typename T, typename deg_type>    Polynomial<T, deg_type> operator << (Polynomial<T, deg_type> const &,  deg_type);
template <typename T, typename deg_type>    Polynomial<T, deg_type> operator >> (Polynomial<T, deg_type> const &,  deg_type);
template <typename T, typename deg_type>    Polynomial<T, deg_type> operator +  (Polynomial<T, deg_type> const &,  Polynomial<T, deg_type> const &);
template <typename T, typename deg_type>    Polynomial<T, deg_type> operator *  (Polynomial<T, deg_type> const &,  Polynomial<T, deg_type> const &);
template <typename T, typename deg_type>    std::ostream &          operator << (std::ostream &,                   Polynomial<T, deg_type> const &);
template <typename T, typename deg_type>    std::istream &          operator >> (std::istream &,                   Polynomial<T, deg_type>       &);

template <unsigned p, typename deg_type = size_t>
using ZPoly = Polynomial<Z<p>, deg_type>;

template <typename deg_type>    ZPoly<2, deg_type>  operator << (ZPoly<2, deg_type> , deg_type d);
template <typename deg_type>    ZPoly<2, deg_type>  operator >> (ZPoly<2, deg_type> , deg_type d);
template <typename deg_type>    ZPoly<2, deg_type>  operator *  (ZPoly<2, deg_type>  const &, ZPoly<2, deg_type>  const &);

template <typename deg_type>    std::ostream &      operator << (std::ostream & os, ZPoly<2, deg_type>  const & p);
template <typename deg_type>    std::istream &      operator >> (std::istream & is, ZPoly<2, deg_type>        & p);

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
    
    static  Polynomial      fromCoeffVector(std::vector<T> const & coeffs);
    
    friend  deg_type        deg<>(Polynomial const &);
            Polynomial &    with_monic(deg_type dg = 0) &;
    
    friend  bool operator == (Polynomial const & p, Polynomial const & q) { return p.coeffs == q.coeffs; }
    friend  bool operator != (Polynomial const & p, Polynomial const & q) { return p.coeffs != q.coeffs; }
    friend  bool operator <  (Polynomial const & p, Polynomial const & q) { return p.coeffs <  q.coeffs; }
    
    friend  Polynomial      operator << <>  (Polynomial const & p,  deg_type d);
    friend  Polynomial      operator >> <>  (Polynomial const & p,  deg_type d);
            Polynomial &    operator<<=     (                       deg_type d) & { return *this = *this << d; }
            Polynomial &    operator>>=     (                       deg_type d) & { return *this = *this >> d; }
    
    static  std::pair<Polynomial, Polynomial> divmod(Polynomial         a, Polynomial const & b);
    static  void                              divmod(Polynomial const & a, Polynomial const & b, Polynomial & q, Polynomial & r);
    
    // Must be friend, see http://stackoverflow.com/q/29318021/3273130?sem=2
    friend  Polynomial      operator +      (Polynomial p) { return p; }
            Polynomial      operator -      () const;
    
    friend  Polynomial      operator *      (T          const & t,   Polynomial         q) { return q *= t; }
    friend  Polynomial      operator *      (Polynomial         p,   T          const & t) { return p *= t; }
    
    // Auch wenn es nicht nötig ist, warum funtioniert er nicht??
    friend  Polynomial      operator + <>   (Polynomial const & p,   Polynomial const & q);
    friend  Polynomial      operator * <>   (Polynomial const & p,   Polynomial const & q);
    friend  Polynomial      operator -      (Polynomial const & p,   Polynomial const & q) { return p + (-q); }
    friend  Polynomial      operator /      (Polynomial const & p,   Polynomial const & q) { return divmod(p, q).first; }
    friend  Polynomial      operator %      (Polynomial const & p,   Polynomial const & q) { return divmod(p, q).second; }
    
            Polynomial &    operator *=     (T          const & t) &;
    
            Polynomial &    operator +=     (Polynomial const & q) &;
            Polynomial &    operator -=     (Polynomial const & q) & { return *this += (-q); }
            Polynomial &    operator *=     (Polynomial const & q) & { return *this = *this * q; }
            Polynomial &    operator /=     (Polynomial const & q) & { return *this = *this / q; }
            Polynomial &    operator %=     (Polynomial const & q) & { return *this = *this % q; }
    
            T const &       at              (deg_type) const &;
            T               at              (deg_type)      &&;
    
    friend  std::ostream &  operator << <>  (std::ostream &, Polynomial const &);
    friend  std::istream &  operator >> <>  (std::istream &, Polynomial       &);
    
            size_t       hash() const noexcept;
};


// Represents a Polynomial of Z<2> using vector<bool>.
// Provides usual arithmetic, equality comparison, plugging in.
// Maybe like usual Polynomial class with std::set<deg_type> instead of std::vector<bool>.
template<typename deg_type>
class Polynomial<Z<2>, deg_type> // non-literal class (non-trivial destructor)
{
private:
    std::vector<bool> coeffs;

    // Dangerous constructor. Use only when sure that highest coefficient is set true!
    Polynomial(std::vector<bool>      && coeffs) : coeffs(coeffs) {  }
    Polynomial(std::vector<bool> const & coeffs) : coeffs(coeffs) {  }

    // Calculates v += (w >> offset)
    Polynomial & add_with_offset(Polynomial const & p, size_t offset);

public:
    Polynomial(Z<2> const & z = Z<2>(), deg_type deg = 0);

    static  Polynomial      fromCoeffVector(std::vector<Z<2>> const & coeffs);
    
    friend  deg_type        deg(Polynomial const & p)    { return p.coeffs.empty() ? 0 : p.coeffs.size() - 1; }
            Polynomial &    with_monic(deg_type dg = 0) &;

    friend  bool            operator ==     (Polynomial const & p, Polynomial const & q)    { return p.coeffs == q.coeffs; }
    friend  bool            operator !=     (Polynomial const & p, Polynomial const & q)    { return p.coeffs != q.coeffs; }
    friend  bool            operator <      (Polynomial const & p, Polynomial const & q)    { return p.coeffs <  q.coeffs; }
    
    friend  Polynomial      operator << <>  (Polynomial, deg_type d);
    friend  Polynomial      operator >> <>  (Polynomial, deg_type d);
            Polynomial &    operator<<=     (deg_type d) & { return *this = *this << d; }
            Polynomial &    operator>>=     (deg_type d) & { return *this = *this >> d; }
    
    static  std::pair<Polynomial, Polynomial> divmod(Polynomial         a, Polynomial const & b);
    static  void                              divmod(Polynomial const & a, Polynomial const & b, Polynomial & q, Polynomial & r);
    
            Polynomial      operator +      () const { return Polynomial(*this); }
            Polynomial      operator -      () const { return Polynomial(*this); }

    friend  Polynomial      operator *      (Polynomial const & p, Z<2> const & t) { return t == Z<2>(0) ? Polynomial() : p; }
    friend  Polynomial      operator *      (Z<2> const & t, Polynomial const & p) { return p.operator*(t); }
    
    friend  Polynomial      operator +      (Polynomial         p, Polynomial const & q) { return p += q; }
    friend  Polynomial      operator -      (Polynomial const & p, Polynomial const & q) { return p + q; }
    friend  Polynomial      operator *  <>  (Polynomial const & p, Polynomial const & q);
    friend  Polynomial      operator /      (Polynomial const & a, Polynomial const & b) { return divmod(a, b).first; }
    friend  Polynomial      operator %      (Polynomial const & a, Polynomial const & b) { return divmod(a, b).second; }
    
    Polynomial &            operator *=     (Z<2> const & t)          { return t == Z<2>(0) ? *this = Polynomial() : *this; }
    
            Polynomial &    operator +=     (Polynomial const & p) { return add_with_offset(p, 0); }
            Polynomial &    operator -=     (Polynomial const & p) { return *this += p; }
            Polynomial &    operator *=     (Polynomial const & p) { return *this = *this * p; }
            Polynomial &    operator /=     (Polynomial const & p) { return *this = *this / p; }
            Polynomial &    operator %=     (Polynomial const & p) { return *this = *this % p; }
            
    friend  std::ostream &  operator << <>  (std::ostream & os, Polynomial const & p);
    friend  std::istream &  operator >> <>  (std::istream & is, Polynomial       & p);
    
            size_t hash() const noexcept { return std::hash<std::vector<bool>>() (coeffs); }
};

namespace Eks
{
// The namespace is made to minimize errors ocurring for accidentally using X.

/// If you need default X (size_t) to easily type polynomials in code user-friendly, it is recommanded to do like this:
///     using Modulus::Eks::X;
/// For your own your_deg_type, use
///     constexpr Modulus::Eks::Eks<your_deg_type> X;
/// Remember you need parenthesis around 'powers'.
template<typename deg_type = size_t>
class Eks
{
    static_assert(std::numeric_limits<deg_type>::is_integer and not std::numeric_limits<deg_type>::is_signed,
            "deg_type must be an unsigned integer.");
    
    deg_type deg;
    constexpr Eks(deg_type deg) : deg(deg) {  }
    
public:
    constexpr Eks() : deg(1) { }
    
    template <typename T>
    operator Polynomial<T, deg_type>() const noexcept
    {
        return Polynomial<T, deg_type>(T(1), deg);
    }
    
    constexpr Eks   operator ^ (deg_type d) const { return Eks(deg + d); }
    constexpr Eks & operator ^=(deg_type d) const { deg += d; return *this; }
};

constexpr Eks<> X;

} // namespace Eks

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
#include "Polynomial.cpp"
