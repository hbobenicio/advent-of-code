open Printf
open Ocaml_solution

let () =
    let input: string = Utils.stdin_read_all () in
    print_string "Part A: ";
    printf "%d\n" (Solution.A.solution input)
