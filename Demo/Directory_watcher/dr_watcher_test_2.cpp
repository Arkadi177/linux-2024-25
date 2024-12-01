#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <fstream>
#include "dr_watcher.h"

using namespace std::chrono;

// Helper function to simulate high-frequency events
void generate_high_frequency_events(const std::string& dir_path, int file_count) {
  std::vector<std::string> file_paths;

  // Generate file paths
  for (int i = 0; i < file_count; ++i) {
    file_paths.push_back(dir_path + "/file_" + std::to_string(i) + ".txt");
  }

  // Create all files
  for (const auto& file_path : file_paths) {
    std::ofstream file(file_path);
    file << "Initial content\n"; // Trigger IN_CREATE
    file.close();
  }

  // Modify all files
  for (const auto& file_path : file_paths) {
    std::ofstream file(file_path, std::ios::app);
    file << "Additional content\n"; // Trigger IN_MODIFY
    file.close();
  }

  // Delete all files
  for (const auto& file_path : file_paths) {
    std::remove(file_path.c_str()); // Trigger IN_DELETE
  }
}

// Benchmark high-frequency directory monitoring
void benchmark_high_frequency_events() {
  const std::string dir_path = "test_high_freq_dir";
  mkdir(dir_path.c_str(), 0777); // Create a test directory

  const int file_count = 100; // Number of files to create, modify, and delete
  DirectoryWatcher watcher(dir_path);

  auto start = high_resolution_clock::now();

  // Run the event generation in a separate thread
  std::thread event_thread(generate_high_frequency_events, dir_path, file_count);

  // Monitor directory for events
  watcher.watch();

  event_thread.join();

  auto end = high_resolution_clock::now();

  std::cout << "High-frequency event monitoring latency: "
            << duration_cast<milliseconds>(end - start).count() << " ms\n";

  rmdir(dir_path.c_str()); // Clean up
}

int main() {
  std::cout << "Starting high-frequency event benchmark...\n";
  benchmark_high_frequency_events();

  return 0;
}
