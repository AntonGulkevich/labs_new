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
    void add(T object);
    void remove(int id);
    int size() const;
    T& getObject(int id);
    bool isObject(T& orig, bool (*callback)(T&, T&));
    bool getObjects(QList<T> &list);
    bool getObjects(QList<T> &list, bool (*callback)(const T&));
    void check(); // !!
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
    qDebug() << "destruct storage";
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

//        typename QList<T>::iterator it;
        for (auto it = container_->begin(); it != container_->end(); ++it) {
//        for (auto it : *container_) {
            out << *it;
        }

        storageFile.close();
}

// public
template <class T>
void Storage<T>::add(T object)
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

//public
template <class T>
bool Storage<T>::isObject(T& orig, bool (*callback)(T&, T&))
{
    for (auto &it : *container_) {
        if (callback(orig, it))
            return true;
    }
    return false;
}

// public
template <class T>
bool Storage<T>::getObjects(QList<T> &list)
{
    list = *container_;
    return (list.size() > 0);
}

// public
template <class T>
bool Storage<T>::getObjects(QList<T> &list, bool (*callback)(const T&))
{
//    std::for_each(container_->begin(), container_->end(), [&](T obj) {
//       if (callback(obj)) list.append(obj);
//    });

    for (auto &it : *container_) {
        if (callback(it))
            list.append(it);
    }

//    for (auto it = container_->begin(); it != container_->end(); ++it) {
//        if (callback(*it)) {
//            list.append(*it);
//        }
//    }

    return (list.size() > 0);
}

// public
template <class T>
void Storage<T>::check() {
    for (auto it : *container_) {
//        qDebug() << it.
    }
}


#endif // STORAGE_H
