#ifndef SENTIMENTAL_UTILITY_H
#define SENTIMENTAL_UTILITY_H

#include <string>

namespace sm {
    std::string trim(const std::string &str);

    template <class T, class A>
    T join(const A &begin, const A &end, const T &t)
    {
        T result;
        for (A it=begin; it!=end; it++)
        {
            if (!result.empty())
                result.append(t);
            result.append(*it);
        }
        return result;
    }

}

#endif
