#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

using std::string;

class Article {
  public:
    Article(const int& id, const string& title, const string& author, const string& text);
    Article(const Article &a1);
    int getID() const;
    string getTitle() const;
    string getAuthor() const;
    string getText() const;
  private:
    int id;
    string title;
    string author;
    string text;
};
#endif
