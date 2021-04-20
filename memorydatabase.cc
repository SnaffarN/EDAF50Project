#include "memorydatabase.h"

#include <algorithm>
#include <string>
#include <functional>
#include <vector>
#include <map>

using namespace std;

MemoryDatabase::MemoryDatabase() : newsGroups() {}

bool MemoryDatabase::createNewsGroup(string title) {
  int id = getNewsGroupCount();
  if(!containsNewsGroup(title)) {
    incNewsGroupCount();
    newsGroups.insert(pair<int, NewsGroup>{id, NewsGroup(id, title)});
    return true;
  }
  return false;
}

bool MemoryDatabase::deleteNewsGroup(int id) {
  if(containsNewsGroup(id)) {
    newsGroups.erase(id);
    return true;
  }
  return false;
}

NewsGroup& MemoryDatabase::getNewsGroup(int id) {
  try {
    return newsGroups.at(id);
  } catch (exception& e) {
    throw "NewsGroup doesnt exist";
  }
}

NewsGroup& MemoryDatabase::getNewsGroup(string title) {
  for (auto& it : newsGroups) {
      if (it.second.getTitle() == title) {
        return it.second;
      }
  }
  throw "NewsGroup doesnt exist";
}

vector<NewsGroup> MemoryDatabase::getNewsGroups() {
  vector<NewsGroup> result;
  result.reserve(newsGroups.size());
  for(auto kv : newsGroups) {
    result.push_back(kv.second);
  }
  return result;
}

bool MemoryDatabase::containsNewsGroup(int id) {
  auto result = newsGroups.find(id);
  if(result != newsGroups.end()) {
    return true;
  } else {
    return false;
  }
}

bool MemoryDatabase::containsNewsGroup(string title) {
  for (auto& it : newsGroups) {
      if (it.second.getTitle() == title) {
        return true;
      }
  }
  return false;
}

bool MemoryDatabase::createArticle(int newsGroupID, string title, string author, string text) {
  int articleID = getNewsGroup(newsGroupID).getArticleCount();
  getNewsGroup(newsGroupID).incArticleCount();
  return getNewsGroup(newsGroupID).createArticle(articleID, title, author, text);
}

bool MemoryDatabase::deleteArticle(int newsGroupID, int articleID) {
  return getNewsGroup(newsGroupID).deleteArticle(articleID);

}

Article MemoryDatabase::getArticle(int newsGroupID, int articleID) {
  return getNewsGroup(newsGroupID).getArticle(articleID);
}

vector<Article> MemoryDatabase::getArticles(int newsGroupID) {
  return getNewsGroup(newsGroupID).getArticles();
}

bool MemoryDatabase::containsArticle(int newsGroupID, int articleID) {
  return getNewsGroup(newsGroupID).contains(articleID);
}

int MemoryDatabase::hashTitle(string title) {
  hash<string> hasher;
  return static_cast<int>(hasher(title));
}

int MemoryDatabase::getNewsGroupCount() {
  return newsGroupCount;
}
int MemoryDatabase::incNewsGroupCount() {
  return ++newsGroupCount;
}
