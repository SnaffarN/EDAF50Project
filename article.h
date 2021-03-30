#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

using std::string;

class Article {
  public:
    Article(int id, string title, string author, string text);
    int getID();
    string getTitle();
    string getAuthor();
    string getText();
  private:
    int id;
    string title;
    string author;
    string text;
};
#endif
