#include "dr_watcher.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <path_to_watch>" << std::endl;
    return EXIT_FAILURE;
  }

  try {
    DirectoryWatcher watcher(argv[1]);
    watcher.watch();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
