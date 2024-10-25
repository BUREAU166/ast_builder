int f(int x) {
  if(x == 100) {
    return 0;
  }

  x++;

  return f(x);
}

int main() {
  int x = 0;
  f(x);
  return 0;
}
