#include <sentimental/transform/emoticon.h>

using namespace sm::transform;

std::unordered_set<std::string> Emoticon::anger = {
    ":‑X", ":X", ":‑#", ":#", ":‑&", ":&"
};

std::unordered_set<std::string> Emoticon::disgust = {
    "DX", "D;", ":‑/", ":/", ":‑.", ">:\\", ">:/", ":\\",
    "=/", "=\\", ":L", "=L", ":S", ":-|", ":|", "|‑O",
    "<:‑|", "-_-", "._.", "--"
};

std::unordered_set<std::string> Emoticon::fear = {
    "D:<", "D‑':", "D:", "D=", "D8", ":‑###..", ":###..",
    ">.<", "(>_<)", "(>_<)>"
};

std::unordered_set<std::string> Emoticon::joy = {
    ":‑)", ":)", ":-]", ":]", ":3", ":3", ":->", ":>",
    "8-)", "8)", ":-}", ":}", ":o)", ":c)", ":^)", "=]",
    "=)", ":‑D", ":D", "8‑D", "8D", "x‑D", "xD", "X‑D",
    "XD", "=D", "=3", "B^D", ":-))", ":'‑)", ":')", ":-*",
    ":*", ":×", ";‑)", ";)", "*-)", "*)", ";‑]", ";]",
    ";^)", ":‑,", ";D", ":‑P", ":P", "X‑P", "XP", "x-p",
    "xp", ":‑p", ":p", ":‑Þ", ":Þ", ":‑þ", ":þ", ":‑b",
    ":b", "d:", "=p", ">:P", "O:‑)", "O:)", "0:‑3", "0:3",
    "0:‑)", "0:)", "0;^)", ">:‑)", ">:)", "}:‑)", "}:)",
    "3:‑)", "3:)", ">;)", "|;‑)", "#‑)", "%‑)", "%)", "<3",
    "\\o/", "*\0/*", "^_^", "(^_-)", "^_-", "(^_^)/", "lol",
    "-.-"
};

std::unordered_set<std::string> Emoticon::sadness = {
    ":‑(", ":(", ":‑c", ":c", ":‑<", ":<", ":‑[", ":[",
    ":-||", ">:[", ":{", ":@", ">:(", ":'‑(", ":'(", "</3"
    "<\\3", "v.v"
};

std::unordered_set<std::string> Emoticon::surprise = {
    ":‑O", ":O", ":‑o", ":o", ":‑0", "8‑0", ">:O", ":$",
    "O_O", "o‑o", "O_o", "o_O", "o_o", "O-O"
};

std::string Emoticon::operator ()(const std::string &token)
{
    if (anger.find(token) != anger.end())
        return "{{emoticon:angry}}";
    if (disgust.find(token) != disgust.end())
        return "{{emoticon:disgust}}";
    if (fear.find(token) != fear.end())
        return "{{emoticon:fear}}";
    if (joy.find(token) != joy.end())
        return "{{emoticon:joy}}";
    if (sadness.find(token) != sadness.end())
        return "{{emoticon:sadness}}";
    if (surprise.find(token) != surprise.end())
        return "{{emoticon:surprise}}";
    return token;
}
