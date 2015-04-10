// $Id: examples.cc,v 1.5 2006/06/05 17:09:49 yotam Exp $
// (C) Yotam Medini  --  yotam@blueneptune.com
// May be freely copied as long as keeping it complete.
//
// Examples used for "STL Quick Reference"
//
// The following is not meant to recommend programming techniques
// or coding style. In particular, it should not be used as
// examples of How-To-Use-STL.
// It comes only to demonstrate STL functionalities.
//

// C-lib
#include <cmath>

// C++ lib
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <complex>

// STL's headers
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>

namespace std {}; using namespace std;

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void eqrange_example() {
  // first 5 Fibonacci
  static int fb5[] = { 1, 1, 2, 3, 5 };
  for (int n = 0; n <= 6; ++n) {
    pair<int*, int*> p =
      equal_range(fb5, fb5 + 5, n);
    cout << n << ":[" << p.first - fb5 << ','
      << p.second - fb5 << ") ";
    if (n == 3 || n == 6) cout << endl;
  }
} // eqrange_example

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// vector-examples-start
// safe get
int  vi(const vector<unsigned>& v, int i) {
  return(i < (int)v.size() ? (int)v[i] : -1);
}

// safe set
void vin(vector<int>& v, unsigned i, int n) {
  int  nAdd = i - v.size() + 1;
  if (nAdd > 0) v.insert(v.end(), nAdd, n);
  else v[i] = n;
}
// vector-examples-end

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void map_example() {
  typedef map<string, int> MSI;
  MSI  nam2num;
  nam2num.insert(MSI::value_type("one", 1));
  nam2num.insert(MSI::value_type("two", 2));
  nam2num.insert(MSI::value_type("three", 3));
  int n3 = nam2num["one"] + nam2num["two"];
  cout << n3 << " called ";
  for (MSI::const_iterator i = nam2num.begin();
    i != nam2num.end();   ++i)
    if ((*i).second == n3) {
      cout << (*i).first << endl;
    }
} // map_example


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


// Unit Roots
void unitRoots(int n) {
  cout << "unit " << n << "-roots:" << endl;
  vector<complex<float> > roots;
  float  arg = 2.*M_PI / (float)n;
  complex<float> r, r1 = polar((float)1., arg);
  for (r = r1; --n; r *= r1)
    roots.push_back(r);
  copy(roots.begin(), roots.end(),
    ostream_iterator<complex<float> >(cout,
    "\n"));
} // unitRoots
// Unit Roots

void streamIter_example() {
  {ofstream o("primes.txt"); o << "2 3 5"; }
  ifstream pream("primes.txt");
  vector<int> p;
  istream_iterator<int>  priter(pream);
  istream_iterator<int>  eosi;
  copy(priter, eosi, back_inserter(p));
  for_each(p.begin(), p.end(), unitRoots);
} // streamIter_example


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// { splicehelp
void lShow(ostream& os, const list<int>& l) {
  ostream_iterator<int> osi(os, " ");
  copy(l.begin(), l.end(), osi); os << endl;
}

void lmShow(ostream& os, const char* msg,
  const list<int>& l,
  const list<int>& m) {
  os << msg << (m.size() ? ":\n" : ": ");
  lShow(os, l);
  if (m.size()) lShow(os, m);
} // lmShow

list<int>::iterator p(list<int>& l, int val) {
  return find(l.begin(), l.end(), val);
}
// } splicehelp

void splice_example() {
  static int prim[] = { 2, 3, 5, 7 };
  static int perf[] = { 6, 28, 496 };
  const list<int> lPrimes(prim + 0, prim + 4);
  const list<int> lPerfects(perf + 0, perf + 3);
  list<int> l(lPrimes), m(lPerfects);
  lmShow(cout, "primes & perfects", l, m);
  l.splice(l.begin(), m);
  lmShow(cout, "splice(l.beg, m)", l, m);
  l = lPrimes; m = lPerfects;
  l.splice(l.begin(), m, p(m, 28));
  lmShow(cout, "splice(l.beg, m, ^28)", l, m);
  m.erase(m.begin(), m.end()); // <=>m.clear()
  l = lPrimes;
  l.splice(p(l, 3), l, p(l, 5));
  lmShow(cout, "5 before 3", l, m);
  l = lPrimes;
  l.splice(l.begin(), l, p(l, 7), l.end());
  lmShow(cout, "tail to head", l, m);
  l = lPrimes;
  l.splice(l.end(), l, l.begin(), p(l, 3));
  lmShow(cout, "head to tail", l, m);
} // splice_example


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// { numeric_help
template <class T>
class AbsPwr : public unary_function < T, T > {
public:
  AbsPwr(T p) : _p(p) {}
  T operator()(const T& x) const {
    return pow(fabs(x), _p);
  }
private: T _p;
}; // AbsPwr

template<typename InpIter> float
normNP(InpIter xb, InpIter xe, float p) {
  vector<float>  vf;
  transform(xb, xe, back_inserter(vf),
    AbsPwr<float>(p > 0. ? p : 1.));
  return((p > 0.)
    ? pow(accumulate(vf.begin(), vf.end(), 0.),
    1. / p)
    : *(max_element(vf.begin(), vf.end())));
} // normNP

float distNP(const float* x, const float* y,
  unsigned n, float p) {
  vector<float>  diff;
  transform(x, x + n, y, back_inserter(diff),
    minus<float>());
  return normNP(diff.begin(), diff.end(), p);
} // distNP
// } numeric_help


void numeric_example() {
  float  x3y4[] = { 3., 4., 0. };
  float  z12[] = { 0., 0., 12. };
  float  p[] = { 1., 2., M_PI, 0. };
  for (int i = 0; i < 4; ++i) {
    float d = distNP(x3y4, z12, 3, p[i]);
    cout << "d_{" << p[i] << "}=" << d << endl;
  }
} // numeric_example

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// { sorthelp
class ModN {
public:
  ModN(unsigned m) : _m(m) {}
  bool operator ()(const unsigned& u0,
    const unsigned& u1) {
    return ((u0 % _m) < (u1 % _m));
  }
private: unsigned _m;
}; // ModN
// { sorthelp

void sort_example() {
  ostream_iterator<unsigned> oi(cout, " ");
  unsigned  q[6];
  for (int n = 6, i = n - 1; i >= 0; n = i--)
    q[i] = n*n*n*n;
  cout << "four-powers:   ";
  copy(q + 0, q + 6, oi);
  for (unsigned b = 10; b <= 1000; b *= 10) {
    vector<unsigned>  sq(q + 0, q + 6);
    sort(sq.begin(), sq.end(), ModN(b));
    cout << endl << "sort mod " << setw(4) << b << ": ";
    copy(sq.begin(), sq.end(), oi);
  } cout << endl;
} // sort_example

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// Interator Fermat
// self-refering int 
class Interator : public
  iterator < input_iterator_tag, int, size_t > {
  int  _n;
public:
  Interator(int n = 0) : _n(n) {}
  int operator*() const { return _n; }
  Interator& operator++() {
    ++_n;  return *this;
  }
  Interator  operator++(int) {
    Interator t(*this);
    ++_n;   return t;
  }
}; // Interator
bool operator==(const Interator& i0,
  const Interator& i1) {
  return (*i0 == *i1);
}
bool operator!=(const Interator& i0,
  const Interator& i1) {
  return !(i0 == i1);
}

struct Fermat : public
  binary_function < int, int, bool > {
  Fermat(int p = 2) : n(p) {}
  int n;
  int nPower(int t) const
  { // t^n
    int i = n, tn = 1;
    while (i--) tn *= t;
    return tn;
  } // nPower
  int nRoot(int t) const {
    return (int)pow(t + .1, 1. / n);
  }
  int xNyN(int x, int y) const {
    return(nPower(x) + nPower(y));
  }
  bool operator()(int x, int y) const {
    int zn = xNyN(x, y), z = nRoot(zn);
    return(zn == nPower(z));
  }
}; // Fermat
// Interator Fermat


void
fermat_example(int Mx = 8, int My = 30, int Mp = 4) {
  for (int n = 2; n <= Mp; ++n) {
    Fermat fermat(n);
    for (int x = 1; x < Mx; ++x) {
      binder1st<Fermat>
        fx = bind1st(fermat, x);
      Interator iy(x), iyEnd(My);
      while ((iy = find_if(++iy, iyEnd, fx))
        != iyEnd) {
        int  y = *iy,
          z = fermat.nRoot(fermat.xNyN(x, y));
        cout << x << '^' << n << " + "
          << y << '^' << n << " = "
          << z << '^' << n << endl;
        if (n > 2)
          cout << "Fermat is wrong!" << endl;
      }
    }
  }
} // fermat_example


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// Iterator Traits
template <class Itr>
typename iterator_traits<Itr>::value_type
mid(Itr b, Itr e, input_iterator_tag) {
  cout << "mid(general):\n";
  Itr bm(b);  bool  next = false;
  for (; b != e; ++b, next = !next) {
    if (next) { ++bm; }
  }
  return *bm;
} // mid<input>

template <class Itr>
typename iterator_traits<Itr>::value_type
mid(Itr b, Itr e,
random_access_iterator_tag) {
  cout << "mid(random):\n";
  Itr bm = b + (e - b) / 2;
  return *bm;
} // mid<random>

template <class Itr>
typename iterator_traits<Itr>::value_type
mid(Itr b, Itr e) {
  typename
    iterator_traits<Itr>::iterator_category t;
  mid(b, e, t);
} // mid

template <class Ctr>
void fillmid(Ctr& ctr) {
  static int perfects[5] =
  { 6, 14, 496, 8128, 33550336 },
  *pb = &perfects[0];
  ctr.insert(ctr.end(), pb, pb + 5);
  int m = mid(ctr.begin(), ctr.end());
  cout << "mid=" << m << "\n";
} // fillmid
// Iterator Traits

void iterator_traits_example() {
  list<int> l;  vector<int> v;
  fillmid(l);   fillmid(v);
} // iterator_traits_example

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


int
main(int, char**) {
#  define Run(func, msg) \
      cout << ">>" << msg << endl; \
      func(); \
      cout << "<<" << msg << endl;
  Run(eqrange_example, "eqrange");
  Run(map_example, "map");
  Run(splice_example, "spliceexample");
  Run(numeric_example, "numericexample");
  Run(sort_example, "sortexample");
  Run(fermat_example, "fermat");
  cout.precision(3);
  cout.flags(cout.flags() | ios::fixed);
  Run(streamIter_example, "uroots");
  Run(iterator_traits_example, "itertraits");
  return(0);
} // main
