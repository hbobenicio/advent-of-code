open Printf
open Unix

(* Utilities *)

let stdin_size: int = (Unix.fstat Unix.stdin).st_size

let stdin_read_all () : string = 
    let ch = open_in_bin "/dev/stdin" in
    let finally () = close_in ch in
    Fun.protect ~finally (fun () ->
        really_input_string ch stdin_size
    )

let is_not_blank (s: string) : bool =
    let trimmed = String.trim s in
    String.length trimmed > 0

let is_digit = function
    | '0' .. '9' -> true
    | _ -> false

let list_of_string (s: string) : char list =
    List.init (String.length s) (String.get s)

let seq_fold_sum = Seq.fold_left (+) 0

(* Solutions *)

let calibration_value_a (s: string) : int =
    let first: char = match s |> String.to_seq |> Seq.find is_digit with
        | None -> failwith "line is expected to contain at least a number"
        | Some c -> c
    in
    let last: char = list_of_string s
        |> List.rev
        |> List.find is_digit
    in
    int_of_string (sprintf "%c%c" first last)

let literals : string list = [
    "one"  ;
    "two"  ;
    "three";
    "four" ;
    "five" ;
    "six"  ;
    "seven";
    "eight";
    "nine" 
]

let literals_table : (string, char) Hashtbl.t =
    let table = Hashtbl.create 9 in
    let value = ref 0 in
    literals |> List.iter (fun lit ->
        incr value;
        let ascii = !value + (Char.ord '0') in
        Hashtbl.add table lit ascii
    );
    table

let calibration_value_b (s: string) : int =
    let first_char = String.get s 0 in
    (*
    - check if first char is a digit. if it is, then it's the char
    - iterate over the literals keys and check if
    *)
    let digit = if is_digit first_char then first_char else
        let rec aux () =
        in
        aux 
    in

let solve_a (lines: string list) : int =
    lines
    |> List.to_seq
    |> Seq.filter is_not_blank
    |> Seq.map calibration_value_a
    |> seq_fold_sum

let solve_b (lines: string list) : int =
    lines
    |> List.to_seq
    |> Seq.filter is_not_blank
    |> Seq.map calibration_value_b
    |> seq_fold_sum

let () =
    (* input reading and parsing *)
    let input: string = stdin_read_all () in
    let lines: string list = String.split_on_char '\n' input in

    (* Part A *)
    let a_solution: int = solve_a lines in
    printf "Part A: %d\n" a_solution;

    (* Part B *)
    let b_solution: int = solve_b lines in
    printf "Part B: %d\n" b_solution

