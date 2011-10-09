#include "downloadqueue.h"
#include "fileDownloader.h"

DownloadQueue::DownloadQueue(QObject *parent) :
    QObject(parent)
{
    setMaximumDownloads(5);
}

void DownloadQueue::setMaximumDownloads (int size)
{
    while (downloaderArmy.size()<size)
    {
        // �������� �������
        fileDownloader * D = new fileDownloader();
        connect (D, SIGNAL(done()), this, SLOT(checkQueue()));
        downloaderArmy.append(D);
    }

    foreach (fileDownloader * d, downloaderArmy)
    {
        if (downloaderArmy.size()==size)
            break;
        if (!d->isActive())
        {
            downloaderArmy.remove(downloaderArmy.indexOf(d));
            d->deleteLater();
        }
    }
}

void DownloadQueue::checkQueue()
{
    while (!queue.isEmpty()) // ���� ���� � ������� ���������
    {
        QPair<QString, QString> SS = queue.dequeue();
        if (!addDownload(SS.first, SS.second))  // ���� �� �����������
            break;                              //  - ����������
    }
}

bool DownloadQueue::addDownload(QString url, QString fileName)
{
    // ���� ����� ��������, ������ ��. ���� ���������� - �������� � �������
    foreach (fileDownloader * d, downloaderArmy)
        if (!d->isActive())
        {
            d->updateAddress (url);
            d->updateFileName(fileName);
            d->fetch();
            return true;
        }
    queue.append (qMakePair(url, fileName));
    return false;
}
