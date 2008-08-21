#include <string>
#include <fstream>
#include <vector>
#include "tx.hpp"

using namespace std;

int main(int argc, char* argv[]){
  if (argc != 2){
    fprintf(stderr,"%s index\n",argv[0]);
    return -1;
  }

  tx_tool::tx t;
  if (t.read(argv[1]) == -1){
    fprintf(stderr, "%s", t.getErrorLog().c_str());
    fprintf(stderr,"cannot read index %s\n",argv[1]);
    return -1;
  }
  fprintf(stderr,"%s", t.getResultLog().c_str());

  string query;
  for (;;){
    putchar('>');
    getline(cin,query);
    if (query.size() == 0) break;
		
    // prefixSearch
    {
      size_t retLen = 0;
      const tx_tool::uint id = t.prefixSearch(query.c_str(),query.size(),retLen);
      printf("prefixSearch ");
      if (id == tx_tool::tx::NOTFOUND){
	printf("not found\n");
      } else {
	string retKey;
	size_t retLen = t.reverseLookup(id, retKey);
	printf("id:%u len:%u lookup:%s\n", id, retLen, retKey.c_str());
      }
    }

    // expandSearch
    {
      vector<string> ret;
      const tx_tool::uint retNum = t.expandSearch(query.c_str(),query.size(), ret, 10);
      printf("expansionSearch %u\n", retNum);
      for (vector<string>::const_iterator it = ret.begin(); it != ret.end(); it++){
	printf("%s\n",it->c_str());
      }
    }

    // commonPrefixSearch
    {
      vector<string> ret;
      vector<tx_tool::uint> retID;
      const tx_tool::uint retNum = t.commonPrefixSearch(query.c_str(),query.size(), ret, retID, 10);
      printf("commonPrefixSearch %u\n", retNum);
      for (size_t i = 0; i < ret.size(); i++){
	printf("%s (id=%d)\n",ret[i].c_str(), retID[i]);
      }
    }

    // predictiveSearch
    {
      vector<string> ret;
      vector<tx_tool::uint> retID;
      const tx_tool::uint retNum = t.predictiveSearch(query.c_str(),query.size(), ret, retID, 10);
      printf("predictiveSearch %u\n", retNum);
      for (size_t i = 0; i < ret.size(); i++){
	printf("%s (id=%d)\n",ret[i].c_str(), retID[i]);
      }
    }
  }
  return 0;
}