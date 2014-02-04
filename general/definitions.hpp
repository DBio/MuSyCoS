#include <vector>
#include <chrono>
#include <random>

#include <boost/range/algorithm.hpp>
#include <boost/range/any_range.hpp>
#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include <boost/filesystem.hpp>

#include <gecode/int.hh>
#include <gecode/driver.hh>
#include <gecode/gist.hh>

namespace bpo = boost::program_options;
namespace rng = boost::range;
namespace bfs = boost::filesystem;
using namespace std;

const string MODEL_EXTENSION(".pan"); ///< Post's algebra network