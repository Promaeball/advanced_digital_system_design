#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

const int NUM_OSCILLATORS = 8;  // Number of oscillators
const double THRESHOLD = 0.0001;  // Threshold to handle signal fluctuations 

// Structure to store oscillator data
struct OscillatorData {
    std::vector<double> times; 
    std::vector<std::vector<double>> values; 
    std::vector<int> risingEdges;  
};

// Function to read the CSV file and load oscillator data
OscillatorData readCSV(const std::string& filename) {
    OscillatorData data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::string line;

    std::getline(file, line);

    // Read the data
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        std::vector<double> row;



        std::getline(iss, token, ',');
        data.times.push_back(std::stod(token));

        // Read oscillator values
        for (int i = 0; i < NUM_OSCILLATORS; ++i) {
            std::getline(iss, token, ',');
            row.push_back(std::stod(token));
        }
        data.values.push_back(row);
    }

    return data;
}

// Function to detect rising edges for each oscillator with debounce logic
void detectRisingEdges(OscillatorData& data) {
    data.risingEdges.resize(NUM_OSCILLATORS, 0);

    for (int i = 0; i < NUM_OSCILLATORS; ++i) {
        bool inHighState = false;  // Flag to track if the signal is in a high state

        for (size_t j = 0; j < data.values.size(); ++j) {
            double currentSignal = data.values[j][i];  // Current signal value

            if (!inHighState && currentSignal > THRESHOLD) {
                // Rising edge detected
                data.risingEdges[i]++;
                inHighState = true;  // Enter high state
            }

            // Signal drops below zero (or near zero), reset the flag
            if (inHighState && currentSignal < -THRESHOLD) {
                inHighState = false;
            }
        }
    }
}

// Function to save results to a CSV file
void saveResults(const OscillatorData& data, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw std::runtime_error("Could not open output file: " + filename);
    }

    // Write the results
    outFile << "Oscillator,Rising Edges\n";
    for (int i = 0; i < NUM_OSCILLATORS; ++i) {
        outFile << i + 1 << "," << data.risingEdges[i] << "\n";
    }
    outFile.close();
}

int main() {
    // File paths
    const std::string csvFilePath = "C:\\Users\\kathy\\source\\oscillators_waveforms.csv";
    const std::string outputFilePath = "C:\\Users\\kathy\\source\\rising_edge_counter_output.csv";

    try {
        // Read the oscillator data from the CSV file
        OscillatorData data = readCSV(csvFilePath);

        // Detect rising edges with debouncing logic
        detectRisingEdges(data);

        // Save the results to a file
        saveResults(data, outputFilePath);

        // Print the results to the console
        std::cout << "Rising Edges for each Oscillator:" << std::endl;
        for (int i = 0; i < NUM_OSCILLATORS; ++i) {
            std::cout << "Oscillator " << i + 1 << ": " << data.risingEdges[i] << " rising edges" << std::endl;
        }

        std::cout << "\nResults saved to: " << outputFilePath << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
