(**
 * $ ocaml unix.cma main.ml
 *)
open Printf

let stdin_read_all () : string = 
    let ch = open_in_bin "/dev/stdin" in
    let finally () = close_in ch in
    Fun.protect ~finally (fun () ->
        let stdin_size: int = (Unix.fstat Unix.stdin).st_size in
        really_input_string ch stdin_size
    )

let is_not_blank (s: string) : bool =
    let trimmed = String.trim s in
    String.length trimmed > 0

let sort_asc = List.sort compare

module Model = struct
    type t = (int list) * (int list)
end

module A = struct

    let parse_line (line: string) : int list =
        line
        |> String.split_on_char ' '
        |> List.filter is_not_blank
        |> List.map int_of_string

    let parse (input: string) : Model.t  =
        let lhs = ref []
        and rhs = ref [] in
        let lines: string list = input
            |> String.split_on_char '\n'
            |> List.filter is_not_blank
        in
        List.iter (fun line -> 
            let fields: int list = parse_line line in
            lhs := List.nth fields 0 :: !lhs;
            rhs := List.nth fields 1 :: !rhs;
        ) lines;
        (!lhs, !rhs)

    let distance ((x: int), (y: int)) : int = abs (x - y)

    let solve (model: Model.t) : int =
        let lhs = sort_asc (fst model)
        and rhs = sort_asc (snd model) in
        List.combine lhs rhs
        |> List.map distance
        |> List.fold_left (+) 0

    let solution (input: string) : int = input |> parse |> solve
end

module B = struct
    let parse_line (line: string) : int list =
        line
        |> String.split_on_char ' '
        |> List.filter is_not_blank
        |> List.map int_of_string

    let parse (input: string) : Model.t  =
        let lhs = ref []
        and rhs = ref [] in
        let lines: string list = input
            |> String.split_on_char '\n'
            |> List.filter is_not_blank
        in
        List.iter (fun line -> 
            let fields: int list = parse_line line in
            lhs := List.nth fields 0 :: !lhs;
            rhs := List.nth fields 1 :: !rhs;
        ) lines;
        (!lhs, !rhs)

    let solve (model: Model.t) : int =
        let histogram: (int, int) Hashtbl.t = Hashtbl.create 0 in
        List.iter (fun (x: int) ->
            match Hashtbl.find_opt histogram x with
            | Some value -> Hashtbl.replace histogram x (value + 1)
            | None -> Hashtbl.add histogram x 1
        ) (snd model);
        let similarity_score = ref 0 in
        List.iter (fun x ->
            match Hashtbl.find_opt histogram x with
            | None -> ()
            | Some value -> similarity_score := !similarity_score + (x * value)
        ) (fst model);
        !similarity_score

    let solution (input: string) : int = input |> parse |> solve
end

let () =
    let input: string = stdin_read_all () in
    printf "Part A: %d\n" (A.solution input);
    printf "Part B: %d\n" (B.solution input)
