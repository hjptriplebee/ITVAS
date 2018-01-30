/******************************************
   file name: Common_global.h
   create time: 2017年12月28日 星期四 13时53分46秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: QT export file
******************************************/
#ifndef COMMON_GLOBAL_H
#define COMMON_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMMON_LIBRARY)
#  define COMMONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define COMMONSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // COMMON_GLOBAL_H
