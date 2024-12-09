(**
 * $ ocaml unix.cma str.cma main.ml < sample.txt
 *)
open Printf
open Str

let stdin_read_all () : string = 
    let ch = open_in_bin "/dev/stdin" in
    let finally () = close_in ch in
    Fun.protect ~finally (fun () ->
        let stdin_size: int = (Unix.fstat Unix.stdin).st_size in
        really_input_string ch stdin_size
    )

module A = struct
    let solution (input: string) : int =
        let r = Str.regexp {|mul(\([0-9]+\),\([0-9]+\))|} in
        let rec find_all (values: (int * int) list) (rest: string) : (int * int) list =
            try
                let match_index: int = Str.search_forward r rest 0 in
                let lexeme: string = Str.matched_string rest in
                let lexeme_len: int = String.length lexeme in
                let v1: int = int_of_string (Str.matched_group 1 rest) in
                let v2: int = int_of_string (Str.matched_group 2 rest) in
                let offset = match_index + lexeme_len in
                find_all ((v1, v2) :: values) (String.sub rest offset (String.length rest - offset))
            with Not_found -> values
        in
        let values: (int * int) list = List.rev (find_all [] input) in
        values
        |> List.map (fun (x, y) -> x * y)
        |> List.fold_left (+) 0
end

module B = struct
    let solution (input: string) : int =
        printf "\nInput: %s\n" input;
        let r = Str.regexp {|do()\|don't()\|mul(\([0-9]+\),\([0-9]+\))|} in
        let rec find_all (values: (int * int) list) (enabled: bool) (rest: string) : (int * int) list =
            try
                let match_index: int = Str.search_forward r rest 0 in
                let lexeme: string = Str.matched_string rest in
                let lexeme_len: int = String.length lexeme in
                let offset = match_index + lexeme_len in
                let remaining: string = String.sub rest offset (String.length rest - offset) in
                match lexeme with
                | "do()" ->
                    print_endline "do()";
                    find_all values true remaining
                | "don't()" ->
                    print_endline "don't()";
                    find_all values false remaining
                | mul -> 
                    printf "%s (enabled: %s)\n" lexeme (string_of_bool enabled);
                    if not enabled then
                        find_all values enabled remaining
                    else begin
                        let v1: int = int_of_string (Str.matched_group 1 rest) in
                        let v2: int = int_of_string (Str.matched_group 2 rest) in
                        let pair = (v1, v2) in
                        find_all (pair :: values) enabled remaining
                    end
            with Not_found ->
                print_endline "Not more matches found.";
                List.rev values
        in
        let values: (int * int) list = find_all [] true input in
        values
        |> List.map (fun (x, y) -> x * y)
        |> List.fold_left (+) 0
end

let () =
    let input: string = stdin_read_all () in
    print_string "Part A: ";
    printf "%d\n" (A.solution input);
    print_string "Part B: ";
    printf "%d\n" (B.solution input)
