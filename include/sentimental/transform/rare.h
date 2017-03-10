#ifndef SENTIMENTAL_TRANSFORM_RARE_H
#define SENTIMENTAL_TRANSFORM_RARE_H

#include <string>
#include <sentimental/wordcount.h>

namespace sm {
    namespace transform {
        class Rare
        {
        public:
            Rare(std::size_t max = 5, bool remove = true);

            std::string operator ()(const std::string &token, const WordCount &count);
        private:
            std::size_t max_;
            bool remove_;
        };
    }
}

#endif // RARE_H
