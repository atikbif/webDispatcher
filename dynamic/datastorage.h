#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <QMutex>
#include "objectvars.h"

class DataStorage
{
    QMutex mutex;
public:
    static DataStorage& Instance()
    {
        static DataStorage singleton;
        return singleton;
    }
    void updateObject(const ObjectVars &ob);
private:
    DataStorage();                                      // Private constructor
    ~DataStorage();
    DataStorage(const DataStorage&);                    // Prevent copy-construction
    DataStorage& operator=(const DataStorage&);         // Prevent assignment
};

#endif // DATASTORAGE_H
