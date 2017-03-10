#ifndef SENTIMENTAL_BAYESNET_BDEU_H
#define SENTIMENTAL_BAYESNET_BDEU_H

#include <sentimental/feature.h>
#include <sentimental/bayesnet/graph.h>
#include <sentimental/bayesnet/cpt.h>
#include <list>

namespace sm {
    namespace bayesnet {
        class BDeu
        {
        public:
            BDeu(const Cpt &cpt,
                 const TermDocFeature &feature,
                 double alpha = 0.5,
                 std::size_t cache_size = 5,
                 std::size_t label = 0,
                 std::size_t true_state = 1,
                 std::size_t false_state = 0);

            double operator ()(const Graph &graph);

        private:
            bool ret_cache(const std::size_t &child,
                           const std::set<std::size_t> &parents,
                           double &score);

            void put_cache(const std::size_t &child,
                           const std::set<std::size_t> &parents,
                           double score);

            const Cpt &cpt_;
            const TermDocFeature &feature_;
            double alpha_;
            std::size_t cache_size_;
            std::unordered_map<std::size_t, std::list<std::pair<std::set<std::size_t>, double>>> cache_;
            std::size_t label_;
            std::size_t true_state_;
            std::size_t false_state_;
        };
    }
}

#endif
