#include "article.h"

#include <string>

using std::string;

Article::Article(int i, string t, string a, string txt) :
  id(i), title(t), author(a), text(txt) {}

int Article::getID() {
  return id;
}
string Article::getTitle() {
  return title;
}
string Article::getAuthor() {
  return author;
}
string Article::getText() {
  return text;
}
