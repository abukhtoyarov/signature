#include "arg_parser.h"

#include <math.h>

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <exception>
#include <iostream>
#include <regex>
#include <string>

namespace sig {

using namespace std;
using namespace boost::program_options;

using boost::lexical_cast;

constexpr size_t kb_mult = (1LL << 10);
constexpr size_t mb_mult = (1LL << 20);

static size_t parseBlockSize(const variable_value& var) {
  const auto& parsed = var.as<string>();

  static regex reg("\\s*(\\d+|\\d+[.]|\\d+?[.]\\d+)\\s*((|K|k|M|m)[Bb]?)\\s*");
  smatch m;
  if (!regex_match(parsed, m, reg))
    throw runtime_error("Unable to parse '" + parsed + "' as size.");

  // could be a numberr with floating point
  const auto bs = lexical_cast<double>(m[1].str());

  size_t mult = 1;

  if (m[3] == "")
    // default unit is byte
    mult = 1;

  else if (m[3] == "K" || m[3] == "k") {
    // kilobytes
    if (bs > 10 * 1024)
      throw runtime_error(
          "block-size option must be in range from 512b to 10Mb.");
    mult = kb_mult;
  } else if (m[3] == "M" || m[3] == "m") {
    // megabytes
    if (bs > 10)
      throw runtime_error(
          "block-size option must be in range from 512b to 10Mb.");
    mult = mb_mult;
  } else
    throw runtime_error("Unhandled prefix: " + m[2].str() +
                        " possible unit values: b, Kb, Mb; b - default");

  if (mult == 1 && floor(bs) != bs)
    throw runtime_error("Size in the bytes MUST be integer.");

  auto bytes = bs * mult;
  if (bytes < 512 || bytes > 10 * mb_mult)
    throw runtime_error(
        "block-size option must be in range from 512b to 10Mb.");

  return ceil(bytes);
}

bool parse_command_line(int argc, const char** argv, Config& config) {
  try {
    options_description desc{
        "signature -i <input file> -o <output file> [-b <block "
        "size>(b|Kb|Mb)]\n\n"
        "Usage"};

    desc.add_options()("help,h", "help screen")(
        "input,i", value<string>()->required(), "input file stream")(
        "output,o", value<string>()->required(), "output file stream")(
        "block-size,b", value<string>()->default_value("1Mb"),
        "block size is integer value with unit (possible unit values: b, Kb, "
        "Mb; b - default)");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
      cout << desc << "\n";
      return false;
    }

    notify(vm);

    if (vm.count("block-size"))
      config.blockSize = parseBlockSize(vm["block-size"]);

    if (vm.count("input")) config.inputFile = vm["input"].as<string>();

    if (vm.count("output")) config.outputFile = vm["output"].as<string>();

    return true;
  } catch (const exception& e) {
    cerr << "Error: " << e.what() << "\n";
    return false;
  } catch (...) {
    cerr << "Unknown error"
         << "\n";
    return false;
  }

  return true;
}

}  // namespace sig
