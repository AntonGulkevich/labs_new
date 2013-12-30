#ifndef ALG_H
#define ALG_H

#include <QStringList>
#include <cmath>
#include <QDebug>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/lexical_cast.hpp>

QStringList getPrimes(int limit);
QList< QPair<int, int> > getDivisors(int n);
QStringList getPrimesByK(int limit, int k, int p);
int coder(int value, int e, int n);

#endif // ALG_H
