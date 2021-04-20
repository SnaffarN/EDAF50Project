#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include "article.h"

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

class NewsGroup {
  public:
    NewsGroup(int id, string title);
    NewsGroup(const NewsGroup &n1);
    bool createArticle(const int& articleID, const string& title, const string& author, const string& text);
    bool deleteArticle(int articleID);
    int getID();
    string getTitle();
    Article getArticle(int articleID);
    vector<Article> getArticles();
    bool contains(int articleID);
    int getArticleCount();
    int incArticleCount();
  private:
    int id;
    int articleCount = 0;
    string title;
    map<int, Article> articles;
};
#endif
