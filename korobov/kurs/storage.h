#ifndef STORAGE_H
#define STORAGE_H
#include <QMap>
#include <QString>
#include <QFile>
#include <QDebug>


template <class T>
class Storage
{
//    typedef bool (*Callback)(T);
private:
    QList<T> *container_;
    QString filename_;
    QString path_;

public:
    Storage(const QString filename);
    ~Storage();

    void importStorage();
    void exportStorage();
    void add(T& object);
    void remove(int id);
    int size() const;
    T& getObject(int id);
    QList<T&> getObjects(bool (*callback)(T));
    void check(); // !!
    typename QList<T> ::Iterator iterator;

    typename QList<T> ::Iterator begin();
    typename QList<T> ::Iterator end();
};

// public
template <class T>
Storage<T>::Storage(const QString filename)
    : container_(new QList<T>()), filename_(filename)
{
    qDebug() << "storage created!";
    path_ = "/home/dn/mailclient";
}
template <class T>
Storage<T>::~Storage()
{
    delete container_;
    qDebug() << "destruct";
}

// public
template <class T>
void Storage<T>::importStorage()
{
    QFile storageFile(path_ + "/" + filename_);
    if (storageFile.exists()) {
        storageFile.open(QIODevice::ReadOnly);
        QDataStream in(&storageFile);
        T tmp;
        while (!in.atEnd()) {
            in >> tmp;
            add(tmp);
        }
        storageFile.close();
    }
}

// public
template <class T>
void Storage<T>::exportStorage()
{
    QFile storageFile(path_ + "/" + filename_);
    qDebug() << storageFile.fileName();
        storageFile.open(QIODevice::WriteOnly);
        QDataStream out(&storageFile);

        typename QList<T>::iterator it;
        for (it = container_->begin(); it != container_->end(); ++it) {
            out << *it;
        }

        storageFile.close();
}

// public
template <class T>
void Storage<T>::add(T& object)
{
    container_->append(object);
}

// public
template <class T>
void Storage<T>::remove(int id)
{
    container_->removeAt(id);
}

// public
template <class T>
int Storage<T>::size() const
{
    return container_->size();
}

// public
template <class T>
T&  Storage<T>::getObject(int id)
{
    return (*container_)[id];
}

// public
template <class T>
QList<T&> Storage<T>::getObjects(bool (*callback)(T))
{
    QList<T&> ret;
    typename QMap<int, T>::iterator it;
    for (it = container_->begin(); it != container_->end(); ++it) {
        if (callback(*it)) {
            ret.append(*it);
        }
    }

    return ret;
}

// public
template <class T>
void Storage<T>::check() {
    typename QMap<int, T>::iterator it;
    for (it = container_->begin(); it != container_->end(); ++it) {
//        qDebug() << it->to();
    }
}
// public
template <class T>
typename QList<T>::Iterator Storage<T>::begin()
{
    return container_->begin();
}

// public
template <class T>
typename QList<T>::Iterator Storage<T>::end()
{
    return container_->end();
}

#endif // STORAGE_H
