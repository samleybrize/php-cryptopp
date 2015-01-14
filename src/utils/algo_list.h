#ifndef PHP_ALGO_LIST_H
#define PHP_ALGO_LIST_H

#include <string>
#include <vector>

using namespace std;

class AlgoList
{
public:
    void addAlgo(const std::string algoName, const std::string hashClassname);
    vector<string> getAlgoList();
    string getAlgoClass(const string &algoName);

protected:
    vector<string> algoList;
    vector<string> classList;
};

#endif /* PHP_ALGO_LIST_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
