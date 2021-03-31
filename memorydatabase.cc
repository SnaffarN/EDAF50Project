#include "memorydatabase.h"

#include <string>
#include <functional>
#include <vector>
#include <unordered_map>

using namespace std;

MemoryDatabase::MemoryDatabase() : newsGroups() {}

bool MemoryDatabase::createNewsGroup(string title) {
  int id = hashTitle(title);
  if(!containsNewsGroup(id)) {
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
  int id = hashTitle(title);
  return getNewsGroup(id);
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

bool MemoryDatabase::createArticle(int newsGroupID, string title, string author, string text) {
  return getNewsGroup(newsGroupID).createArticle(getCount(), title, author, text);
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

int MemoryDatabase::getCount() {
  return count++;
}
