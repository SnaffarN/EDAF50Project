#include "messagehandler.h"
#include "protocol.h"
#include "connectionclosedexception.h"
#include "connection.h"

#include <string>
#include <memory>
#include <exception>

using std::exception;
using std::string;

MessageHandler::MessageHandler(std::shared_ptr<Connection> c) : conn(c) {}

void MessageHandler::sendCode(Protocol code) {
  sendByte(static_cast<int>(code));
}

void MessageHandler::sendInt(int value) {
  sendByte((value >> 24) & 0xFF);
  sendByte((value >> 16) & 0xFF);
  sendByte((value >> 8) & 0xFF);
  sendByte(value & 0xFF);
}

void MessageHandler::sendIntParameter(int param) {
  sendCode(Protocol::PAR_NUM);
  sendInt(param);
}

void MessageHandler::sendStringParameter(string param) {
  sendCode(Protocol::PAR_STRING);
  sendInt(static_cast<int>(param.length()));
  for(char c : param) {
    sendByte(c);
  }
}

Protocol MessageHandler::recvCode() {
  Protocol code;
  try {
    code = static_cast<Protocol>(recvByte());
  } catch(exception& e) {
    throw "Invalid protocol";
  }
  return code;

}

int MessageHandler::recvInt() {
    int b1 = recvByte();
    int b2 = recvByte();
    int b3 = recvByte();
    int b4 = recvByte();

    return b1 << 24 | b2 << 16 | b3 << 8 | b4;
}

int MessageHandler::recvIntParameter() {
  Protocol code = recvCode();
  if(code != Protocol::PAR_NUM) {
    throw "Expected code PAR_NUM got code: " + static_cast<int>(code); //add ProtocolViolationException later.
  }
  return recvInt();
}

string MessageHandler::recvStringParameter() {
  Protocol code = recvCode();
  if(code != Protocol::PAR_STRING) {
    throw "Expected code PAR_STRING got code: " + static_cast<int>(code); //add ProtocolViolationException later.
  }
  int n = recvInt();
  if (n < 0) {
    throw "String length must be greater or equal to 0";
  }
  string result;
  for(int i = 0; i < n; i++) {
    char c = conn->read();
    result.push_back(c);
  }
  return result;
}

void MessageHandler::sendByte(int code) {
  try {
    conn->write(code);
  } catch(exception& e)  {
    throw "Connection Exception";//ConnectionClosedException::ConnectionClosedException();
  }
}

int MessageHandler::recvByte() {
  return conn->read();
}
