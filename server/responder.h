#ifndef RESPONDER_H
#define RESPONDER_H

#include <QObject>
#include "qhttpserver-master/src/qhttpserverfwd.h"
#include <QString>

class Responder : public QObject
{
    Q_OBJECT
    QHttpRequest *m_req;

public:
    explicit Responder(QHttpRequest *req, QHttpResponse *resp, QObject *parent = 0);
    ~Responder();
};

#endif // RESPONDER_H
