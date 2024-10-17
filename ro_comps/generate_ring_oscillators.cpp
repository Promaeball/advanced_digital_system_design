#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <iomanip>
#include <tuple>

constexpr size_t total_oscillators = 8;
constexpr size_t total_inverters = 12;
constexpr double mean = 1.0;
constexpr double sigma = 0.05;
constexpr double lower_bound = 0.85;
constexpr double upper_bound = 1.1;

std::random_device rd{};
std::mt19937 gen{ rd() };
std::normal_distribution<> d{ mean, sigma };

auto generate_random_param() -> double {
    double value;
    do {
        value = d(gen);
    } while (value < lower_bound || value > upper_bound);
    return value;
}

auto generate_size_variations() -> std::tuple<double, double, double, double> {
    return { generate_random_param(), generate_random_param(),
             generate_random_param(), generate_random_param() };
}

auto generate_thickness_variations() -> std::tuple<double, double> {
    return { generate_random_param(), generate_random_param() };
}

void generate_oscillator_subcircuit(size_t i) {
    std::string filename = "ring_oscillator_" + std::to_string(i) + ".cir";
    std::ofstream out_file(filename);

    if (!out_file) {
        std::cerr << "Error creating file: " << filename << std::endl;
        return;
    }

    out_file << std::fixed << std::setprecision(6);
    out_file << "*ring_oscillator_" << i << "\n";
    out_file << ".include nand.cir\n";
    out_file << ".include inverter.cir\n";
    out_file << ".subckt ring_oscillator_" << i << " enable output vdd vss\n";


    // NAND gate
    auto [tplv, tpwv, tnln, tnwn] = generate_size_variations();
    auto [tpotv, tnotv] = generate_thickness_variations();
    out_file << "X1 enable output n0 vdd vss nand"
        << " tplv=" << tplv << " tpwv=" << tpwv
        << " tnln=" << tnln << " tnwn=" << tnwn
        << " tpotv=" << tpotv << " tnotv=" << tnotv << "\n";

    // Inverters
    for (size_t j = 0; j < total_inverters - 1; ++j) {
        auto [tplv, tpwv, tnln, tnwn] = generate_size_variations();
        auto [tpotv, tnotv] = generate_thickness_variations();
        out_file << "X" << (j + 2) << " n" << j << " n" << (j + 1) << " vdd vss inverter"
            << " tplv=" << tplv << " tpwv=" << tpwv
            << " tnln=" << tnln << " tnwn=" << tnwn
            << " tpotv=" << tpotv << " tnotv=" << tnotv << "\n";
    }

    // Last inverter connecting back to output
    auto [final_tplv, final_tpwv, final_tnln, final_tnwn] = generate_size_variations();
    auto [final_tpotv, final_tnotv] = generate_thickness_variations();
    out_file << "X" << (total_inverters + 1) << " n" << total_inverters - 1 << " output vdd vss inverter"
        << " tplv=" << final_tplv << " tpwv=" << final_tpwv
        << " tnln=" << final_tnln << " tnwn=" << final_tnwn
        << " tpotv=" << final_tpotv << " tnotv=" << final_tnotv << "\n";

    out_file << ".ends ring_oscillator_" << i << "\n";
    out_file.close();
}

int main() {
    for (size_t i = 0; i < total_oscillators; ++i) {
        generate_oscillator_subcircuit(i);
    }
    std::cout << "Ring oscillator subcircuits generated successfully.\n";
    return 0;
}
