TEMPLATE  = subdirs

SUBDIRS   = \
    cplusplus


QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11

#for(l, SUBDIRS) {
#    QTC_LIB_DEPENDS =
#    include($$l/$${l}_dependencies.pri)
#    lv = $${l}.depends
#    $$lv = $$QTC_LIB_DEPENDS
#}

