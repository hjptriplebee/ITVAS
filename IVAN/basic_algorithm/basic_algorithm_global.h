/******************************************
   file name: Basic_algorithm_global.h
   create time: 2017年12月27日 星期三 21时47分13秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: QT export file
******************************************/
#ifndef BASIC_ALGORITHM_GLOBAL_H
#define BASIC_ALGORITHM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BASIC_ALGORITHM_LIBRARY)
#  define BASIC_ALGORITHMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define BASIC_ALGORITHMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // BASIC_ALGORITHM_GLOBAL_H
