#include <QApplication>
#include <QLabel>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QStringBuilder>
#include <QTextDocument>
#include <QTextCursor>
#include <QPlainTextDocumentLayout>
#include <QTextCharFormat>
#include <QBrush>

#include <iostream>

#include "Console.h"

namespace MessageOutput
{
    static QtMsgHandler previousMsgHandler_ = NULL;
    static bool duplicateToStdOut_ = true;
    static bool stdOutLineNumbering_ = true;
    static int stdOutCounter_ = 0;

    // Console //////////////////////////////////////////////////////////////////////////////////////////////////
    Console::Console(bool lineNumbering, QTextDocument * buffer)
    : lineNumbering_( lineNumbering), lineNumber_(0), buf_( buffer), cur_( new QTextCursor(buffer) ), dublicateToStdOut_(false)
    {
            buf_->setParent( this);
            buf_->setDocumentLayout( new QPlainTextDocumentLayout( buf_.get()) );
            buf_->setMaximumBlockCount( 2000);
    }

    QTextDocument * Console::buffer()
    {
            return buf_.get();
    }

    const QTextDocument * Console::buffer() const
    {
            return buf_.get();
    }

    bool Console::dublicateToStdOut() const
    {
        return dublicateToStdOut_;
    }

    void Console::setDublicateToStdOut( bool dublicate)
    {
        dublicateToStdOut_ = dublicate;
    }

    void Console::append(const QString & msg, QtMsgType type)
    {
        QTextCharFormat cf = cur_->charFormat();

        switch (type) {
        case QtDebugMsg:
                cf.setForeground( QBrush( Qt::black ) );
                break;
        case QtWarningMsg:
                cf.setForeground( QBrush( Qt::magenta ) );
                break;
        case QtCriticalMsg:
                cf.setForeground( QBrush( Qt::red ) );
                break;
        case QtFatalMsg:
                cf.setForeground( QBrush( Qt::red ) );
                break;
        }

        append( msg, type, cf);
    }

    void Console::append(const QString & msg, QtMsgType type, const QBrush & brush)
    {
        QTextCharFormat cf = cur_->charFormat();
        cf.setForeground( brush );
        append( msg, type, cf);
    }

    void Console::append(const QString & msg, QtMsgType type, const QTextCharFormat & cf)
    {
            //std::cout << msg.toAscii().data();
            QString newLine = msg % '\n';

            switch (type) {
            case QtDebugMsg:
                    //cf.setForeground( QBrush( Qt::black ) );
                    break;
            case QtWarningMsg:
                    newLine = "Warning: " % msg % '\n';
                    //cf.setForeground( QBrush( Qt::magenta ) );
                    break;
            case QtCriticalMsg:
                    newLine = "Critical: " % msg % '\n';
                    //cf.setForeground( QBrush( Qt::red ) );
                    break;
            case QtFatalMsg:
                    newLine = "Fatal: " % msg % '\n';
                    //cf.setForeground( QBrush( Qt::red ) );
                    break;
            default:
                    newLine = "Unknown message: " % msg;
                    break;
            }

            if (lineNumbering_)
            {
                    newLine = QString::number(lineNumber_) % "| " % newLine;
                    cur_->movePosition( QTextCursor::End);
            }

            cur_->movePosition( QTextCursor::End);
            cur_->insertText( newLine, cf);

            if (dublicateToStdOut_)
                std::wcout << newLine.toStdWString();

            ++lineNumber_;
    }

    void Console::on_new_command(QString command)
    {
            this->append("user: " + command, QtDebugMsg);
            emit new_command(command);
    }

    void Console::customEvent ( QEvent * event )
    {
        if ( event->type() ==  MessageEvent::eventClassType() )
        {
            MessageEvent * me = (MessageEvent*) event;
            if ( me->isCustomCharFormatSet() )
                append( me->message(), me->msgType(), me->charFormat() );
            else
                append( me->message(), me->msgType() );
        }
    }

    // MessageEvent //////////////////////////////////////////////////////////////////////////////////////
    QEvent::Type MessageEvent::type_ = (QEvent::Type)QEvent::registerEventType();

    MessageEvent::MessageEvent( QString msg, QtMsgType _msgType)
    :   QEvent(type_), message_(msg), msgType_(_msgType), isCustomCharFormatSet_(false)
    {
    }

    MessageEvent::MessageEvent( QString msg, QtMsgType _msgType, const QBrush & cf)
    :   QEvent(type_), message_(msg), msgType_(_msgType), charFormat_(cf), isCustomCharFormatSet_(true)
    {
    }

    QEvent::Type MessageEvent::eventClassType()
    {
        return type_;
    }

    QString MessageEvent::message() const
    {
        return message_;
    }

    QtMsgType MessageEvent::msgType() const
    {
        return msgType_;
    }

    bool MessageEvent::isCustomCharFormatSet() const
    {
        return isCustomCharFormatSet_;
    }

    QBrush MessageEvent::charFormat() const
    {
        return charFormat_;
    }


    // ConsoleWidget ///////////////////////////////////////////////////////////////////////////////////////////////////
    ConsoleWidget::ConsoleWidget( Console * console, QWidget * parent, bool withInput)
    :	QWidget(parent), console_(console)
    {
            this->setContentsMargins(0,0,0,0);
            lay_ = new QGridLayout(this);
            lay_->setContentsMargins(0,0,0,0);
            this->setLayout(lay_);

            text_ = new QPlainTextEdit( this);
            text_->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn);
            text_->setStyleSheet( "background-color: white; font-size: 12px; font-weight: 100;");
            lay_->addWidget(text_, 0, 0, 1, 2);

            text_->setDocument( console_->buffer() );

            if ( withInput)
            {
    //		input_ = new QLineEdit(this);
    //		lay_->addWidget( input_, 1, 0);
    //		enter_ = new QPushButton("Ввод", this);
    //		enter_->setMaximumSize( 100, 50);
    //		enter_->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
    //		connect( enter_, SIGNAL( clicked() ), SLOT( enterClicked()) );
    //		lay_->addWidget( enter_, 1, 1);

    //		keyboard_ = new TouchKeyboard(this, TouchKeyboard::English);
    //		keyboard_->addReceiver( input_);
    //		lay_->addWidget( keyboard_, 2, 0, 1, 2, Qt::AlignCenter);
            }
            else
            {
                    input_ = NULL;
                    enter_ = NULL;
                    keyboard_ = NULL;
            }


            //connect( console_, SIGNAL(new_line(QString)), SLOT(on_new_line(QString)) );
    }

    ConsoleWidget::~ConsoleWidget()
    {
    }


    void ConsoleWidget::enterClicked()
    {
            static QString console_hide("console hide");
            static QString console_windowmode("console windowmode");
            QString command = input_->text();
            if ( command.compare(console_hide) == 0)
            {
                    this->hide();
                    qDebug() << "Console hidden.";
            }
            else
            if ( command.compare(console_windowmode) == 0)
                    this->setWindowState( this->windowState() ^ Qt::WindowFullScreen );
            else
                    emit new_command( input_->text() );
            //console_->on_new_command(this->input->text());
    }

    void ConsoleWidget::clear()
    {
            text_->clear();
    }


    // Functions ///////////////////////////////////////
    Console * globalConsole()
    {
            static Console * gConsole = new Console(true);
            return gConsole;
    }

    void setMessageOutputToGlobalConsole( bool repeatToPrevious)
    {
        QtMsgHandler p = qInstallMsgHandler( messageOutput);
        if ( ( p != messageOutput) && repeatToPrevious) previousMsgHandler_ = p;
    }


    bool duplicateToStdOut()
    {
        return duplicateToStdOut_;
    }

    void setDuplicateToStdOut( bool duplicate)
    {
        duplicateToStdOut_ = duplicate;
    }

    bool stdOutLineNumbering()
    {
        return stdOutLineNumbering_;
    }

    void setStdOutLineNumbering( bool numbering)
    {
        stdOutLineNumbering_ = numbering;
    }

    void messageOutput(QtMsgType type, const char *msg)
    {
        formattedMessageOutput( type, msg, QBrush(), false );
    }

    void formattedMessageOutput(QtMsgType type, const char *msg, const QBrush & brush, bool customCharFormat)
    {
        static QMutex mutex;

        QMutexLocker locker( &mutex);

        if ( duplicateToStdOut_)
        {
            if ( stdOutLineNumbering_) std::cout << ++stdOutCounter_ << "| ";
            std::cout << msg << endl;
        }

        if ( !qApp) return;

        MessageEvent * me;
        if ( customCharFormat)
            me = new MessageEvent( msg, type, brush);
        else
            me = new MessageEvent( msg, type);

        qApp->postEvent( globalConsole(), me );
//        if (previousMsgHandler)
//            previousMsgHandler(type, msg);

    }
}
