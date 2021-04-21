#include "messagehandler.h"
#include "protocol.h"
#include "connectionclosedexception.h"
#include "protocolviolationexception.h"
#include "connection.h"

#include <string>
#include <memory>
#include <exception>
#include <iostream>

using std::exception;
using std::string;
using std::endl;
using std::cout;

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
  } catch (ConnectionClosedException&) {
    throw;
  } catch (exception&) {
    throw ProtocolViolationException();
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
    throw ProtocolViolationException();
  }
  return recvInt();
}

string MessageHandler::recvStringParameter() {
  Protocol code = recvCode();
  if(code != Protocol::PAR_STRING) {
    throw ProtocolViolationException();

  }
  int n = recvInt();
  if (n < 0) {
    throw ProtocolViolationException();
  }
  string result;
  for(int i = 0; i < n; i++) {
    try {
      char c = conn->read();
      result.push_back(c);
    } catch(ConnectionClosedException&)  {
      throw;
    }
  }
  return result;
}

void MessageHandler::sendByte(int code) {
  try {
    conn->write(code);
  } catch(ConnectionClosedException&)  {
    throw;
  }
}

int MessageHandler::recvByte() {
  try {
    return conn->read();
  } catch(ConnectionClosedException&)  {
    throw;
  }
}
