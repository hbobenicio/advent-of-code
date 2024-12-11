open Ocaml_solution
open Printf

let () =
    let input: string = Utils.stdin_read_all () in
    print_string "Part A: ";
    printf "%d\n" (Solution.A.solution input);
    print_string "Part B: ";
    printf "%d\n" (Solution.B.solution input)
