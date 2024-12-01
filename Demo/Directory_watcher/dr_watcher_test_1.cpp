#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include "dr_watcher.h"

using namespace std::chrono;

// Helper function to generate file events
void generate_file_events(const std::string& file_path) {
    std::this_thread::sleep_for(milliseconds(100)); // Allow watcher to start
    std::ofstream file(file_path, std::ios::app);
    file << "Writing to file\n"; // Trigger IN_MODIFY
    file.close();
    std::ifstream access_file(file_path); // Trigger IN_ACCESS
    access_file.close();
    std::remove(file_path.c_str()); // Trigger IN_DELETE_SELF
}

// Helper function to generate directory events
void generate_directory_events(const std::string& dir_path) {
    std::this_thread::sleep_for(milliseconds(100)); // Allow watcher to start
    std::string file_path = dir_path + "/test_file.txt";
    std::ofstream file(file_path); // Trigger IN_CREATE
    file.close();
    std::ofstream mod_file(file_path, std::ios::app);
    mod_file << "Modifying file\n"; // Trigger IN_MODIFY
    mod_file.close();
    std::remove(file_path.c_str()); // Trigger IN_DELETE
}

// Benchmark monitoring a file
void benchmark_file_watching() {
    const std::string file_path = "test_file.txt";
    std::ofstream file(file_path); // Create a file
    file.close();

    auto start = high_resolution_clock::now();
    DirectoryWatcher watcher(file_path);

    std::thread event_thread(generate_file_events, file_path);
    std::thread watch_thread([&]() { watcher.watch(); });

    event_thread.join();
    watch_thread.join();

    auto end = high_resolution_clock::now();
    std::cout << "File monitoring latency: "
              << duration_cast<milliseconds>(end - start).count() << " ms\n";
}

// Benchmark monitoring a directory
void benchmark_directory_watching() {
    const std::string dir_path = "test_dir";
    mkdir(dir_path.c_str(), 0777); // Create a directory

    auto start = high_resolution_clock::now();
    DirectoryWatcher watcher(dir_path);

    std::thread event_thread(generate_directory_events, dir_path);
    std::thread watch_thread([&]() { watcher.watch(); });

    event_thread.join();
    watch_thread.join();

    auto end = high_resolution_clock::now();
    std::cout << "Directory monitoring latency: "
              << duration_cast<milliseconds>(end - start).count() << " ms\n";

    rmdir(dir_path.c_str()); // Clean up directory
}

int main() {
    std::cout << "Starting file monitoring benchmark...\n";
    benchmark_file_watching();

    std::cout << "Starting directory monitoring benchmark...\n";
    benchmark_directory_watching();

    return 0;
}
