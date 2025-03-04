#include <QCoreApplication>
#include <QTcpServer>
#include <QtHttpServer/QHttpServer>
#include <QtHttpServer/QHttpServerResponse>
#include "qtservice.h"

class MyHttpService : public QtService<QCoreApplication>
{
public:
    MyHttpService(int argc, char **argv)
        : QtService<QCoreApplication>(argc, argv, "Qt MyHttp Daemon")
    {
        setServiceDescription("My dummy HTTP service implemented with Qt");
        setServiceFlags(QtServiceBase::CanBeSuspended);
    }

protected:
    void start()
    {
        // QCoreApplication *app = application();
        tcpServer = new QTcpServer();
        server = new QHttpServer();
        tcpServer->listen(QHostAddress::Any, 18079);
        server->route("/", []() { return QHttpServerResponse("hello"); });
        if (!server->bind(tcpServer)) {
            qCritical() << "HTTP server bind failed";
            logMessage(QString("Failed to bind to port 18079"), QtServiceBase::Error);
        }
    }
    // void pause() {}

    // void resume() {}
private:
    QHttpServer *server;
    QTcpServer *tcpServer;
};

int main(int argc, char *argv[])
{
    QHttpServer *server = new QHttpServer();
    QTcpServer *tcpServer = new QTcpServer();
    tcpServer->listen(QHostAddress::Any, 18080);
    server->route("/", []() { return QHttpServerResponse("hello"); });
    if (!server->bind(tcpServer)) {
        qCritical() << "HTTP server bind failed";
        return 1;
    }

    MyHttpService service(argc, argv);
    return service.exec();

    // QCoreApplication app(argc, argv);
    // return app.exec();
}
