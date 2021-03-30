#ifndef MEMORYDATABASE_H
#define MEMORYDATABASE_H

#include "database.h"

#include <string>
#include <vector>
#include <unordered_map>

using std::string;

class MemoryDatabase: public Database {
public:
  bool createNewsGroup(string title) override;
  bool deleteNewsGroup(int id) override;
  NewsGroup getNewsGroup(int id) override;
  NewsGroup getNewsGroup(string title) override;
  vector<NewsGroup> getNewsGroups() override;
  bool containsNewsGroup(int id) override;

  bool createArticle(int id, string title, string author, string text) override;
  bool deleteArticle(int newsGroupID, int articleID) override;
  Article getArticle(int newsGroupID, int articleID) override;
  vector<Article> getArticles(int newsGroupID) override;
  bool containsArticle(int newsGroupID, int articleID) override;

private:
  int hashTitle(string title) override;
  int getCount() override;
  int count = 0;
  unordered_map<int, NewsGroup> newsGroups;
};
#endif
