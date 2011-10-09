#include <QFile>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QXmlDefaultHandler>
#include <QUrl>

#include "opmlimport.h"
#include "podcastsdb.h"


class OPMLHandler : public QXmlDefaultHandler
{
private:
    int m_newElements;
public:
    OPMLHandler(void) : QXmlDefaultHandler()
    {
        resetCounter();
    }

    void resetCounter()
    {
        m_newElements = 0;
    }

    int newElements ()
    {
        return m_newElements;
    }

    virtual bool startElement ( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts)
    {
		Q_UNUSED (qName);
		Q_UNUSED (namespaceURI);

        int idx = atts.index("xmlUrl");
        if (localName == "outline" && idx>0)
        {
            QUrl url (atts.value(idx));
            qDebug() << "����� ��������, url = " << url.toString();
            // ��������� ���� �� � ���� � ����� ����� �������
            if (pdb.exists(url)) // ��� ����� ����
                return true; // ������ ������

            pdb.append (url);
            m_newElements++;
        }
        return true;
    }
};

int OPMLImport ( QString src )
{
    QFile file (src);
    QXmlSimpleReader xmlReader;
    QXmlInputSource *source = new QXmlInputSource(&file);

    OPMLHandler *handler = new OPMLHandler;
    xmlReader.setContentHandler(handler);
    xmlReader.setErrorHandler(handler);

    bool ok = xmlReader.parse(source);

    if (!ok)
        qDebug() << "Parsing failed." << endl;
    return handler->newElements();
}
