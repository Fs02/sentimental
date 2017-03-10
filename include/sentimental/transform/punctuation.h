#ifndef SENTIMENTAL_TRANSFORM_PUNCTUATION_H
#define SENTIMENTAL_TRANSFORM_PUNCTUATION_H

#include <string>
#include <unordered_set>

namespace sm
{
    namespace transform {
        class Punctuation
        {
        public:
            Punctuation(bool alpha_numeric = false);

            std::string operator ()(const std::string &token);

        private:
            bool alpha_numeric_;
        };
    }
}

#endif
