#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <QMutex>
#include "objectvars.h"
#include <QVector>

class DataStorage
{
    mutable QMutex mutex;
    QVector<ObjectVars*> objects;

public:
    static DataStorage& Instance()
    {
        static DataStorage singleton;
        return singleton;
    }
    void updateObject(const ObjectVars &ob);
    void clear();
    int getObCount(void) const {return objects.count();}
    ObjectVars getObject(int num) const;
private:
    DataStorage();                                      // Private constructor
    ~DataStorage();
    DataStorage(const DataStorage&);                    // Prevent copy-construction
    DataStorage& operator=(const DataStorage&);         // Prevent assignment
};

#endif // DATASTORAGE_H
