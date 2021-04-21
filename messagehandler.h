#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "connection.h"
#include "protocol.h"

#include <memory>
#include <string>

using std::string;

class MessageHandler {
  public:
    MessageHandler(std::shared_ptr<Connection> c);

    void sendCode(Protocol code);
    void sendInt(int value);
    void sendIntParameter(int param);
    void sendStringParameter(string param);

    Protocol recvCode();
    int recvInt();
    int recvIntParameter();
    string recvStringParameter();
  private:
    void sendByte(int code);

    int recvByte();

    std::shared_ptr<Connection> conn;
};
#endif
