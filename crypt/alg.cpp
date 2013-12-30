#include "alg.h"


using boost::multiprecision::cpp_int;
//using boost::lexical_cast;

QStringList getPrimes(int limit) {
    QStringList primes;

    int t, i, flag;

    for(t=2 ; t <= limit ; ++t) {
        flag = 1;
        for (i=2; i * i <= t; ++i)
            if (t % i == 0) {
                flag=0; break;
            }
        if (flag)
            primes << QString::number(t);
    }
    return primes;
}

QStringList getPrimesByK(int limit, int k, int p) {
    QStringList primes;

    int t, i, flag;

    for(t=2 ; t <= limit ; ++t) {
        flag = 1;
        for (i=2; i * i <= t; ++i)
            if (t % i == 0) {
                flag=0; break;
            }
        if (flag) {
            if ((pow(2, k) / p < t) && (t <= pow(2, k + 1) / p ))
                primes << QString::number(t);
        }
    }
    primes.insert(0, "Not selected");
    return primes;
}


QList< QPair<int, int> > getDivisors(int n) {
    QList <QPair<int, int> > pairs;
    for (int i = 2; i < sqrt(n); ++i) {
        if (n % i == 0)
            pairs.append(qMakePair(i, n/i));
    }
    return pairs;
}

int coder(int value, int e, int n) {
    cpp_int code = boost::multiprecision::powm(
                boost::lexical_cast<cpp_int>(value),
                boost::lexical_cast<cpp_int>(e),
                boost::lexical_cast<cpp_int>(n));
    return boost::lexical_cast<int>(code);
}
