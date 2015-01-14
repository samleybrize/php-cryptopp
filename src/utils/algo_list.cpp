#include "algo_list.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

void AlgoList::addAlgo(const std::string algoName, const std::string hashClassname)
{
    algoList.push_back(algoName);
    classList.push_back(hashClassname);
}

vector<string> AlgoList::getAlgoList()
{
    vector<string> _algos(algoList);
    sort(_algos.begin(), _algos.end());
    return _algos;
}

string AlgoList::getAlgoClass(const string &algoName)
{
    vector<string>::iterator iterator = find(algoList.begin(), algoList.end(), algoName);

    if (iterator == algoList.end()) {
        // hash algorithm not found
        return "";
    } else {
        // hash algorithm found
        // return corresponding hash classname
        int pos = iterator - algoList.begin();
        return classList[pos];
    }
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
