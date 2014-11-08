#ifndef CPPHELPER_H
#define CPPHELPER_H
#include<CppDocument.h>
#include<FindUsages.h>

using namespace CPlusPlus;
class CppHelper
{
private:
    Snapshot snapshot;
    Document::Ptr documentPtr;
    QByteArray source;
public:
    CppHelper();
    CppHelper(const Snapshot & _snapshot, const Document::Ptr & _documentPtr);
    void add(const QString & filename);
    QList<Usage> findUsages(int line,int row);
};

#endif // CPPHELPER_H
