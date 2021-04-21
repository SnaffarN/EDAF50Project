#ifndef DISKDATABASE_H
#define DISKDATABASE_H

#include "database.h"

#include <string>
#include <vector>
#include <map>

using std::string;

class DiskDatabase: public Database {
public:
  DiskDatabase();
  bool createNewsGroup(string title) override;
  bool deleteNewsGroup(int id) override;
  NewsGroup& getNewsGroup(int id) override;
  NewsGroup& getNewsGroup(string title) override;
  vector<NewsGroup> getNewsGroups() override;
  bool containsNewsGroup(int id) override;
  bool containsNewsGroup(string title) override;

  void read_newsgroups();
  void read_articles();
  void create_newsgroup_on_disk(int id, string title);
  void create_article_on_disk(int articleID, int newsGroupID, string title, string author, string text);
  void delete_newsgroup_on_disk(int newsGroupID);
  void delete_article_on_disk(int newsGroupID, int articleID);

  bool createArticle(int id, string title, string author, string text) override;
  bool deleteArticle(int newsGroupID, int articleID) override;
  Article getArticle(int newsGroupID, int articleID) override;
  vector<Article> getArticles(int newsGroupID) override;
  bool containsArticle(int newsGroupID, int articleID) override;

private:
  int hashTitle(string title);
  int getNewsGroupCount();
  int incNewsGroupCount();
  int newsGroupCount = 0;
  map<int, NewsGroup> newsGroups;
};
#endif
