#include "connection.h"
#include "connectionclosedexception.h"
#include "protocolviolationexception.h"
#include "server.h"
#include "messagehandler.h"
#include "database.h"
#include "memorydatabase.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

void listNewsGroup(MessageHandler messageHandler, Database& database) {
  try {
    if(messageHandler.recvCode() == Protocol::COM_END) {
      vector<NewsGroup> newsGroups = database.getNewsGroups();
      messageHandler.sendCode(Protocol::ANS_LIST_NG);
      messageHandler.sendIntParameter(static_cast<int>(newsGroups.size()));
      for(NewsGroup n : newsGroups) {
        messageHandler.sendIntParameter(n.getID());
        messageHandler.sendStringParameter(n.getTitle());
      }
      messageHandler.sendCode(Protocol::ANS_END);
    } else throw ProtocolViolationException();
  } catch (exception&) {
    throw;
  }
}

void createNewsGroup(MessageHandler messageHandler, Database& database) {
  try {
    string title = messageHandler.recvStringParameter();
    if(messageHandler.recvCode() == Protocol::COM_END) {
      messageHandler.sendCode(Protocol::ANS_CREATE_NG);
      if(database.createNewsGroup(title)) {
        messageHandler.sendCode(Protocol::ANS_ACK);
      } else {
        messageHandler.sendCode(Protocol::ANS_NAK);
        messageHandler.sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
      }
      messageHandler.sendCode(Protocol::ANS_END);
    } else throw ProtocolViolationException();
  } catch (exception&) {
    throw;
  }
}

void deleteNewsGroup(MessageHandler messageHandler, Database& database) {
  try {
    int id = messageHandler.recvIntParameter();
    if(messageHandler.recvCode() == Protocol::COM_END) {
      messageHandler.sendCode(Protocol::ANS_DELETE_NG);
      if(database.deleteNewsGroup(id)) {
        messageHandler.sendCode(Protocol::ANS_ACK);
      } else {
        messageHandler.sendCode(Protocol::ANS_NAK);
        messageHandler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
      }
      messageHandler.sendCode(Protocol::ANS_END);
    } else throw ProtocolViolationException();
  } catch (exception&) {
    throw;
  }
}

void listArticles(MessageHandler messageHandler, Database& database) {
  try {
    int id = messageHandler.recvIntParameter();
    if(messageHandler.recvCode() == Protocol::COM_END) {
      messageHandler.sendCode(Protocol::ANS_LIST_ART);
      if(database.containsNewsGroup(id)) {
        vector<Article> articles = database.getArticles(id);
        messageHandler.sendCode(Protocol::ANS_ACK);
        messageHandler.sendIntParameter(static_cast<int>(articles.size()));
        for(Article a : articles) {
          messageHandler.sendIntParameter(a.getID());
          messageHandler.sendStringParameter(a.getTitle());
        }
      } else {
        messageHandler.sendCode(Protocol::ANS_NAK);
        messageHandler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
      }
      messageHandler.sendCode(Protocol::ANS_END);
    } else throw ProtocolViolationException();
  } catch (exception&) {
    throw;
  }
}

void createArticle(MessageHandler messageHandler, Database& database) {
  try {
    int id = messageHandler.recvIntParameter();
    string title = messageHandler.recvStringParameter();
    string author = messageHandler.recvStringParameter();
    string text = messageHandler.recvStringParameter();
    if(messageHandler.recvCode() == Protocol::COM_END) {
      messageHandler.sendCode(Protocol::ANS_CREATE_ART);
      if(database.createArticle(id, title, author, text)) {
        messageHandler.sendCode(Protocol::ANS_ACK);
      } else {
        messageHandler.sendCode(Protocol::ANS_NAK);
        messageHandler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
      }
      messageHandler.sendCode(Protocol::ANS_END);
    } else throw ProtocolViolationException();
  } catch (exception&) {
    throw;
  }
}

void deleteArticle(MessageHandler messageHandler, Database& database) {
  try {
    int newsGroupID = messageHandler.recvIntParameter();
    int articleID = messageHandler.recvIntParameter();
    if(messageHandler.recvCode() == Protocol::COM_END) {
      messageHandler.sendCode(Protocol::ANS_DELETE_ART);
      if(database.containsNewsGroup(newsGroupID)) {
        if(database.deleteArticle(newsGroupID, articleID)) {
          messageHandler.sendCode(Protocol::ANS_ACK);
        } else {
          messageHandler.sendCode(Protocol::ANS_NAK);
          messageHandler.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
        }
      } else {
        messageHandler.sendCode(Protocol::ANS_NAK);
        messageHandler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
      }
      messageHandler.sendCode(Protocol::ANS_END);
    } else throw ProtocolViolationException();
  } catch (exception&) {
    throw;
  }
}

void getArticle(MessageHandler messageHandler, Database& database) {
  try {
    int newsGroupID = messageHandler.recvIntParameter();
    int articleID = messageHandler.recvIntParameter();
    if(messageHandler.recvCode() == Protocol::COM_END) {
      messageHandler.sendCode(Protocol::ANS_GET_ART);
      if(database.containsNewsGroup(newsGroupID)) {
        if(database.containsArticle(newsGroupID, articleID)) {
          messageHandler.sendCode(Protocol::ANS_ACK);
          Article article = database.getArticle(newsGroupID, articleID);
          messageHandler.sendStringParameter(article.getTitle());
          messageHandler.sendStringParameter(article.getAuthor());
          messageHandler.sendStringParameter(article.getText());
        } else {
          messageHandler.sendCode(Protocol::ANS_NAK);
          messageHandler.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
        }
      } else {
        messageHandler.sendCode(Protocol::ANS_NAK);
        messageHandler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
      }
      messageHandler.sendCode(Protocol::ANS_END);
    } else throw ProtocolViolationException();
  } catch (exception&) {
    throw;
  }
}

void readMessage(MessageHandler messageHandler, Database& database) {
  Protocol code = messageHandler.recvCode();
  switch(code) {
    case Protocol::COM_LIST_NG:     listNewsGroup(messageHandler, database);
                                    break;
    case Protocol::COM_CREATE_NG:   createNewsGroup(messageHandler, database);
                                    break;
    case Protocol::COM_DELETE_NG:   deleteNewsGroup(messageHandler, database);
                                    break;
    case Protocol::COM_LIST_ART:    listArticles(messageHandler, database);
                                    break;
    case Protocol::COM_CREATE_ART:  createArticle(messageHandler, database);
                                    break;
    case Protocol::COM_DELETE_ART:  deleteArticle(messageHandler, database);
                                    break;
    case Protocol::COM_GET_ART:     getArticle(messageHandler, database);
                                    break;
    default:  throw ProtocolViolationException();
  }
}

Server init(int argc, char* argv[])
{
  if (argc != 2) {
    cerr << "Usage: myserver port-number" << endl;
    exit(1);
  }

  int port = -1;
  try {
    port = stoi(argv[1]);
  } catch (exception& e) {
    cerr << "Wrong format for port number. " << e.what() << endl;
    exit(2);
  }

  Server server(port);
  if (!server.isReady()) {
    cerr << "Server initialization error." << endl;
    exit(3);
  }
  return server;
}

int main(int argc, char* argv[])
{
  auto server = init(argc, argv);
  Database *database = new MemoryDatabase();
  while(true) {
    auto conn = server.waitForActivity();
    if(conn != nullptr) {
      try {
        MessageHandler messageHandler(conn);
        readMessage(messageHandler, (*database));
      } catch (ConnectionClosedException&) {
        server.deregisterConnection(conn);
        cout << "Client closed connection" << endl;
      } catch (ProtocolViolationException&) {
        //cout << "Error occured while communicating with a client." << endl;
      } catch (exception& e) {
        cout << "An error occured." << e.what() << endl;
      }
    } else {
      conn = make_shared<Connection>();
      server.registerConnection(conn);
      cout << "New client connects" << endl;
    }
  }
  return 0;
}
