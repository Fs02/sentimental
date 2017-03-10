#ifndef SENTIMENTAL_NOISYOR_H
#define SENTIMENTAL_NOISYOR_H

#include <sentimental/feature.h>
#include <sentimental/bayesnet/graph.h>
#include <sentimental/noisyor/weight.h>
#include <initializer_list>

namespace sm {

/* 
 * Binary Noisy Or Gate Model Bayesian Network Classifier
 * Reference:
 * Or gate Bayesian networks for text classification: A discriminative alternative approach to multinomial naive Bayes.
 * Luis M. de Campos, Juan M. Fern´andez-Luna, Juan F. Huete, Alfonso E. Romero
 */
class NoisyOr
{
public:
    explicit NoisyOr(const TermDocFeature &feature, bool mle_parameter = false);

    std::string predict(const std::string &text) const;
    std::size_t predict(const std::unordered_map<std::size_t, std::size_t> &vars) const;

    double posterior(std::size_t node, const std::unordered_map<std::size_t, std::size_t> &vars) const;

    inline bayesnet::Graph &graph() { return graph_; }
    inline const bayesnet::Graph &graph() const { return graph_; }

    inline noisyor::Weight &weight() { return weight_; }
    inline const noisyor::Weight &weight() const { return weight_; }
private:
    bayesnet::Graph graph_;
    noisyor::Weight weight_;
    std::unordered_map<std::size_t, std::string> label_vertices_;
};

}

#endif
