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

// IMPORTANT NOTE: DO NOT INCLUDE THIS HEADER! ONLY EXCEPTION AT THE END OF Polynomial.hpp //
/////////////////////////////////////////////////////////////////////////////////////////////


// Polynomial<T> //
namespace Modulus
{


// Constructors //

template <typename T, typename deg_type> inline
Polynomial<T, deg_type>::Polynomial(T const & t, deg_type deg)
{
    if (t != T()) coeffs[deg] = t;
}

template <typename T, typename deg_type>
Polynomial<T, deg_type>
Polynomial<T, deg_type>::fromCoeffVector(std::vector<T> const & coeffs)
{
    Polynomial res;
    for (size_t i = 0; i < coeffs.size(); ++i)
        if (coeffs[i] != T())
            res.coeffs[i] = coeffs[i];
    return res;
}


// Methods //

template <typename T, typename deg_type> inline
deg_type deg(Polynomial<T, deg_type> const & p)
{
    if (p.coeffs.empty()) return 0;
    return p.coeffs.rbegin()->first;
}

template <typename T, typename deg_type>
Polynomial<T, deg_type> &
Polynomial<T, deg_type>::with_monic(deg_type dg) &
{
    const deg_type d = deg(*this) + 1;
    coeffs[d] = T(1);
    return *this;
}


// Shift //

template <typename T, typename deg_type>
Polynomial<T, deg_type>
    operator <<(Polynomial<T, deg_type> const & p, deg_type n)
{
    Polynomial<T, deg_type> res;
    for (auto & dcp : p.coeffs) res.coeffs[dcp.first + n] = dcp.second;
    return res;
}

template <typename T, typename deg_type>
Polynomial<T, deg_type>
    operator >>(Polynomial<T, deg_type> const & p, deg_type n)
{
    Polynomial<T, deg_type> res;
    for (auto & dcp : p.coeffs) if (dcp.first >= n) res.coeffs[dcp.first - n] = dcp.second;
    return res;
}


// Arithmetric //

template <typename T, typename deg_type>
Polynomial<T, deg_type>
Polynomial<T, deg_type>::operator - () const
{
    Polynomial<T, deg_type> res = *this;
    for (auto & dcp : res.coeffs) dcp.second = -dcp.second;
    return res;
}

template <typename T, typename deg_type>
Polynomial<T, deg_type>
    operator + (Polynomial<T, deg_type> const & p, Polynomial<T, deg_type> const & q)
{
    if (deg(p) > deg(q)) return q + p;
    Polynomial<T, deg_type> res = p;
    res += q;
    return res;
}

template <typename T, typename deg_type>
Polynomial<T, deg_type>
    operator * (Polynomial<T, deg_type> const & p, Polynomial<T, deg_type> const & q)
{
    Polynomial<T, deg_type> res;
    for (auto & dcp : q.coeffs) res += dcp.second * p << dcp.first;
    return res;
}

template <typename T, typename deg_type>
Polynomial<T, deg_type> &
Polynomial<T, deg_type>::operator *=(T const & t) &
{
    for (auto & coeff : coeffs) coeff.second *= t;
    return *this;
}

template <typename T, typename deg_type>
Polynomial<T, deg_type> &
Polynomial<T, deg_type>::operator +=(Polynomial<T, deg_type> const & q) &
{
    for (auto & dcp : q.coeffs) if ((coeffs[dcp.first] += dcp.second) == T()) coeffs.erase(dcp.first);
    return *this;
}


template <typename T, typename deg_type>
std::pair<Polynomial<T, deg_type>, Polynomial<T, deg_type>>
Polynomial<T, deg_type>::divmod(Polynomial<T, deg_type> a, Polynomial<T, deg_type> const & b)
{
    Polynomial<T, deg_type> q;
    while (deg(a) >= deg(b))
    {
        auto const d = deg(a) - deg(b);
        auto const c = a.coeffs.at(deg(a)) / b.coeffs.at(deg(b));
        q.coeffs[d] = c;
        a -= b * c << d;
    }
    return std::make_pair(std::move(q), std::move(a));
}

template <typename T, typename deg_type> inline
void Polynomial<T, deg_type>::divmod(Polynomial<T, deg_type> const & a, Polynomial<T, deg_type> const & b, Polynomial<T, deg_type> & q, Polynomial<T, deg_type> & r)
{
    auto qr_pair = divmod(a, b);
    q = std::move(qr_pair.first);
    r = std::move(qr_pair.second);
}

// Access //

template <typename T, typename deg_type>
T const & Polynomial<T, deg_type>::at(deg_type k) const &
{
    auto const it = coeffs.find(k);
    if (it == coeffs.end()) return T();
    return it->second;
}

template <typename T, typename deg_type>
T         Polynomial<T, deg_type>::at(deg_type k)      &&
{
    auto it = coeffs.find(k);
    if (it == coeffs.end()) return T();
    return move(it->second);
}

// Helper functions //

template<typename T> bool equal_minus_one(const T & value) { return value == T(-1); }
template<>           bool equal_minus_one<unsigned long long>(unsigned long long const &) { return false; }

template <typename T, typename deg_type>
std::string monom_string(std::pair<deg_type, T> const & m)
{
    std::ostringstream os;
    switch (m.first)
    {
    case  0:
        os << m.second;
        break;
        
    case  1:
        if      (m.second == 1)                 os << 'x';
        else if (equal_minus_one<T>(m.second))  os << "-x";
        else                                    os << m.second << 'x';
        break;
        
    default:
        if      (m.second ==  1)               os << "x^" << m.first;
        else if (equal_minus_one<T>(m.second)) os << "-x^" << m.first;
        else                                   os << m.second << "x^" << m.first;
    }
    return os.str();
}

// IO streams //

template <typename T, typename deg_type>
std::ostream &
    operator <<(std::ostream & o, Polynomial<T, deg_type> const & p)
{
    auto    it = p.coeffs.rbegin();
    
    if (it == p.coeffs.rend()) return o << T();
    
    o << monom_string(*it);
    while (++it != p.coeffs.rend())
    {
        if (it->second < T()) o << " - " << monom_string(std::make_pair(it->first, -it->second));
        else                  o << " + " << monom_string(*it);
    }
    return o;
}

template <typename T, typename deg_type>
bool read_monom(std::istringstream & is, deg_type & deg, T & coeff)
{
    using std::ios_base;
    using std::istringstream;
    
    std::string inp;
    is >> inp;
    if (inp == "") is.setstate(ios_base::failbit);
    else if (inp[0] == 'x' or (inp.size() >= 2 and inp[0] == '+' and inp[1] == 'x')) // starts with "x" or "+x"
    {
        auto x_pos = inp.find('x');
        if      (inp.back() == 'x')     { deg = 1; coeff = T(1); }
        else if (inp[x_pos + 1] != '^') is.setstate(ios_base::failbit);
        else
        {
            if (inp[0] == 'x') inp[0] = inp[1]          = ' '; // overwrite "x^"
            else               inp[0] = inp[1] = inp[2] = ' '; // overwrite "+x^"

            istringstream iss_inp(inp);
            if (iss_inp >> deg) coeff = T(1);
            else                is.setstate(ios_base::failbit);
        }
    }
    else if (inp[0] == '-' and inp[1] == 'x') // starts with "-x"
    {
        if (inp.back() == 'x') { deg = 1; coeff = T(-1); }
        else
        {
            inp[0] = inp[1] = inp[2] = ' '; // overwrite "-x^"
            istringstream iss_inp(inp);
            if (iss_inp >> deg) coeff = T(-1);
            else                is.setstate(ios_base::failbit);
        }
    }
    else
    {
        auto x_pos = inp.find('x');
        if (x_pos == std::string::npos)
        {
            istringstream iss_inp(inp);
            if (iss_inp >> coeff) deg = 0;
            else                  is.setstate(ios_base::failbit);
        }
        else if (inp.back() == 'x')
        {
            inp[x_pos] = ' ';
            istringstream iss_inp(inp);
            if (iss_inp >> coeff) deg = 1;
            else                  is.setstate(ios_base::failbit);
        }
        else if (x_pos + 1 != '^') is.setstate(ios_base::failbit);
        else
        {
            inp[x_pos] = inp[x_pos + 1] = ' ';
            istringstream iss_inp(inp);
            if (not(iss_inp >> coeff >> deg)) is.setstate(ios_base::failbit);
        }
    }

    return is;
}

void str_replace(std::string & subject, std::string const & search, std::string const & replace)
{
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos)
    {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

template <typename T, typename deg_type>
std::istream &
    operator >>(std::istream & i, Polynomial<T, deg_type>       & p)
{
    p = T();
    
    std::string inp;
    if (i >> inp)
    {
        str_replace(inp, "+", " +");
        str_replace(inp, "-", " -");
        
        std::istringstream is(inp);
        deg_type deg;
        T coeff;
        while (read_monom(is, deg, coeff))
        {
            if (coeff != T())
                p.coeffs[deg] = std::move(coeff);
        }
        if (is.bad()) i.setstate(std::ios_base::failbit);
    }
    return i;
}

// Hash //

template <typename T, typename deg_type>
size_t Polynomial<T, deg_type>::hash() const noexcept
{
    std::hash<deg_type> dhash;
    std::hash<T>        Thash;
    
    size_t res = 0;
    for (auto & dcp : coeffs)
    {
        auto fsthash = dhash(dcp.first);
        res ^= fsthash << (8 * sizeof(size_t));
        res ^= fsthash >> (8 * sizeof(size_t));
        res ^= Thash(dcp.second);
    }
    return res;
}


} // namespace Modulus

/*

// Polynomial<Z2> //
namespace Modulus
{

// Constructors //

template <typename deg_type>
Polynomial<Z<2>, deg_type>::Polynomial(K const & z, deg_type deg)
    : coeffs(z == K(0) ? 0 : deg + 1)
{
    if (not coeffs.empty()) coeffs[deg] = true;
}

template <typename deg_type>
Polynomial<Z<2>, deg_type>
Polynomial<Z<2>, deg_type>::fromCoeffVector(std::vector<Z<2>> const & coeffs)
{
    Polynomial<Z<2>, deg_type>  res;
    res.coeffs.reserve(coeffs.size());
    for (K z : coeffs) res.coeffs.push_back(z != K(0));
    while (not res.coeffs.empty() and not res.coeffs.back()) res.coeffs.pop_back();
    return res;
}

// Methods //

template <typename deg_type>
Polynomial<Z<2>, deg_type> &
Polynomial<Z<2>, deg_type>::with_monic(deg_type dg) &
{
    if (dg < coeffs.size()) { coeffs.push_back(true); }
    else                    { coeffs.resize(dg+1); coeffs[dg] = true; }
    return *this;
}

// Shift //

template <typename deg_type>
Polynomial<Z<2>, deg_type>
Polynomial<Z<2>, deg_type>::operator <<(deg_type d)
{
    coeffs.resize(coeffs.size() + d);
    for (deg_type k = coeffs.size() - 1; k >= d; ++k) coeffs[k] = coeffs[k-d];
    for (deg_type k = 0; k < d; ++k) coeffs[k] = false;
}

template <typename deg_type>
Polynomial<Z<2>, deg_type>
Polynomial<Z<2>, deg_type>::operator >>(deg_type d)
{
    deg_type k;
    for (k = 0; d < coeffs.size(); ++k, ++d) coeffs[k] = std::move(coeffs[d]);
    coeffs.resize(k);
}

// Arithmetric //

template <typename deg_type>
Polynomial<Z<2>, deg_type> &
Polynomial<Z<2>, deg_type>::add_with_offset(Polynomial<Z<2>, deg_type> const & p, size_t offset)
{
    std::vector<bool>       & v =   coeffs;
    std::vector<bool> const & w = p.coeffs;
    
    if (v.size() < w.size() + offset) v.resize(w.size() + offset);
    for (size_t i = 0; i < w.size(); ++i, ++offset) v[offset] = v[offset] != w[i];
    while (not v.empty() and not v.back()) v.pop_back();
    
    return *this;
}

template <typename deg_type>
Polynomial<Z<2>, deg_type>
    operator * (Polynomial<Z<2>, deg_type> const & p, Polynomial<Z<2>, deg_type> const & q)
{
    Polynomial<Z<2>, deg_type>  res;
    res.coeffs.reserve(p.coeffs.size() + q.coeffs.size());
    for (size_t i = 0; i < p.coeffs.size(); ++i) if (p.coeffs[i]) res.add_with_offset(q, i);
    return res;
}

template <typename deg_type>
std::pair<Polynomial<Z<2>, deg_type>, Polynomial<Z<2>, deg_type>>
    Polynomial<Z<2>, deg_type>::divmod(Polynomial<Z<2>, deg_type>         a,
                                       Polynomial<Z<2>, deg_type> const & b)
{
    // a is a copy, b is a reference!
    std::vector<bool> q;
    while (a.coeffs.size() >= b.coeffs.size())
    {
        q.push_back(a.coeffs.back());
        if (a.coeffs.back())
        {
            auto itv = a.coeffs.rbegin(); // iterator
            auto itw = b.coeffs.rbegin(); // const_iterator
            do { *itv = (*itv != *itw); } while (++itv, ++itw != b.coeffs.rend());
        }
        a.coeffs.pop_back();
    }
    std::reverse(q.begin(), q.end());
    return make_pair( Polynomial<Z<2>, deg_type>::fromCoeffVector(q), std::move(a) );
}

// IO streams //

template <typename deg_type>
std::ostream & operator <<(std::ostream & os, Polynomial<Z<2>, deg_type> const & p)
{
    auto & v = p.coeffs;
    switch (v.size())
    {
        case 0: os << '1'; break;
        case 1: os << "x"; if (v[0]) os << " + 1"; break;
        default:
            size_t i = v.size() - 1;
            os << "x^" << i;
            while (--i > 1) if (v[i]) os << " + x^" << i;
            if (v[1]) os << " + x";
            if (v[0]) os << " + 1";
    }
    return os;
}

template <typename deg_type>
std::istream & operator >>(std::istream & is, Polynomial<Z<2>, deg_type>       & p)
{
    using std::istringstream;
    using K = Z<2>;
    
    p = K(0);
    std::string inp;
    if (!(is >> inp)) return is;
    for (auto & c : inp) if (c == '+' or c == '-') c = ' ';

    istringstream iss(inp);
    while(iss >> inp)
    {
        if      (inp == "1") p += K(1);
        else if (inp == "x") p += Polynomial<K, deg_type>(K(1), 1);
        else
        {
            inp[0] = inp[1] = ' '; // delete "x^".
            deg_type d;
            istringstream iss_inp(inp);
            if (iss_inp >> d) p += Polynomial<K, deg_type>(K(1), d);
            else is.setstate(std::ios_base::failbit);
        }
    }
    return is;
}

} // namespace Modulus

*/
