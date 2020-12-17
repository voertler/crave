/****************************************************************************************
//	MIT License
//***************************************************************************************
//	Copyright (c) 2012-2020 University of Bremen, Germany. 
//  	Copyright (c) 2015-2020 DFKI GmbH Bremen, Germany.
//  	Copyright (c) 2020 Johannes Kepler University Linz, Austria.
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in all
//	copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//	SOFTWARE.
****************************************************************************************/

#include <crave/SystemC.hpp>
#include <crave/ConstrainedRandom.hpp>
#include <systemc.h>
#include <boost/timer.hpp>

using crave::rand_obj;
using crave::randv;
using sc_dt::sc_bv;
using sc_dt::sc_uint;

/**
 * ALU:
 * complete enumerated there are:
 * ADD 0x0: 136
 * SUB 0x1: 136
 * MUL 0x2:  76
 * DIV 0x3: 240
 * valid assignments.
 */
struct ALU4 : public rand_obj {
  randv<sc_bv<2> > op;
  randv<sc_uint<4> > a, b;

  ALU4(rand_obj* parent = 0) : rand_obj(parent), op(this), a(this), b(this) {
    constraint((op() != (unsigned char)0x0) || ((unsigned char)15 >= a() + b()));
    constraint((op() != (unsigned char)0x1) || (((unsigned char)15 >= a() - b()) && (b() <= a())));
    constraint((op() != (unsigned char)0x2) || ((unsigned char)15 >= a() * b()));
    constraint((op() != (unsigned char)0x3) || (b() != (unsigned char)0));
  }

  friend std::ostream& operator<<(std::ostream& o, ALU4 const& alu) {
    o << alu.op << ' ' << alu.a << ' ' << alu.b;
    return o;
  }
};

int sc_main(int argc, char** argv) {
  crave::init("./crave.cfg");
  boost::timer timer;
  ALU4 c;
  CHECK(c.next());
  std::cout << "first: " << timer.elapsed() << "\n";
  for (int i = 0; i < 1000; ++i) {
    CHECK(c.next());
    // std::cout << i << ": " << c << std::endl;
  }
  std::cout << "complete: " << timer.elapsed() << "\n";
  return 0;
}