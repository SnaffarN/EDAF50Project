#include "connection.h"
#include "connectionclosedexception.h"
#include "protocolviolationexception.h"
#include "messagehandler.h"
#include "protocol.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>
#include <limits>

using namespace std;


string help() {
  return "1. List newsgroups\n"
         "2. Create newsgroup\n"
         "3. Delete newsgroup\n"
         "4. List articles\n"
         "5. Create article\n"
         "6. Delete article\n"
         "7. View article\n"
         "8. Exit";
}

string listNewsGroups(MessageHandler messageHandler) {
  try {
    string result;
    messageHandler.sendCode(Protocol::COM_LIST_NG);
    messageHandler.sendCode(Protocol::COM_END);
    messageHandler.recvCode();
    int nbrNewsGroups = messageHandler.recvIntParameter();
    for (int i = 0; i < nbrNewsGroups; i++) {
      string id = to_string(messageHandler.recvIntParameter());
      string name = messageHandler.recvStringParameter();
      result = result + id + ": " + name + "\n";
    }
    messageHandler.recvCode();
    return result;
  } catch (exception&) {
    throw;
  }
}

string createNewsGroup(MessageHandler messageHandler) {
  try {
    string result;
    messageHandler.sendCode(Protocol::COM_CREATE_NG);
    char c[256];
    cout << "Enter name of Newsgroup you wish to create: ";
    cin.ignore();
    cin.getline(c, 256);
    string name(c);
    messageHandler.sendStringParameter(name);
    messageHandler.sendCode(Protocol::COM_END);
    messageHandler.recvCode();
    Protocol code = messageHandler.recvCode();
    if(code == Protocol::ANS_ACK) {
      messageHandler.recvCode();
      return "Newsgroup: " + name + " was created.";
    } else if (code == Protocol::ANS_NAK) {
      if(messageHandler.recvCode() == Protocol::ERR_NG_ALREADY_EXISTS) {
        messageHandler.recvCode();
        return "Newsgroup: " + name + " was not created because a newsgroup with that name already exists.";
      } else {
        return "Newsgroup: " + name + " was not created and no reason was provided.";
      }
    } else {
      throw ProtocolViolationException();
    }
  } catch (exception&) {
    throw;
  }
}

string deleteNewsGroup(MessageHandler messageHandler) {
  try {
    string result;
    messageHandler.sendCode(Protocol::COM_DELETE_NG);
    int id;
    cout << "Enter id of Newsgroup you wish to delete: ";
    cin >> id;
    messageHandler.sendIntParameter(id);
    messageHandler.sendCode(Protocol::COM_END);
    messageHandler.recvCode();
    Protocol code = messageHandler.recvCode();
    if(code == Protocol::ANS_ACK) {
      messageHandler.recvCode();
      return "Newsgroup with id: " + to_string(id) + " was deleted.";
    } else if (code == Protocol::ANS_NAK) {
      if(messageHandler.recvCode() == Protocol::ERR_NG_DOES_NOT_EXIST) {
        messageHandler.recvCode();
        return "Newsgroup with id: " + to_string(id) + ", was not deleted because a newsgroup with that id doesn't exists.";
      } else {
        return "Newsgroup with id: " + to_string(id) + ", was not deleted and no reason was provided.";
      }
    } else {
      throw ProtocolViolationException();
    }
  } catch (exception&) {
    throw;
  }
}

string listArticles(MessageHandler messageHandler) {
  try {
    int newsGroupID;
    cout << "Enter id of the newsgroup which articles you wish to list: ";
    cin >> newsGroupID;
    string result;
    messageHandler.sendCode(Protocol::COM_LIST_ART);
    messageHandler.sendIntParameter(newsGroupID);
    messageHandler.sendCode(Protocol::COM_END);
    messageHandler.recvCode();
    Protocol code = messageHandler.recvCode();
    if(code == Protocol::ANS_ACK) {
      int nbrArticles = messageHandler.recvIntParameter();
      for (int i = 0; i < nbrArticles; i++) {
        string id = to_string(messageHandler.recvIntParameter());
        string name = messageHandler.recvStringParameter();
        result = result + id + ": " + name + "\n";
      }
    } else if (code == Protocol::ANS_NAK) {
      if(messageHandler.recvCode() == Protocol::ERR_NG_DOES_NOT_EXIST) {
        messageHandler.recvCode();
        return "Newsgroup with id: " + to_string(newsGroupID) + " does not exist.";
      } else {
        return "Newsgroup with id: " + to_string(newsGroupID) + " could not list articles.";
      }
    } else {
      throw ProtocolViolationException();
    }
    messageHandler.recvCode();
    return result;
  } catch (exception&) {
    throw;
  }
}

string createArticle(MessageHandler messageHandler) {
  try {
    int newsGroupID;
    char c[2000];
    cout << "Enter id of the newsgroup you want to create article under: ";
    cin >> newsGroupID;
    cout << "Enter title: ";
    cin.ignore();
    cin.getline(c, 2000);
    string title(c);
    cout << "Enter author: ";
    cin.getline(c, 2000);
    string author(c);
    cout << "Enter text: ";
    cin.getline(c, 2000);
    string text(c);
    messageHandler.sendCode(Protocol::COM_CREATE_ART);
    messageHandler.sendIntParameter(newsGroupID);
    messageHandler.sendStringParameter(title);
    messageHandler.sendStringParameter(author);
    messageHandler.sendStringParameter(text);
    messageHandler.sendCode(Protocol::COM_END);
    messageHandler.recvCode();
    Protocol code = messageHandler.recvCode();
    if(code == Protocol::ANS_ACK) {
      messageHandler.recvCode();
      return "Article: " + title + " was created.";
    } else if (code == Protocol::ANS_NAK) {
      if(messageHandler.recvCode() == Protocol::ERR_NG_DOES_NOT_EXIST) {
        messageHandler.recvCode();
        return "Newsgroup with id: " + to_string(newsGroupID) + " does not exist.";
      } else {
        return "Newsgroup with id: " + to_string(newsGroupID) + " could not list articles.";
      }
    } else {
      throw ProtocolViolationException();
    }
  } catch (exception&) {
    throw;
  }
}

string deleteArticle(MessageHandler messageHandler) {
  try {
    int newsGroupID;
    int articleID;
    cout << "Enter id of the newsgroup you want to delete article under: ";
    cin >> newsGroupID;
    cout << "Enter id of the article you want to delete: ";
    cin >> articleID;
    messageHandler.sendCode(Protocol::COM_DELETE_ART);
    messageHandler.sendIntParameter(newsGroupID);
    messageHandler.sendIntParameter(articleID);
    messageHandler.sendCode(Protocol::COM_END);
    messageHandler.recvCode();
    Protocol code = messageHandler.recvCode();
    if(code == Protocol::ANS_ACK) {
      messageHandler.recvCode();
      return "Article with id: " + to_string(articleID) + " was deleted";
    } else if (code == Protocol::ANS_NAK) {
      if(messageHandler.recvCode() == Protocol::ERR_NG_DOES_NOT_EXIST) {
        messageHandler.recvCode();
        return "Newsgroup with id: " + to_string(newsGroupID) + " does not exist.";
      } else {
        return "Couldn't delete article and no reason was given.";
      }
    } else {
      throw ProtocolViolationException();
    }
  } catch (exception&) {
    throw;
  }
}

string getArticle(MessageHandler messageHandler) {
  try {
    int newsGroupID;
    int articleID;
    cout << "Enter id of the newsgroup you want to view an article under: ";
    cin >> newsGroupID;
    cout << "Enter id of the article you want to view: ";
    cin >> articleID;
    messageHandler.sendCode(Protocol::COM_GET_ART);
    messageHandler.sendIntParameter(newsGroupID);
    messageHandler.sendIntParameter(articleID);
    messageHandler.sendCode(Protocol::COM_END);
    messageHandler.recvCode();
    Protocol code = messageHandler.recvCode();
    if(code == Protocol::ANS_ACK) {
      string title;
      string author;
      string text;
      title = messageHandler.recvStringParameter();
      author = messageHandler.recvStringParameter();
      text = messageHandler.recvStringParameter();
      messageHandler.recvCode();
      return "Title: " + title + "\n" + "Author: " + author + "\n" + "Text: " + text + "\n";
    } else if (code == Protocol::ANS_NAK) {
      Protocol code = messageHandler.recvCode();
      if(code == Protocol::ERR_NG_DOES_NOT_EXIST) {
        messageHandler.recvCode();
        return "Newsgroup with id: " + to_string(newsGroupID) + " does not exist.";
      } else if (code == Protocol::ERR_ART_DOES_NOT_EXIST) {
        messageHandler.recvCode();
        return "Article with id: " + to_string(articleID) + " doesn't exist.";
      } else {
        messageHandler.recvCode();
        return "Couldn't get article and no reason was given.";
      }
    } else {
      throw ProtocolViolationException();
    }
  } catch (exception&) {
    throw;
  }
}

string executeCommand(int nbr, MessageHandler messageHandler) {
  switch(nbr) {
    case 1: return listNewsGroups(messageHandler);
            break;
    case 2: return createNewsGroup(messageHandler);
            break;
    case 3: return deleteNewsGroup(messageHandler);
            break;
    case 4: return listArticles(messageHandler);
            break;
    case 5: return createArticle(messageHandler);
            break;
    case 6: return deleteArticle(messageHandler);
            break;
    case 7: return getArticle(messageHandler);
            break;
    case 8: return "Exit";
            break;
    case 0: return help();
            break;
    default: return "Command doesn't exist. Please enter one of the following command numbers: \n" + help();
  }
}

/* Creates a client for the given args, if possible.
 * Otherwise exits with error code.
 */
shared_ptr<Connection> init(int argc, char* argv[])
{
        if (argc != 3) {
                cerr << "Usage: myclient host-name port-number" << endl;
                exit(1);
        }

        int port = -1;
        try {
                port = stoi(argv[2]);
        } catch (exception& e) {
                cerr << "Wrong port number. " << e.what() << endl;
                exit(2);
        }

        shared_ptr<Connection> conn(new Connection(argv[1], port));
        if (!conn->isConnected()) {
                cerr << "Connection attempt failed" << endl;
                exit(3);
        }

        return conn;
}

int app(shared_ptr<Connection> conn)
{
  MessageHandler messageHandler(conn);
  cout << help() << endl;
  cout << "Enter command(#): ";
  int nbr;
  while (true) {
    cin >> nbr;
    if (!cin.good()) {
      cin.clear();
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      cout << "Not a valid command number please try again." << endl;
      cout << help() << endl;
    } else {
      try {
          string reply = executeCommand(nbr, messageHandler);
          if(reply == "Exit") {
            cout << "exiting...." << endl;
            return 0;
          }
          cout << reply << endl;

      } catch (ConnectionClosedException&) {
        cout << " no reply from server. Exiting." << endl;
        return 1;
      } catch (ProtocolViolationException&) {
        cout << "An error occured in the communication with the server please try again." << endl;
      } catch (exception&) {
        cout << "Unkown error occured please try again." << endl;
      }
    }
    cout << "Enter command(#), type \"0\" for command list: ";
  }
  cout << "\nexiting.\n" << endl;
  return 0;
}

int main(int argc, char* argv[])
{
        shared_ptr<Connection> conn = init(argc, argv);
        return app(conn);
}
