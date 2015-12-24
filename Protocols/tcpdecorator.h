#ifndef TCPDECORATOR_H
#define TCPDECORATOR_H

#include "commanddecorator.h"


class TcpDecorator : public CommandDecorator
{
public:
    explicit TcpDecorator(CommandInterface *cmd);
    bool waitAnAnswer(Request &req, QIODevice &io);
    bool checkAnAnswer(Request &req);
    bool getAnAnswer(Request &req);
    ~TcpDecorator();

    // CommandInterface interface
public:
    bool execute(Request &req, QIODevice &io);
};

#endif // TCPDECORATOR_H
