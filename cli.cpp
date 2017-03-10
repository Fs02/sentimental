#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sentimental/table.h>
#include <sentimental/feature.h>
#include <sentimental/featureselection.h>
#include <sentimental/texttransform.h>
#include <sentimental/naivebayes.h>
#include <sentimental/bayesnet.h>
#include <sentimental/metrics/accuracy.h>

char* get_option(char **begin, char **end, const std::string &option, const std::string &option_alt = "")
{
    char **itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
        return *itr;

    if (option_alt.empty())
        return 0;

    itr = std::find(begin, end, option_alt);
    if (itr != end && ++itr != end)
        return *itr;

    return 0;
}

bool option_exists(char **begin, char **end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

/*
Usage: sentimental-cli [Operations] [options]
       sentimental-cli clean -s data.csv -o clean.csv -r tweet

Operations:
data:clean : clean csv
    -i, --input file.csv          Path of csv input
    -o, --output file.csv         Path of csv output
    -r, --row name              Selected row
data:split : split csv
    -i, --input file.csv          Path of csv input
    -o1, --output1 file.csv         Path of csv output
    -o2, --output2 file.csv         Path of csv output
    -s, --split 0.7                 Split number
feature : extract feature from csv
    -i --input file.csv         Path of csv input
    -o --output file.json        Path to save json output
    -t --text-row name      Text row name
    -l --labels-row name    Labels row name
fselect : select feature using chi2
    -i, --input feature.json    Path to input feature file
    -o, --input feature.json    Path to output feature file
    -b, --best 1000             Number of best feature to take
    -p, --percentage 50            Percentage of best feature to take
    default to biggest diff if -b or -p is not specified
naivebayes:train : train Naive Bayes model
    -i, --input feature.json    Path to input feature
    -o, --output model.json     Path to ouput model
naivebayes:test : Test data using Naive Bayes model
    -m, --model model.json      Path to naivebayes model
    -i, --input file.csv        Path to test file
    -t, --text-row name         Text row name
    -l, --labels-row name       Labels row name
*/
int main(int argc, char *argv[])
{
    if (argc <= 1)
        return 0;

    if (option_exists(argv, argv+argc, "data:clean"))
    {
        char *input = get_option(argv, argv+argc, "-i", "--input");
        char *output = get_option(argv, argv+argc, "-o", "--output");
        char *row = get_option(argv, argv+argc, "-r", "--row");
        if (!input || !output || !row)
            return 0;

        sm::TextTransform transform;
        transform.add(sm::transform::HTMLEscape());
        transform.add(sm::transform::Apostrophe());
        transform.add(sm::transform::Emoticon());
        transform.add(sm::transform::Username());
        transform.add(sm::transform::Url());
        transform.add(sm::transform::Money());
        transform.add(sm::transform::Number());
        transform.add(sm::transform::Repeats());
        transform.add(sm::transform::Stem());
        transform.add(sm::transform::Punctuation(true));
        transform.add(sm::transform::Hashtag());
        transform.add(sm::transform::Rare());

        auto table = sm::Table::load(input);
        auto clean = transform(table[row]);
        table.update(row, clean);
        table.save(output);
    }
    if (option_exists(argv, argv+argc, "data:split"))
    {
        char *input = get_option(argv, argv+argc, "-i", "--input");
        char *output1 = get_option(argv, argv+argc, "-o1", "--output1");
        char *output2 = get_option(argv, argv+argc, "-o2", "--output2");
        char *split = get_option(argv, argv+argc, "-s", "--split");
        if (!input || !output1 || !output2 || !split)
            return 0;

        float d_split = std::atof(split);
        auto table = sm::Table::load(input);
        sm::Table::Storage storage1;
        sm::Table::Storage storage2;
        for (auto it = table.storage().cbegin(); it != table.storage().cend(); ++it)
        {
            std::size_t split_pos = it->second.size() * d_split;
            storage1[it->first] = std::vector<std::string>(it->second.begin(), it->second.begin() + split_pos);
            storage2[it->first] = std::vector<std::string>(it->second.begin() + split_pos, it->second.end());
        }
        sm::Table(storage1).save(output1);
        sm::Table(storage2).save(output2);
    }
    else if (option_exists(argv, argv+argc, "feature"))
    {
        char *input = get_option(argv, argv+argc, "-i", "--input");
        char *output = get_option(argv, argv+argc, "-o", "--output");
        char *text_row = get_option(argv, argv+argc, "-t", "--text-row");
        char *labels_row = get_option(argv, argv+argc, "-l", "--labels-row");

        if (!input || !output || !text_row || !labels_row)
            return 0;

        std::ofstream file(output);
        auto table = sm::Table::load(input);
        auto feature = sm::TermDocFeature(table[labels_row], table[text_row]);
        file << sm::TermDocFeature::serialize(feature);
    }
    else if (option_exists(argv, argv+argc, "fselect"))
    {
        char *input = get_option(argv, argv+argc, "-i", "--input");
        char *output = get_option(argv, argv+argc, "-o", "--output");
        char *best = get_option(argv, argv+argc, "-b", "--best");
        char *percent = get_option(argv, argv+argc, "-p", "--percentage");

        if (!input || !output)
            return 0;

        Json::Value root;
        std::ifstream finput(input);
        finput >> root;
        finput.close();;

        auto feature = sm::TermDocFeature::deserialize(root);
        sm::FeatureSelection fselect(feature, sm::selection::Chi2);

        std::ofstream foutput(output);
        if (best)
        {
            foutput << sm::TermDocFeature::serialize(fselect.best(std::atoi(best)));
        }
        else if (percent)
        {
            double percentage = std::atof(percent);
            percentage = percentage < 0.0f ? 0.0f : percentage > 100.0f ? 100.0f : percentage;
            foutput << sm::TermDocFeature::serialize(fselect.percentage(percentage));
        }
        else
        {
            foutput << sm::TermDocFeature::serialize(fselect.biggest_diff());
        }
    }
    else if (option_exists(argv, argv+argc, "naivebayes:train"))
    {
        char *input = get_option(argv, argv+argc, "-i", "--input");
        char *output = get_option(argv, argv+argc, "-o", "--output");

        if (!input || !output)
            return 0;

        Json::Value root;
        std::ifstream finput(input);
        finput >> root;
        finput.close();;

        auto feature = sm::TermDocFeature::deserialize(root);

        sm::NaiveBayes nb;
        nb.train(feature);

        std::ofstream foutput(output);
        foutput << sm::NaiveBayes::serialize(nb);
    }
    else if (option_exists(argv, argv+argc, "naivebayes:test"))
    {
        char *model = get_option(argv, argv+argc, "-m", "--model");
        char *input = get_option(argv, argv+argc, "-i", "--input");
        char *text_row = get_option(argv, argv+argc, "-t", "--text-row");
        char *labels_row = get_option(argv, argv+argc, "-l", "--labels-row");
        char *report = get_option(argv, argv+argc, "-r", "--report");

        if (!model || !input || !text_row || !labels_row)
            return 0;

        std::ofstream fsummary;
        if (report)
            fsummary.open(report);
        else
            fsummary.open("report.csv");

        fsummary << "No,Predicted,Actual,Text" << std::endl;

        auto table = sm::Table::load(input);
        Json::Value root;
        std::ifstream fmodel(model);
        fmodel >> root;
        fmodel.close();;
        auto nb = sm::NaiveBayes::deserialize(root);
        std::vector<std::string> predicted;
        for (int i = 0; i < table[labels_row].size(); ++i)
        {
            predicted.push_back(nb.predict(table[text_row][i]));
            std::cout << "[" << i << "/" << table[labels_row].size() << "] "
                      << predicted.back()
                      << " vs "
                      << table[labels_row][i]
                      << " => " << table[text_row][i] << std::endl;

            fsummary << i+1 << ","
                     << predicted.back() << ","
                     << table[labels_row][i] << ",\""
                     << table[text_row][i] << "\"" << std::endl;
        }

        std::cout << "Accuracy : " << sm::metrics::accuracy_score(table[labels_row], predicted) << std::endl;
    }

    return 0;
}
