#include <vector>

#include <thread>

#include <functional>
#include <algorithm>



template <template <typename, typename...> class Container, typename Item, typename... Ts, typename Func>
void FOR_EACH_PAR(Container<Item, Ts...> const & cont, Func && func)
{
    using Cont = Container<Item, Ts...>;
    
    size_t const size = cont.size();
    
    size_t const max_threads = std::max(static_cast<size_t>(1),
                                        std::min(size,
                                                 static_cast<size_t>(std::thread::hardware_concurrency())));
    std::vector<std::thread> threads;
    threads.reserve(max_threads);
    
    size_t inc = size / max_threads;
    size_t rem = size % max_threads;

    auto beg = cont.begin();
    auto end = cont.end();

    while (beg != end)
    {
        auto it = beg;
        beg += (rem > 0  ?  --rem, inc + 1  :  inc);
        
        threads.emplace_back(std::bind(std::for_each<typename Cont::const_iterator, Func>, it, beg, func));
        
    }

    for (auto & t : threads) t.join();
    
}
