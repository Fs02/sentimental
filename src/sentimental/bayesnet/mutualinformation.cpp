#include <sentimental/bayesnet/mutualinformation.h>
#include <sentimental/bayesnet/observefrequency.h>
#include <cmath>

double sm::bayesnet::cond_mutual_information(const Graph &graph, const Cpt &cpt, const TermDocFeature &feature, std::size_t x, std::size_t y)
{
    ObserveFrequency n(x, {y}, graph, cpt, feature);
    double total = n.i();

    double sum = 0.0;
    for (std::size_t s = 0; s < n.s(); ++s)
    {
        double p_z = n.ic(s)/total;
        for (std::size_t q = 0; q < n.q(); ++q)
        {
            double p_xz = n.ijc(q, s)/total;
            for (std::size_t r = 0; r < n.r(); ++r)
            {
                double p_yz = n.ick(s, r)/total;
                double p_xyz = n.ijck(q, s, r)/total;

                double i = p_xyz * std::log((p_z * p_xyz)/(p_xz * p_yz));

                if (std::isfinite(i))
                    sum += i;
            }
        }
    }

    return sum;
}

double sm::bayesnet::mutual_information(const Graph &graph, const Cpt &cpt, const TermDocFeature &feature, std::size_t x, std::size_t y)
{
    ObserveFrequency n(x, {y}, graph, cpt, feature);
    double total = n.i();

    double sum = 0.0;
    for (std::size_t q = 0; q < n.q(); ++q)
    {
        double p_x = n.ij(q)/total;
        for (std::size_t r = 0; r < n.r(); ++r)
        {
            double p_y = n.ik(r)/total;
            double p_xy = n.ijk(q, r)/total;

            double i = p_xy * std::log(p_xy/(p_x*p_y));

            if (std::isfinite(i))
                sum += i;
        }
    }

    return sum;
}

double sm::bayesnet::mutual_information(const TermDocFeature &feature, const std::string &x, const std::string &y, const std::vector<std::string> &conditions)
{
    auto variables = conditions;
    variables.push_back(y);
    variables.push_back(x);
    sm::Frequency f(feature, variables);

    double total = f({});
    double sum = 0.0;
    auto zs = f.permutate(conditions);
    for (std::size_t z_ = 0; z_ < zs.size(); ++z_)
    {
        // get conditioning variables
        std::unordered_map<std::string, std::size_t> z_vars;
        for (std::size_t p = 0; p < zs[z_].size(); ++p)
            z_vars[conditions[p]] = zs[z_][p];

        double p_z = f(z_vars)/total;

        for (std::size_t y_ = 0; y_ < f.arity(y); ++y_)
        {
            auto yz_vars = z_vars;
            yz_vars[y] = y_;
            double p_yz = f(yz_vars)/total;

            for (std::size_t x_ = 0; x_ < f.arity(x); ++x_)
            {
                auto xz_vars = z_vars;
                xz_vars[x] = x_;
                double p_xz = f(xz_vars)/total;

                // xyz_vars
                z_vars[x] = x_;
                z_vars[y] = y_;
                double p_xyz = f(z_vars)/total;

                double term = p_xyz * std::log((p_z*p_xyz)/(p_xz*p_yz));

                if (std::isfinite(term))
                    sum += term;
            }
        }
    }

    return sum;
}
