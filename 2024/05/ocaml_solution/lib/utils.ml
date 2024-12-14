let stdin_read_all () : string = 
    (** stdin_read_all reads all bytes from stdin and creates a string out of it. *)
    let ch = open_in_bin "/dev/stdin" in
    let finally () = close_in ch in
    Fun.protect ~finally (fun () ->
        let stdin_size: int = (Unix.fstat Unix.stdin).st_size in
        really_input_string ch stdin_size
    )

let is_not_blank (s: string) : bool =
    let s = String.trim s in
    String.length s > 0

let%test _ = is_not_blank ""  == false
let%test _ = is_not_blank " " == false
let%test _ = is_not_blank "a" == true

let string_to_char_list (s: string): char list =
    s |> String.to_seq |> List.of_seq

let%test _ = List.equal (==) (string_to_char_list "XMAS") ['X'; 'M'; 'A'; 'S']

let char_list_to_string (cs: char list) : string = 
    String.of_seq (List.to_seq cs)

let%test _ = String.equal (char_list_to_string ['X'; 'M'; 'A'; 'S']) "XMAS"

let list_count predicate xs =
    let rec reduce (count: int) = function
        | [] -> count
        | x :: rest ->
            if predicate x then
                reduce (count + 1) rest
            else
                reduce count rest
    in 
    reduce 0 xs

let%test _ = (list_count (String.equal "XMAS") []) == 0
let%test _ = (list_count (String.equal "XMAS") ["MMMS"; "XMAS"; "MMAM"]) == 1

let index_of eql x xs : int option =
    let rec aux (pos: int) xs =
        match xs with
        | [] -> None
        | head :: tail ->
            if eql head x then
                Some pos
            else
                aux (pos + 1) tail
    in
    aux 0 xs

let%test _ = Option.equal (==) (index_of String.equal "B" ["A"; "B"; "C"]) (Some 1)
let%test _ = Option.equal (==) (index_of String.equal "D" ["A"; "B"; "C"]) None
