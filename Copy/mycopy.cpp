#include "../Utils/Iterator/Directory.h"
#include "../Utils/Iterator/ArgumentParser.h"

void copy(std::string src, std::string dest) {
  Directory dir(src);
  Directory destDir(dest);
  // if(dir.begin().get_d_type() == )
}

int main(int argc , char ** argv) {
  ArgumentParser args(argc , argv , "rnf");
  return 0;
}