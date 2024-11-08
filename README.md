# What it does 

Builds CFG (control-flow-graph) look-alike graph of a function and its dependencies (functions and classes that it need to be defined to work)
Iterates through the whole supplies directory

# Build 

```bash
./build/build_2.sh
```

# Run

```bash
./ast legacy_project/eval.cc filter 
        ^ entry point           ^ function to searh
```

