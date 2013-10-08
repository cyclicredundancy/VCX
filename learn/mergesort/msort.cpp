#include <iostream>
//#include <boost/timer/timer.hpp>
//#include <boost/timer.hpp>
#include <boost/progress.hpp>
#include <boost/program_options.hpp>
#include <cmath>

namespace po = boost::program_options;

int a_try();

int main(int argc, char** argv)
{
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()("help", "produce help message");

    std::cout << "Hello World!" << std::endl;

    a_try();

    return 0;
}

int a_try(void)
{
  //boost::timer::auto_cpu_timer t;

  boost::progress_timer t;

  for (long i = 0; i < 100000000; ++i)
    std::sqrt(123.456L); // burn some time

  return 0;
}
