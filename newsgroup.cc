#include "newsgroup.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <pair>

using std::string;
using std::vector;
using std::unordered_map;
using std::pair;

NewsGroup::NewsGroup(int i, string t) : id(i), title(t) {}

bool NewsGroup::createArticle(int articleID, string title, string author, string text) {
  if(!contains(articleID)) {
    articles.insert(pair<int, Article>{articleID, Article(articleID, title, author, text)});
    return true;
  }
  return false;
}

bool NewsGroup::deleteArticle(int articleID) {
  if(contains(articleID)) {
    articles.erase(articleID);
    return true;
  }
  return false;
}

int NewsGroup::getID() {
  return id;
}

string NewsGroup::getTitle() {
  return title;
}

Article NewsGroup::getArticle(int articleID) {
  try {
    Article result = articles.at(articleID);
  } catch (exception& e) {
    result = NULL;
  }
  return result;
}

vector<Article> NewsGroup::getArticles() {
  vector<Article> result;
  result.reserve(articles.size());
  for(auto kv : articles) {
    result.push_back(kv.second);
  }
  return result;
}

bool contains(int articleID) {
  auto result = articles.find(id);
  if(result != articles.end()) {
    return true;
  } else {
    return false;
  }
}
