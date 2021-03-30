#ifndef DATABASE_H
#define DATABASE_H

#include "article.h"
#include "newsgroup.h"

#include <unordered_map>
#include <vector>
#include <string>

using namespace std;
class Database {
  public:
    virtual ~Database() {};
    virtual bool createNewsGroup(string title) = 0;
    virtual bool deleteNewsGroup(int id) = 0;
    virtual NewsGroup getNewsGroup(int id) = 0;
    virtual NewsGroup getNewsGroup(string title) = 0;
    virtual vector<NewsGroup> getNewsGroups() = 0;
    virtual bool containsNewsGroup(int id) = 0;

    virtual bool createArticle(int newsGroupID, string title, string author, string text) = 0;
    virtual bool deleteArticle(int newsGroupID, int articleID) = 0;
    virtual Article getArticle(int newsGroupID, int articleID) = 0;
    virtual vector<Article> getArticles(int newsGroupID) = 0;
    virtual bool containsArticle(int newsGroupID, int articleID) = 0;

  private:
    virtual int hashTitle(string title) = 0;
    virtual int getCount() = 0;
};

#endif
