#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include "article.h"

#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::vector;
using std::unordered_map;

class NewsGroup {
  public:
    NewsGroup(int id, string title);
    bool createArticle(int articleID, string title, string author, string text);
    bool deleteArticle(int articleID);
    int getID();
    string getTitle();
    Article getArticle(int articleID);
    vector<Article> getArticles();
    bool contains(int articleID);
  private:
    int id;
    string title;
    unordered_map<int, Article> articles;
};
#endif
