#ifndef PROTOCOL_VIOLATION_EXCEPTION_H
#define PROTOCOL_VIOLATION_EXCEPTION_H

#include <exception>
using namespace std;

struct ProtocolViolationException : public exception {
  const char * what () const throw ()  {
    return "A protocol was violated.";
  }
};

#endif
