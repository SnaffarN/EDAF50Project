#include "newsgroup.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <exception>
#include <iostream>

using std::string;
using std::vector;
using std::unordered_map;
using std::pair;
using std::exception;
using std::cout;
using std::endl;

NewsGroup::NewsGroup(int i, string t) : id(i), title(t), articles() {
  cout << "Creating NewsGroup: " << t << " with ID: " << i << endl;
}

NewsGroup::NewsGroup(const NewsGroup &n1) {
  id = n1.id;
  title = n1.title;
  articles = n1.articles;
}

bool NewsGroup::createArticle(const int& articleID, const string& title, const string& author, const string& text) {
  if(!contains(articleID)) {
    articles.insert(std::make_pair(articleID, Article(articleID, title, author, text)));
    cout << "Creating article: " << title << ". The number of articles is now: " << articles.size() << endl;
    cout << "I am newsgroup: " << getTitle() << " with id: " << getID() << endl;
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
    return articles.at(articleID);
  } catch (exception& e) {
    throw "Cant find article";
  }
}

vector<Article> NewsGroup::getArticles() {
  vector<Article> result;
  result.reserve(articles.size());
  for(auto kv : articles) {
    result.push_back(kv.second);
  }
  return result;
}

bool NewsGroup::contains(int articleID) {
  auto result = articles.find(articleID);
  if(result != articles.end()) {
    return true;
  } else {
    return false;
  }
}
