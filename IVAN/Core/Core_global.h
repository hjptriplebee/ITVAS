/******************************************
   file name: Core_global.h
   create time: 2017年12月26日 星期二 20时42分24秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: QT export file
******************************************/
#ifndef CORE_GLOBAL_H
#define CORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CORE_LIBRARY)
#  define CORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define CORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CORE_GLOBAL_H
