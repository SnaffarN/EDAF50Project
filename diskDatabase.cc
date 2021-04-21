#include "diskDatabase.h"
#include <cstdlib>
#include <algorithm>
#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

DiskDatabase::DiskDatabase() : newsGroups() {
  read_newsgroups();
  read_articles();
  getNewsGroups();
}

void DiskDatabase::read_newsgroups() {
  ifstream infile("newsgroups/newsgroups.txt");
  string str;
  
  while (getline(infile, str)) {
    string id = (str.substr(0, str.find(" ")));
    string title = str.substr(str.find(" ") + 1);
    
    if(!containsNewsGroup(title)) {
      incNewsGroupCount();
      newsGroups.insert(pair<int, NewsGroup>{stoi(id), NewsGroup(stoi(id), title)});
    }
  }
  infile.close();
 }

void DiskDatabase::read_articles() {
  //bool DiskDatabase::createArticle(int newsGroupID, string title, string author, string text) {
  ifstream infile("articles/articles.txt");
  string str;
  
  while (getline(infile, str)) { 
    int group_id = stoi(str.substr(0, str.find(" ")));
    int article_id = stoi(str.substr(str.find(" ") + 1));  
    getline(infile, str);
    string title = str;
    getline(infile, str);
    string author = str;

    string text = "";
    while (getline(infile, str)) {
      if (str == "</>") {
        break;
      } 
      text += str + "\n";
    }
    getNewsGroup(group_id).incArticleCount();
    getNewsGroup(group_id).createArticle(article_id, title, author, text);    
  }
  infile.close();
}

void DiskDatabase::create_newsgroup_on_disk(int id, string title) {
  ofstream group("newsgroups/newsgroups.txt", ios_base::app);
  group << to_string(id) << " " << title << endl;
  group.close();
}

bool DiskDatabase::createNewsGroup(string title) {
  int id = getNewsGroupCount();
  if(!containsNewsGroup(title)) {
    incNewsGroupCount();
    create_newsgroup_on_disk(id, title);
    newsGroups.insert(pair<int, NewsGroup>{id, NewsGroup(id, title)});
    return true;
  }
  return false;
}

void DiskDatabase::delete_newsgroup_on_disk(int newsGroupID) {
  ifstream infile("newsgroups/newsgroups.txt");
  ofstream out("newsgroups/newsgroups_new.txt");
  string str;
  while (getline(infile, str)) {
    string id = str.substr(0, str.find(" "));    
    if (to_string(newsGroupID) != id) {
      out << str << endl;
    }
  }
  infile.close();
  out.close();
  rename("newsgroups/newsgroups_new.txt", "newsgroups/newsgroups.txt");
}

bool DiskDatabase::deleteNewsGroup(int id) {
  if(containsNewsGroup(id)) {
    cout << "delete: " << to_string(id) << endl;
    vector<Article> v = getNewsGroup(id).getArticles();
    for (auto a : v) {
      delete_article_on_disk(id, a.getID());
    }
    newsGroups.erase(id);
    delete_newsgroup_on_disk(id);
    return true;
  }
  return false;
}

NewsGroup& DiskDatabase::getNewsGroup(int id) {
  try {
    return newsGroups.at(id);
  } catch (exception& e) {
    throw "NewsGroup doesnt exist";
  }
}

NewsGroup& DiskDatabase::getNewsGroup(string title) {
  for (auto& it : newsGroups) {
      if (it.second.getTitle() == title) {
        return it.second;
      }
  }
  throw "NewsGroup doesnt exist";
}


vector<NewsGroup> DiskDatabase::getNewsGroups() {
  vector<NewsGroup> result;
  result.reserve(newsGroups.size());
  for(auto kv : newsGroups) {
    result.push_back(kv.second);
  }
  return result;
}

bool DiskDatabase::containsNewsGroup(int id) {
  auto result = newsGroups.find(id);
  if(result != newsGroups.end()) {
    return true;
  } else {
    return false;
  }
}

bool DiskDatabase::containsNewsGroup(string title) {
  for (auto& it : newsGroups) {
      if (it.second.getTitle() == title) {
        return true;
      }
  }
  return false;
}

void DiskDatabase::create_article_on_disk(int articleID, int newsGroupID, string title, string author, string text) {
  ofstream article("articles/articles.txt", ios_base::app);
  article << to_string(newsGroupID) << " " << to_string(articleID) << "\n"
          << title << "\n" 
          << author << "\n" 
          << text << "\n"
          << "</>" << endl;
  article.close();
}

bool DiskDatabase::createArticle(int newsGroupID, string title, string author, string text) {
  int articleID = getNewsGroup(newsGroupID).getArticleCount();
  create_article_on_disk(articleID, newsGroupID, title, author, text);
  getNewsGroup(newsGroupID).incArticleCount();
  return getNewsGroup(newsGroupID).createArticle(articleID, title, author, text);
}

void DiskDatabase::delete_article_on_disk(int newsGroupID, int articleID) {
  ifstream infile("articles/articles.txt");
  ofstream out("articles/articles_new.txt");
  string str;
  while (getline(infile, str)) {
    if (to_string(newsGroupID) + " " + to_string(articleID) == str) {
      while (getline(infile, str)) {
        if (str == "</>") {
          break;
        }
      }
    } else {
      out << str << endl;
    }
  }
  infile.close();
  out.close();
  rename("articles/articles_new.txt", "articles/articles.txt");
}

bool DiskDatabase::deleteArticle(int newsGroupID, int articleID) {
  delete_article_on_disk(newsGroupID, articleID);
  return getNewsGroup(newsGroupID).deleteArticle(articleID);
}

Article DiskDatabase::getArticle(int newsGroupID, int articleID) {
  return getNewsGroup(newsGroupID).getArticle(articleID);
}

vector<Article> DiskDatabase::getArticles(int newsGroupID) {
  return getNewsGroup(newsGroupID).getArticles();
}

bool DiskDatabase::containsArticle(int newsGroupID, int articleID) {
  return getNewsGroup(newsGroupID).contains(articleID);
}

int DiskDatabase::hashTitle(string title) {
  hash<string> hasher;
  return static_cast<int>(hasher(title));
}

int DiskDatabase::getNewsGroupCount() {
  return newsGroupCount;
}
int DiskDatabase::incNewsGroupCount() {
  return ++newsGroupCount;
}
