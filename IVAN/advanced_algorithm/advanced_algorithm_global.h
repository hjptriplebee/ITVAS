/******************************************
   file name: Advanced_algorithm_global.h
   create time: 2017年12月27日 星期三 22时44分55秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: QT export file
******************************************/
#ifndef ADVANCED_ALGORITHM_GLOBAL_H
#define ADVANCED_ALGORITHM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ADVANCED_ALGORITHM_LIBRARY)
#  define ADVANCED_ALGORITHMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ADVANCED_ALGORITHMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ADVANCED_ALGORITHM_GLOBAL_H
