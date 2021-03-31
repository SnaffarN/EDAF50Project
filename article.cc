#include "article.h"

#include <string>

using std::string;

Article::Article(const int& i, const string& t, const string& a, const string& txt) :
  id(i), title(t), author(a), text(txt) {}

Article::Article(const Article &a1) {
  id = a1.id;
  title = a1.title;
  author = a1.author;
  text = a1.text;
}

int Article::getID() const{
  return id;
}
string Article::getTitle() const{
  return title;
}
string Article::getAuthor() const{
  return author;
}
string Article::getText() const{
  return text;
}
