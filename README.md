# What it does 

Builds Abstract Syntax Graph (in text form of *dot* or *graphviz*) for C files

# Features 

- Data-flow connections (cause its ASG not AST)
- Type information (same reason)
- Detects and connects macros to their usage (not fully)
- Shows warnings and errors 

# Build 

```bash
./build/build_2.sh
```

# Run

```bash
./ast legacy_project/eval.cc filter 
        ^ entry point           ^ function to searh
```

