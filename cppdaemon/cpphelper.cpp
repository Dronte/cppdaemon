#include "cpphelper.h"
#include "Overview.h"
#include<QByteArray>
#include<QFile>
#include<QDebug>

using namespace CPlusPlus;

CppHelper::CppHelper(const Snapshot & _snapshot, const Document::Ptr & _documentPtr):
    snapshot(_snapshot),documentPtr(_documentPtr),source()
{}

CppHelper::CppHelper():
    snapshot(),documentPtr(),source()
{}

void CppHelper::add(const QString &filename){
    /*if(!Document::Ptr previousDocumentPtr = snapshot.document(filename)){

    }*/
    QFile sourceFile(filename);
    qDebug()<<sourceFile.open(QIODevice::ReadOnly | QIODevice::Text);
    source = sourceFile.readAll();
    if (documentPtr && filename==documentPtr->fileName()){
    }else{
        documentPtr = snapshot.preprocessedDocument(source,filename);
        documentPtr->tokenize();
        documentPtr->parse();
        documentPtr->check();
    }
}

QList <Usage> CppHelper::findUsages(int line,int col){
    qDebug()<<"findUsages"<<line<<col;
    Symbol * symbol = documentPtr->lastVisibleSymbolAt(line,col);
    qDebug()<<"symbol name="<<symbol->name()->identifier()->chars();
    Overview overview;
    qDebug()<<"symbol pretty name="<<overview(symbol->name());
    qDebug()<<documentPtr->functionAt(line,col);
    qDebug()<<"document::skipFunctionBody"<<documentPtr->skipFunctionBody();
    int total = documentPtr->globalSymbolCount() ;
    qDebug()<<"total="<<total;
    for(int i=0 ; i < total; ++i){
        qDebug()<<documentPtr->globalSymbolAt(i)->identifier()->chars();
    }
    if(symbol){
        qDebug()<<"symbol";
        FindUsages usages(source,documentPtr,snapshot);
        usages(symbol);
        return usages.usages();
    }
    return QList<Usage>();
}
