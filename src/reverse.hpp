// Compile with clang++-3.5 -std=c++14

/* This code was found on kbok's blogspot, at 
 * http://kbokonseriousstuff.blogspot.de/2011/09/using-reverse-iterators-with-c11-range.html
 * visited on 2015-03-19.
 * 
 * As the "About" page states, code snippets are released under Public Domain.
 * See http://kbokonseriousstuff.blogspot.de/p/about.html
 */

namespace Modulus { namespace Utility
{

template<class Container>
class const_reverse_wrapper {
    Container const & container;

public:
    using iterator         = decltype(container.rbegin());
    using reverse_iterator = decltype(container.begin());
    
    const_reverse_wrapper(Container const & cont) : container(cont) { }
    
    iterator begin() const { return container.rbegin(); }
    iterator end()   const { return container.rend(); }
    
    reverse_iterator rbegin() const { return container.begin(); }
    reverse_iterator rend()   const { return container.end(); }
};

template<class Container>
class reverse_wrapper {
    Container & container;

public:
    using iterator         = decltype(container.rbegin());
    using reverse_iterator = decltype(container.begin());
    
    reverse_wrapper(Container & cont) : container(cont) { }
    
    iterator begin() { return container.rbegin(); }
    iterator end()   { return container.rend(); }
    
    reverse_iterator rbegin() const { return container.begin(); }
    reverse_iterator rend()   const { return container.end(); }
};

template<class Container> const_reverse_wrapper<Container> reverse(Container const & cont) { return const_reverse_wrapper<Container>(cont); }
template<class Container>       reverse_wrapper<Container> reverse(Container       & cont) { return       reverse_wrapper<Container>(cont); }

}} // namespace Modulus::Utility
