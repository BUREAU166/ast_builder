union un {
  int i;
  float f;
};

int main() {
  union un data;
  data.i = 1;
  data.f = 9.9f;

  return 0;
}
