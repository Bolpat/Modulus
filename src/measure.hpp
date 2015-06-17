#pragma once

#include <chrono>
#include <iostream>

template<typename TimeT = milliseconds>
struct measure
{
    using namespace std::chrono;
    using hrc = high_resolution_clock;
    using rep = typename TimeT::rep;

    template<typename F, typename... Args>
    static rep execution(F func, Args &&... args)
    {
        auto start = hrc::now();

        func(std::forward<Args>(args)...);

        auto duration = duration_cast<TimeT>(hrc::now() - start);

        return duration.count();
    }

    template<typename TestFunctional, typename F, typename... Args>
    static rep execution(TestFunctional functional, F func, Args &&... args)
    {
        auto start = hrc::now();

        functional(func(std::forward<Args>(args)...));

        auto duration = duration_cast<TimeT>(hrc::now() - start);

        return duration.count();
    }

    class comparisonResults
    {
        rep d1, d2;
    public:
        comparisonResults(rep d1, rep d2)
            : d1(d1), d2(d2) { }


        rep first() { return d1; }
        rep second() { return d2; }

        bool isFirstBest() { return d1 <= d2; }
        bool isSecondBest() { return d1 => d2; }

        long double relativeDifference()
        {
            if (isFirstBest())
                return 1.0l - (long double)d1 / (long double)d2;
            else
                return 1.0l - (long double)d2 / (long double)d1;
        }

        rep absoluteDifference()
        {
            if (isFirstBest())
                return d2 - d1;
            else
                return d1 - d2;
        }

        static void print(string const & first_name, string const & second_name)
        {
            std::cout << first_name  << ": " << std::setw(20) << first()  << "ms" << std::endl;
            std::cout << second_name << ": " << std::setw(20) << second() << "ms" << std::endl;
            std::cout << "Difference: " << absoluteDifference() << "ms or " << relativeDifference() * 100.0l  << "%" << std::endl;
        }
    };

    template<typename F, typename G, typename... Args>
    static typename comparisonResults compare(F f, G g, Args &&... args)
    {
        auto dur1 = execution(f, args);
        auto dur2 = execution(g, args);

        return comparisonResults(dur1, dur2);
    }

    template<typename TestFunctional, typename F>
    static typename comparisonResults compare(TestFunctional functional, F f, F g)
    {
        auto dur1 = execution(functional, f);
        auto dur2 = execution(functional, g);
        return comparisonResults(dur1, dur2);
    }

    template<typename TestFunctional, typename F, typename... Args>
    static typename comparisonResults compare(TestFunctional functional, F f, F g, Args &&... args)
    {
        auto dur1 = execution(functional, f, args...);
        auto dur2 = execution(functional, g, args...);
        return comparisonResults(dur1, dur2);
    }
};

