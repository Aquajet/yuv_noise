#pragma once
#include <QWidget>
#include <QGridLayout>
#include <QTextDocument>

#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QtDebug>
#include <QMutex>
#include <QTextCursor>
#include <QTextDocument>
#include <QEvent>

#include <memory>

class QLabel;
class QListWidget;
class QPlainTextEdit;
class TouchKeyboard;
class MessageEvent;

using namespace std;

namespace MessageOutput
{
    //////////////////////////////
    class Console : public QObject
    {
            Q_OBJECT
            Q_PROPERTY( bool dublicateToStdOut READ dublicateToStdOut WRITE setDublicateToStdOut)

    public:
            Console( bool lineNumbering, QTextDocument * buffer = new QTextDocument() );

            // Добавление сообщения
            Console & operator << (const QString & msg);

    public slots:

            // дбавление сообщения
            void append(const QString & msg, QtMsgType type);
            void append(const QString & msg, QtMsgType type, const QBrush & brush);
            void append(const QString & msg, QtMsgType type, const QTextCharFormat & cf);

            QTextDocument * buffer();
            const QTextDocument * buffer() const;

            // Дублировать вывод в std::cout
            bool dublicateToStdOut() const;
            void setDublicateToStdOut( bool dublicate);

    signals:
            void new_command(QString);

    public slots:
            void on_new_command(QString command);
            // Событие получения сообщения ( для передачи сообщений между потоками через postEvent)
            void customEvent ( QEvent * event );

    private:
            bool lineNumbering_;
            int lineNumber_;
            auto_ptr<QTextDocument> buf_;
            auto_ptr<QTextCursor> cur_;
            bool dublicateToStdOut_;
    };


    ////////////////////////////////////
    class MessageEvent : public QEvent
    {

    public:
        explicit MessageEvent( QString msg, QtMsgType msgType);
        explicit MessageEvent( QString msg, QtMsgType msgType, const QBrush & cf);
        static QEvent::Type eventClassType();

        // сообщение
        QString message() const;
        // тип сообщения
        QtMsgType msgType() const;
        // установлен ли особый формат символов
        bool isCustomCharFormatSet() const;
        // формат символов
        QBrush charFormat() const;


    private:
        static QEvent::Type type_;

        QString message_;
        QtMsgType msgType_;
        QBrush charFormat_;
        bool isCustomCharFormatSet_;
    };

    ////////////////////////////////////
    class ConsoleWidget : public QWidget
    {
            Q_OBJECT

    public:
            ConsoleWidget(Console * console, QWidget * parent, bool withInput = false);
            ~ConsoleWidget();

    signals:
            void new_command(QString);

    public slots:
            void enterClicked();
            void clear();

    private:
            QGridLayout *lay_;
            QPlainTextEdit * text_;
            QLineEdit * input_;
            QPushButton * enter_;
            TouchKeyboard * keyboard_;

            Console * console_;
    };


    void setMessageOutputToGlobalConsole( bool repeatToPrevious = false);
    bool duplicateToStdOut();
    void setDuplicateToStdOut( bool dubl);

    bool stdOutLineNumbering();
    void setStdOutLineNumbering( bool numbering);

    void messageOutput(QtMsgType type, const char *msg);
    void formattedMessageOutput(QtMsgType type, const char *msg, const QBrush & brush, bool customCharFormat = false);
    Console * globalConsole();
}
