#ifndef CLIHANDLER_H
#define CLIHANDLER_H

#include <fmt/core.h>
#include <portaudio.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <stdexcept>
#include <util/config.hpp>
#include <util/singleton.hpp>
#include <util/listdevices.hpp>

namespace Util {

void parse_cli(int const argc, char** argv) noexcept {
    namespace po = boost::program_options;

    try {
        po::options_description cli("CLI Options");

        cli.add_options()("help,h", "Show Help")("list-devices", "List connected devices");

        po::options_description algorithm("Algorithm Options");
        algorithm.add_options()("file,f", po::value<std::string>()->default_value("filter.txt"),
                                "path to file containing filter-coefficients");

        auto defaultDevice = Pa_GetDeviceCount() - 1;

        po::options_description system("System Options");
        system.add_options()("samplerate,s", po::value<double>()->default_value(48000.0),
                             "Samplerate to run the system at")(
                "device,d", po::value<int>()->default_value(defaultDevice),
                "Portaudio device to use. use --list-devices to get list of devices");

        po::options_description all;
        all.add(cli).add(algorithm).add(system);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(all).run(), vm);

        po::notify(vm);

        if (vm.count("help")) {
            fmt::print("Usage: {} [options]\n{}", argv[0]);
            std::cout << all;
            exit(1);
        }
        if (vm.count("list-devices")) {
            print_devices();
            exit(1);
        }

        auto& config = Singleton<Config>();

        config.FilterPath = vm["file"].as<std::string>();
        config.SampleRate = vm["samplerate"].as<double>();
        config.DeviceIndex = vm["device"].as<int>();

    } catch (std::exception const& e) {
        fmt::print("Caught exception while parsing CLI parameters: {}", e.what());
        exit(1);
    }
}

} // namespace Util

#endif /* CLIHANDLER_H */
