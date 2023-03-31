#include <QtGui/QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine qml;

  QUrl url = u"qrc:/app/MainWindow.qml"_qs;
  qml.load(url);

  return app.exec();
}