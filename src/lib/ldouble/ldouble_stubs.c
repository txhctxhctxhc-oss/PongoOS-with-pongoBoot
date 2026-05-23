#include <math.h>
#include <stdlib.h>
#include <wchar.h>

long double sinl(long double x)       { return sin(x); }
long double cosl(long double x)       { return cos(x); }
long double tanl(long double x)       { return tan(x); }
long double asinl(long double x)      { return asin(x); }
long double acosl(long double x)      { return acos(x); }
long double atanl(long double x)      { return atan(x); }
long double atan2l(long double y, long double x) { return atan2(y, x); }
long double sinhl(long double x)      { return sinh(x); }
long double coshl(long double x)      { return cosh(x); }
long double tanhl(long double x)      { return tanh(x); }
long double asinhl(long double x)     { return asinh(x); }
long double acoshl(long double x)     { return acosh(x); }
long double atanhl(long double x)     { return atanh(x); }
long double expl(long double x)       { return exp(x); }
long double exp2l(long double x)      { return exp2(x); }
long double expm1l(long double x)     { return expm1(x); }
long double logl(long double x)       { return log(x); }
long double log2l(long double x)      { return log2(x); }
long double log10l(long double x)     { return log10(x); }
long double log1pl(long double x)     { return log1p(x); }
long double logbl(long double x)      { return logb(x); }
long double powl(long double x, long double y) { return pow(x, y); }
long double sqrtl(long double x)      { return sqrt(x); }
long double cbrtl(long double x)      { return cbrt(x); }
long double hypotl(long double x, long double y) { return hypot(x, y); }
long double fabsl(long double x)      { return fabs(x); }
long double ceill(long double x)      { return ceil(x); }
long double floorl(long double x)     { return floor(x); }
long double roundl(long double x)     { return round(x); }
long double truncl(long double x)     { return trunc(x); }
long double rintl(long double x)      { return rint(x); }
long double nearbyintl(long double x) { return nearbyint(x); }
long double fmodl(long double x, long double y) { return fmod(x, y); }
long double remainderl(long double x, long double y) { return remainder(x, y); }
long double remquol(long double x, long double y, int *q) { return remquo(x, y, q); }
long double fmal(long double x, long double y, long double z) { return fma(x, y, z); }
long double fmaxl(long double x, long double y) { return fmax(x, y); }
long double fminl(long double x, long double y) { return fmin(x, y); }
long double fdiml(long double x, long double y) { return fdim(x, y); }
long double frexpl(long double x, int *e) { return frexp(x, e); }
long double ldexpl(long double x, int e)  { return ldexp(x, e); }
long double modfl(long double x, long double *i) { double d; double r = modf(x, &d); *i = d; return r; }
long double scalbnl(long double x, int n)  { return scalbn(x, n); }
long double scalblnl(long double x, long n){ return scalbln(x, n); }
long double copysignl(long double x, long double y) { return copysign(x, y); }
long double nanl(const char *s)        { return nan(s); }
long double erfl(long double x)        { return erf(x); }
long double erfcl(long double x)       { return erfc(x); }
long double tgammal(long double x)     { return tgamma(x); }
long double lgammal(long double x)     { return lgamma(x); }
long double nextafterl(long double x, long double y) { return nextafter(x, y); }
long double nexttowardl(long double x, long double y){ return nextafter(x, (double)y); }
double      nexttoward(double x, long double y)      { return nextafter(x, (double)y); }
float       nexttowardf(float x, long double y)      { return nextafterf(x, (float)y); }
long long   llrintl(long double x)     { return llrint(x); }
long long   llroundl(long double x)    { return llround(x); }
long        lrintl(long double x)      { return lrint(x); }
long        lroundl(long double x)     { return lround(x); }
int         ilogbl(long double x)      { return ilogb(x); }
int         finitel(long double x)     { return isfinite((double)x); }
long double strtold(const char *s, char **e)              { return (long double)strtod(s, e); }
long double strtold_l(const char *s, char **e, void *l)   { return (long double)strtod(s, e); }
long double wcstold(const wchar_t *s, wchar_t **e)        { return (long double)wcstod(s, e); }
long double wcstold_l(const wchar_t *s, wchar_t **e, void *l) { return (long double)wcstod(s, e); }
long double _strtold_r(void *r, const char *s, char **e) { return (long double)strtod(s, e); }
