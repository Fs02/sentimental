#ifndef SENTIMENTAL_TRANSFORM_USERNAME_H
#define SENTIMENTAL_TRANSFORM_USERNAME_H

#include <string>

namespace sm {
    namespace transform {
        class Username
        {
        public:
            std::string operator ()(const std::string &token);
        };
    }
}

#endif
