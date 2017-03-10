#ifndef SENTIMENTAL_TRANSFORM_MONEY_H
#define SENTIMENTAL_TRANSFORM_MONEY_H

#include <string>

namespace sm {
    namespace transform {
        class Money
        {
        public:
            std::string operator ()(const std::string &token);
        };
    }
}

#endif
