#include <iostream>
#include <algorithm>
#include <sentimental/table.h>
#include <sentimental/texttransform.h>
#include <sentimental/feature.h>
#include <sentimental/wordcount.h>
#include <sentimental/naivebayes.h>
#include <sentimental/featureselection.h>
#include <sentimental/bayesnet.h>
#include <sentimental/bayesnet/simulatedannealing.h>
#include <sentimental/bayesnet/bdeu.h>
#include <sentimental/bayesnet/mapestimate.h>
#include <sentimental/bayesnet/mleestimate.h>
#include <sentimental/bayesnet/mestimate.h>
#include <sentimental/bayesnet/observefrequency.h>
#include <sentimental/bayesnet/fcll.h>
#include <sentimental/bayesnet/classificationrate.h>
#include <sentimental/bayesnet/treeaugmentednaivebayes.h>
#include <sentimental/bayesnet/mutualinformation.h>
#include <sentimental/noisyor.h>
#include <sentimental/frequency.h>
#include <cassert>
#include <json/json.h>
#include <fstream>
#include <iomanip>
#include <pgm/pgm.h>
#include <random>

#define MAX_EDGES 2

void generate_mood_dataset(const std::vector<std::string> &text, const std::vector<std::string> &label, const std::string path)
{
    std::ofstream out(path);
    out << "positive,negative,emotion" << std::endl;

    for (std::size_t i = 0; i < text.size(); ++i)
    {
        bool positive = false;
        bool negative = false;

        positive = positive || (text[i].find("{{emoticon:joy}}") != std::string::npos);

        negative = negative || (text[i].find("{{emoticon:angry}}") != std::string::npos);
        negative = negative || (text[i].find("{{emoticon:disgust}}") != std::string::npos);
        negative = negative || (text[i].find("{{emoticon:fear}}") != std::string::npos);
        negative = negative || (text[i].find("{{emoticon:sadness}}") != std::string::npos);
        negative = negative || (text[i].find("{{emoticon:surprise}}") != std::string::npos);

        out << (positive ? "true" : "false") << "," << (negative ? "true" : "false") << "," << label[i] << std::endl;
    }
}

void hybrid(std::size_t fold, std::vector<std::pair<std::string, std::string>> &arcs, const std::vector<std::string> &texts_emot, const std::vector<std::string> &texts, const std::vector<std::string> &labels, int seed = 1)
{
    std::vector<int> order(labels.size()) ;
    std::iota (std::begin(order), std::end(order), 0);
    std::mt19937 g(seed);
    std::shuffle(order.begin(), order.end(), g);

    // perform k-cv
    const std::size_t segment = labels.size()/fold;
    for (std::size_t k = 0; k < fold; ++k)
    {
        pgm::Dataset train_dataset;
        train_dataset.add_variable("emotion", {":: fear", ":: surprise", ":: joy", ":: sadness", ":: anger", ":: disgust"});
        train_dataset.add_variable("positive", {"true", "false"});
        train_dataset.add_variable("negative", {"true", "false"});
        pgm::Dataset test_dataset;
        test_dataset.add_variable(train_dataset.variable("emotion"));
        test_dataset.add_variable(train_dataset.variable("positive"));
        test_dataset.add_variable(train_dataset.variable("negative"));

        std::vector<std::string> train_tweet;
        std::vector<std::string> train_label;
        std::vector<std::string> test_tweet;
        std::vector<std::string> test_label;
        for (std::size_t i = 0; i < order.size(); ++i)
        {
            std::size_t index = order[i];

            bool positive = false;
            bool negative = false;
            positive = positive || (texts_emot[index].find("{{emoticon:joy}}") != std::string::npos);
            negative = negative || (texts_emot[index].find("{{emoticon:angry}}") != std::string::npos);
            negative = negative || (texts_emot[index].find("{{emoticon:disgust}}") != std::string::npos);
            negative = negative || (texts_emot[index].find("{{emoticon:fear}}") != std::string::npos);
            negative = negative || (texts_emot[index].find("{{emoticon:sadness}}") != std::string::npos);
            negative = negative || (texts_emot[index].find("{{emoticon:surprise}}") != std::string::npos);

            if (i <= k * segment || i > (k + 1) * segment)
            {
                train_tweet.push_back(texts[index]);
                train_label.push_back(labels[index]);
                train_dataset.push({{"emotion", labels[index]}, {"positive", positive ? "true" : "false"}, {"negative", negative ? "true" : "false"}});
            }
            else
            {
                test_tweet.push_back(texts[order[i]]);
                test_label.push_back(labels[order[i]]);
                test_dataset.push({{"emotion", labels[index]}, {"positive", positive ? "true" : "false"}, {"negative", negative ? "true" : "false"}});
            }
        }
        auto train_feature = sm::TermDocFeature(train_label, train_tweet);

        // bayesnet section
        pgm::Bayesnet bn;
        bn.add_node(train_dataset.variable("emotion"));
        bn.add_node(train_dataset.variable("positive"));
        bn.add_node(train_dataset.variable("negative"));
        for (auto arc : arcs)
        {
            bn.add_arc(arc.first, arc.second);
        }

        pgm::SampleEstimate estimate;
        estimate(bn, train_dataset);

        // multinomial naive bayes section
        sm::NaiveBayes nb;
        nb.train(train_feature);

        std::cout << "==> Testing on Test Data" << std::endl;
        std::ofstream out(std::to_string(k) + "_test.csv");
        out << "actual,predict\n";
        std::size_t correct = 0;
        for (std::size_t i = 0; i < test_label.size(); ++i)
        {
            std::cout << "[" << i << "/" << test_label.size() << "] testing model..." << "\r" << std::flush;
            
            std::string predict;
            double klass_prob = -std::numeric_limits<double>::infinity();

            for (auto state : bn.variable("emotion").states())
            {
                double prob = nb.likelihood(test_tweet[i], state, 1.0);
                auto evidences = test_dataset[i];
                evidences["emotion"] = state;
                prob *= bn.query(evidences);

                if (prob > klass_prob)
                {
                    klass_prob = prob;
                    predict = state;
                }
            }

            auto actual = test_label[i];
            out << actual << "," << predict << "\n";
            if (actual == predict)
                ++correct;
        }
        std::cout << std::endl;
        std::cout << "Correct : " << correct << "/" << test_label.size() << "\n";
        std::cout << "Accuracy : " << correct/double(test_label.size()) << "\n";
    }
}

void test_transform(sm::TextTransform &transform)
{
    std::cout << transform("I i I I&apos;m not http://google.com https:://google.com a supreme &quot;&amp; @aikazm #love leader &lt;3 &gt;&gt;who wants to get to sleep.&quot;") << "\n";

    std::cout << transform("isn't hasn't hadn't didn't wouldn't can't") << "\n";

    std::cout << transform("she's there's he's it's Mary's Jim's Germany's who's") << "\n";

    std::cout << transform("I'll you'll she'll we'll they'll") << "\n";

    std::cout << transform("I'd you'd he'd we'd they'd -_-") << "\n";

    std::cout << transform("I've you've we've they've?") << "\n";

    std::cout << transform("you're GoodDay they're we're") << "\n";

    std::cout << transform("you're $123 $$$ $12 $$ they're sooo sweet loove love sweetts we're") << "\n";

    std::cout << transform("the 12 -14 123.4 -23423.2 babies' crying :)") << "\n";
}

void export_bow(const sm::TermDocFeature &feature, const std::string &path)
{
    std::ofstream out(path);
    std::string row;

    // first row
    row += "label/term";
    for (std::size_t doc = 0; doc < feature.labels().size(); ++doc)
        row += ","+std::to_string(doc);
    out << row << std::endl;

    row.clear();
    row += "<-label->";
    for (std::size_t doc = 0; doc < feature.labels().size(); ++doc)
        row += ","+feature.labels()[doc];
    out << row << std::endl;

    for (auto term : feature.get().storage())
    {
        row.clear();
        row += "\"" + term.first + "\"";
        for (std::size_t doc = 0; doc < feature.labels().size(); ++doc)
        {
            auto it = term.second.find(doc);
            if (it != term.second.end())
                row += "," + std::to_string(it->second);
            else
                row += ",0";
        }
        out << row << std::endl;
    }
}

void export_gobnilp_dat(const sm::TermDocFeature &feature, const std::string &path)
{
    std::ofstream out(path);

    // first row, node
    out << "<!class!>";
    for (auto term : feature.get().storage())
        out << " " << term.first;
    out << std::endl;

    // second row, arities
    out << "6";
    for (auto term : feature.get().storage())
        out << " 2";
    out << std::endl;

    for (std::size_t doc = 0; doc < feature.labels().size(); ++doc)
    {
        out << feature.labels()[doc].substr(3, feature.labels()[doc].size() - 2);;
        for (auto term : feature.get().storage())
        {
            auto it = term.second.find(doc);
            if (it != term.second.end())
                out << " yes";
            else
                out << " no";
        }
        out << std::endl;
    }
}

void export_blip_dat(const sm::TermDocFeature &feature, const std::string &path)
{
    std::map<std::string, int> label_map;
    label_map[":: fear"] = 0;
    label_map[":: surprise"] = 1;
    label_map[":: joy"] = 2;
    label_map[":: sadness"] = 3;
    label_map[":: anger"] = 4;
    label_map[":: disgust"] = 5;

    std::ofstream out(path);

    // first row, node
    out << "{{class}}";
    for (auto term : feature.get().storage())
        out << "," << term.first;
    out << std::endl;

    for (std::size_t doc = 0; doc < feature.labels().size(); ++doc)
    {
        out << label_map.at(feature.labels()[doc]);
        for (auto term : feature.get().storage())
        {
            auto it = term.second.find(doc);
            if (it != term.second.end())
                out << ",1";
            else
                out << ",0";
        }
        out << std::endl;
    }
}

void learn_nb(const sm::TermDocFeature &train_feature, const sm::TermDocFeature &test_feature, const std::string &name)
{
    std::cout << "preparing datasets" << std::endl;
    pgm::Dataset dataset;
    for (std::size_t i = 0; i < train_feature.labels().size(); ++i)
    {
        dataset.set("{{class}}", i, train_feature.labels()[i]);
    }
    for (auto w : train_feature.get().storage())
    {
        dataset.add_variable(w.first, {"F", "T"});
        for (auto doc : w.second)
            dataset.set(w.first, doc.first, "T");
    }

    std::cout << "constructing bn" << std::endl;
    pgm::Bayesnet bn;
    for (auto v : dataset.variables())
        bn.add_node(v);

    pgm::SimulatedAnnealing annealing(0);
    annealing.init_as_naive_bayes("{{class}}");
    annealing.verbose(false);
    pgm::Fcll score(dataset, "{{class}}");
    pgm::SampleEstimate estimate;

    std::cout << "[fake] searching best structure" << std::endl;
    annealing(bn, score);

    std::cout << "estimating distributions" << std::endl;
    estimate(bn, dataset);
    pgm::write_dot(bn, name + ".dot");

    //std::cout << bn << std::endl;

    {
        std::size_t start = dataset.size();
        for (std::size_t i = 0; i < test_feature.labels().size(); ++i)
        {
            dataset.set("{{class}}", start+i, test_feature.labels()[i]);
        }
        for (auto w : test_feature.get().storage())
        {
            if (!dataset.add_variable(w.first, {"F", "T"}))
            {
		        for (auto doc : w.second)
		            dataset.set(w.first, start+doc.first, "T");
            }
            else
            {
            	// if unused, remvoe it
            	dataset.rem_variable(w.first);
            }
        }

        std::cout << "==> Testing on Test Data" << std::endl;
        std::ofstream out(name + "_test.csv");
        out << "actual,predict\n";
        std::size_t correct = 0;
        for (std::size_t i = start; i < dataset.size(); ++i)
        {
            std::cout << "[" << i+1-start << "/" << dataset.size()-start << "] testing model..." << "\r" << std::flush;
            auto row = dataset[i];

            auto actual = row["{{class}}"];
            auto predict = bn.infer("{{class}}", row);
            out << actual << "," << predict << "\n";
            if (actual == predict)
                ++correct;
        }
        std::cout << std::endl;
        std::cout << "Correct : " << correct << "/" << dataset.size()-start << "\n";
        std::cout << "Accuracy : " << correct/double(dataset.size()-start) << "\n";
    }
}

void timelapse_bn(const sm::TermDocFeature &train_feature, const sm::TermDocFeature &test_feature, const std::string &name, std::size_t max_iter)
{
    std::cout << "preparing datasets" << std::endl;
    pgm::Dataset dataset;
    for (std::size_t i = 0; i < train_feature.labels().size(); ++i)
    {
        dataset.set("{{class}}", i, train_feature.labels()[i]);
    }
    for (auto w : train_feature.get().storage())
    {
        dataset.add_variable(w.first, {"F", "T"});
        for (auto doc : w.second)
            dataset.set(w.first, doc.first, "T");
    }

    std::cout << "constructing bn" << std::endl;
    pgm::Bayesnet bn;
    for (auto v : dataset.variables())
        bn.add_node(v);
    bn.graph().max_adjacents(MAX_EDGES);

    pgm::SimulatedAnnealing annealing(max_iter);
    annealing.init_as_naive_bayes("{{class}}");
    annealing.verbose(true);
    pgm::Fcll score(dataset, "{{class}}");
    pgm::SampleEstimate estimate;

    std::cout << "searching best structure" << std::endl;
    double fitness = annealing(bn, score);
    std::cout << "estimating distributions" << std::endl;
    estimate(bn, dataset);
    pgm::write_dot(bn, name + ".dot");

    //std::cout << bn << std::endl;

    {
        std::cout << "==> Testing on Train Data" << std::endl;
        std::ofstream out(name + "-fitness-"+ std::to_string(fitness) + "_train.csv");
        out << "actual,predict\n";
        std::size_t correct = 0;
        for (std::size_t i = 0; i < dataset.size(); ++i)
        {
            std::cout << "[" << i+1 << "/" << dataset.size() << "] testing model..." << "\r" << std::flush;
            auto row = dataset[i];

            auto actual = row["{{class}}"];
            auto predict = bn.infer("{{class}}", row);
            out << actual << "," << predict << "\n";
            if (actual == predict)
                ++correct;
        }
        std::cout << std::endl;
        std::cout << "Correct : " << correct << "/" << dataset.size() << "\n";
        std::cout << "Accuracy : " << correct/double(dataset.size()) << "\n";        
    }

    {
        std::size_t start = dataset.size();
        for (std::size_t i = 0; i < test_feature.labels().size(); ++i)
        {
            dataset.set("{{class}}", start+i, test_feature.labels()[i]);
        }
        for (auto w : test_feature.get().storage())
        {
            if (!dataset.add_variable(w.first, {"F", "T"}))
            {
		        for (auto doc : w.second)
		            dataset.set(w.first, start+doc.first, "T");            	
            }
            else
            {
            	// if unused, remvoe it
            	dataset.rem_variable(w.first);
            }
        }

        std::cout << "==> Testing on Test Data" << std::endl;
        std::ofstream out(name + "-fitness-"+ std::to_string(fitness) + "_test.csv");
        out << "actual,predict\n";
        std::size_t correct = 0;
        for (std::size_t i = start; i < dataset.size(); ++i)
        {
            std::cout << "[" << i+1-start << "/" << dataset.size()-start << "] testing model..." << "\r" << std::flush;
            auto row = dataset[i];

            auto actual = row["{{class}}"];
            auto predict = bn.infer("{{class}}", row);
            out << actual << "," << predict << "\n";
            if (actual == predict)
                ++correct;
        }
        std::cout << std::endl;
        std::cout << "Correct : " << correct << "/" << dataset.size()-start << "\n";
        std::cout << "Accuracy : " << correct/double(dataset.size()-start) << "\n";        
    }
}

void peek_cv(std::size_t fold, std::size_t k, const std::vector<std::string> &tweets, const std::vector<std::string> &labels, 
    double critical_value, int seed = 1)
{
    // shuffle dataset order
    std::vector<int> order(labels.size()) ;
    std::iota (std::begin(order), std::end(order), 0);
    std::mt19937 g(seed);
    std::shuffle(order.begin(), order.end(), g);

    // perform peek k-cv
    const std::size_t segment = labels.size()/fold;
    std::vector<std::string> train_tweet;
    std::vector<std::string> train_label;
    std::vector<std::string> test_tweet;
    std::vector<std::string> test_label;
    for (std::size_t i = 0; i < order.size(); ++i)
    {
        if (i <= k * segment || i > (k + 1) * segment)
        {
            train_tweet.push_back(tweets[order[i]]);
            train_label.push_back(labels[order[i]]);
        }
        else
        {
            test_tweet.push_back(tweets[order[i]]);
            test_label.push_back(labels[order[i]]);
        }
    }
    auto train_feature = sm::TermDocFeature(train_label, train_tweet);
    auto test_feature = sm::TermDocFeature(test_label, test_tweet);
    std::cout << "original train vocab : " << train_feature.get().storage().size() << std::endl;;

    sm::FeatureSelection fselect(train_feature, sm::selection::Chi2);

    auto selected = fselect.range(critical_value);
    std::cout << "selected train vocab : " << selected.get().storage().size() << std::endl;;

    for (std::size_t iter = 0; iter <= 5000; iter += 200)
    {
        std::cout << "======================================\n";
        std::cout << "ITER MAX = " << iter << "\n";
        std::cout << "======================================\n";
        timelapse_bn(selected, test_feature, "bn-fold-" + std::to_string(k) + "-iter-" + std::to_string(iter) + "-cv-" + std::to_string(critical_value), iter);
    }
}

void learn_bn(const sm::TermDocFeature &train_feature, const sm::TermDocFeature &test_feature, const std::string &name)
{
    std::cout << "preparing datasets" << std::endl;
    pgm::Dataset dataset;
    for (std::size_t i = 0; i < train_feature.labels().size(); ++i)
    {
        dataset.set("{{class}}", i, train_feature.labels()[i]);
    }
    for (auto w : train_feature.get().storage())
    {
        dataset.add_variable(w.first, {"F", "T"});
        for (auto doc : w.second)
            dataset.set(w.first, doc.first, "T");
    }

    std::cout << "constructing bn" << std::endl;
    pgm::Bayesnet bn;
    for (auto v : dataset.variables())
        bn.add_node(v);
    bn.graph().max_adjacents(MAX_EDGES);

    pgm::SimulatedAnnealing annealing(5000);
    annealing.init_as_naive_bayes("{{class}}");
    annealing.verbose(false);
    pgm::Fcll score(dataset, "{{class}}");
    pgm::SampleEstimate estimate;

    std::cout << "searching best structure" << std::endl;
    annealing(bn, score);
    std::cout << "estimating distributions" << std::endl;
    estimate(bn, dataset);
    pgm::write_dot(bn, name + ".dot");

    //std::cout << bn << std::endl;

    {
        std::size_t start = dataset.size();
        for (std::size_t i = 0; i < test_feature.labels().size(); ++i)
        {
            dataset.set("{{class}}", start+i, test_feature.labels()[i]);
        }
        for (auto w : test_feature.get().storage())
        {
            if (!dataset.add_variable(w.first, {"F", "T"}))
            {
		        for (auto doc : w.second)
		            dataset.set(w.first, start+doc.first, "T");            	
            }
            else
            {
            	// if unused, remvoe it
            	dataset.rem_variable(w.first);
            }
        }

        std::cout << "==> Testing on Test Data" << std::endl;
        std::ofstream out(name + "_test.csv");
        out << "actual,predict\n";
        std::size_t correct = 0;
        for (std::size_t i = start; i < dataset.size(); ++i)
        {
            std::cout << "[" << i+1-start << "/" << dataset.size()-start << "] testing model..." << "\r" << std::flush;
            auto row = dataset[i];

            auto actual = row["{{class}}"];
            auto predict = bn.infer("{{class}}", row);
            out << actual << "," << predict << "\n";
            if (actual == predict)
                ++correct;
        }
        std::cout << std::endl;
        std::cout << "Correct : " << correct << "/" << dataset.size()-start << "\n";
        std::cout << "Accuracy : " << correct/double(dataset.size()-start) << "\n";        
    }
}

void cross_validation(std::size_t fold, const std::vector<std::string> &tweets, const std::vector<std::string> &labels, 
    double critical_value, int seed = 1)
{
    // shuffle dataset order
    std::vector<int> order(labels.size()) ;
    std::iota (std::begin(order), std::end(order), 0);
    std::mt19937 g(seed);
    std::shuffle(order.begin(), order.end(), g);

    // perform k-cv
    const std::size_t segment = labels.size()/fold;
    for (std::size_t k = 0; k < fold; ++k)
    {
        std::vector<std::string> train_tweet;
        std::vector<std::string> train_label;
        std::vector<std::string> test_tweet;
        std::vector<std::string> test_label;
        for (std::size_t i = 0; i < order.size(); ++i)
        {
            if (i <= k * segment || i > (k + 1) * segment)
            {
                train_tweet.push_back(tweets[order[i]]);
                train_label.push_back(labels[order[i]]);
            }
            else
            {
                test_tweet.push_back(tweets[order[i]]);
                test_label.push_back(labels[order[i]]);
            }
        }
        auto train_feature = sm::TermDocFeature(train_label, train_tweet);
        auto test_feature = sm::TermDocFeature(test_label, test_tweet);
        std::cout << "original train vocab : " << train_feature.get().storage().size() << std::endl;;

        sm::FeatureSelection fselect(train_feature, sm::selection::Chi2);

        auto selected = fselect.range(critical_value);
        std::cout << "selected train vocab : " << selected.get().storage().size() << std::endl;;

        std::cout << "\n\n====> naive bayes" << std::endl;
        learn_nb(selected, test_feature, "nb-fold-" + std::to_string(k) + "-cv-" + std::to_string(critical_value));
        std::cout << "\n\n====> bayes net" << std::endl;
        learn_bn(selected, test_feature, "bn-fold-" + std::to_string(k) + "-cv-" + std::to_string(critical_value));
    }
}

int main(int argc, char *argv[])
{
    sm::TextTransform transform;
    transform.add(sm::transform::HTMLEscape());
    //transform.add(sm::transform::Blacklist());
    transform.add(sm::transform::Apostrophe());
    transform.add(sm::transform::Emoticon());
    transform.add(sm::transform::Username());
    transform.add(sm::transform::Url());
    transform.add(sm::transform::Money());
    transform.add(sm::transform::Number());
    //transform.add(sm::transform::Blacklist());
    transform.add(sm::transform::Repeats());
    transform.add(sm::transform::Blacklist());
    transform.add(sm::transform::Stem());
    transform.add(sm::transform::Blacklist());
    transform.add(sm::transform::Punctuation(true));
    transform.add(sm::transform::Hashtag());
    //transform.add(sm::transform::Rare());

    auto table = sm::Table::load("Jan9-2012-tweets-clean.csv");
    auto clean = transform(table["tweet"]);

    //generate_mood_dataset(clean, table["emotion"], "mood.csv");

    table.update("tweet", clean);
    table.save("clean.csv");

    //const double critical = 9.236400; // 0.1
    const double critical = 15.0863; // 0.01
    //const double critical = 20.515000; // 0.001
    std::cout << "\nCRITICAL: " << critical << "\n";
    //cross_validation(5, clean, table["emotion"], critical);
    peek_cv(5, 1, clean, table["emotion"], critical);

    return 0;

    double ratio = 0.75;
    std::vector<std::string> train_tweet;
    std::vector<std::string> train_label;
    std::vector<std::string> test_tweet;
    std::vector<std::string> test_label;
    for (std::size_t i = 0; i < std::size_t(clean.size()); ++i)
    {
        if (i < std::size_t(clean.size() * ratio))
        {
            train_tweet.push_back(clean[i]);
            train_label.push_back(table["emotion"][i]);
        }
        else
        {
            test_tweet.push_back(clean[i]);
            test_label.push_back(table["emotion"][i]);
        }
    }
    auto train_feature = sm::TermDocFeature(train_label, train_tweet);
    auto test_feature = sm::TermDocFeature(test_label, test_tweet);

    sm::FeatureSelection fselect(train_feature, sm::selection::Chi2);
    std::cout << fselect;

	/*
	std::cout << "\n\n======50 features======\n";
    learn_bn(fselect.best(50), test_feature, "50");

	std::cout << "\n\n======250 features======\n";
    learn_bn(fselect.best(250), test_feature, "250");

	std::cout << "\n\n======500 features======\n";
    learn_bn(fselect.best(500), test_feature, "500");

	std::cout << "\n\n======1500 features======\n";
    learn_bn(fselect.best(1500), test_feature, "1500");

	std::cout << "\n\n======3000 features======\n";
    learn_bn(fselect.best(3000), test_feature, "3000");

	std::cout << "\n\n======all features======\n";
    learn_bn(train_feature, test_feature, "all");
    */

    return 0;
}
