# ocaml_solution

## Installing Dependencies

```bash
opam install . --deps-only
```

## Run

```bash
dune exec bin/main.exe < fixtures/sample.txt
dune exec bin/main.exe < fixtures/input.txt
```

## Tests

```bash
dune runtest
```
