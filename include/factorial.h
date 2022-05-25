#pragma once

constexpr long long factorial(const long long n)
{
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}