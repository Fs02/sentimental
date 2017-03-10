#ifndef SENTIMENTAL_TRANSFORM_HTMLESCAPE_H
#define SENTIMENTAL_TRANSFORM_HTMLESCAPE_H

#include <string>
#include <unordered_map>

namespace sm {
    namespace transform {
        class HTMLEscape
        {
        public:
            std::string operator ()(const std::string &token);

            static std::unordered_map<std::string, char> codes;
        };
    }
}

#endif
