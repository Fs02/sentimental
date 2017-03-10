#ifndef SENTIMENTAL_TRANSFORM_HASHTAG_H
#define SENTIMENTAL_TRANSFORM_HASHTAG_H

#include <string>
#include <sentimental/wordcount.h>

namespace sm {
    namespace transform {
        class Hashtag
        {
        public:
            Hashtag(std::size_t max = 5);

            std::string operator ()(const std::string &token, const WordCount &count);

        private:
            std::size_t max_;
        };
    }
}

#endif
