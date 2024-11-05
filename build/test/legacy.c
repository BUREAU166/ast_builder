int superSort(int n) {
  n = 0;
  return n;
}

int filterFiles(int file) {
  int newF = 1;
  switch(file) {
    case 1: {
      newF = superSort(file);
    } break;
  }

  return newF;
}

int unFilter(int file) {
  int newF = 1;
  return newF;
}

int main() {
  filterFiles(1);
  unFilter(1);

  return 0;
}
