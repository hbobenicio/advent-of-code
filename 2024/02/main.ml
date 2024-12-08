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

module A = struct

    module Report = struct
      type t = int list

      let parse (line: string) : t =
        line
        |> String.split_on_char ' '
        |> List.filter is_not_blank
        |> List.map int_of_string

      let diff_reduce (xs: t) : int list =
        let res = ref [] in
        for i = 0 to (List.length xs - 2) do
            let x = List.nth xs i in
            let y = List.nth xs (i+1) in
            let diff = x - y in
            res := !res @ [diff]
        done;
        !res
    end

    module Model = struct
        type t = Report.t list
    end

    module Safety = struct
        type t = Safe | Unsafe
    end

    let parse (input: string) : Model.t =
        let lines: string list = input
            |> String.split_on_char '\n'
            |> List.filter is_not_blank
        in
        List.map Report.parse lines

    let get_safety (report: Report.t) : Safety.t =
        let diff: int list   = Report.diff_reduce report in
        let are_all_positive = List.for_all ((<) 0) in
        let are_all_negative = List.for_all ((>) 0) in
        let are_all_in_range = List.for_all (fun x -> let v = abs x in 1 <= v && v <= 3) in
        let same_polarity: bool = (are_all_positive diff) || (are_all_negative diff) in
        let all_in_range: bool = are_all_in_range diff in
        if same_polarity && all_in_range then
            Safety.Safe
        else
            Safety.Unsafe

    let solve (model: Model.t) : int =
        let fold_incr_if_safe (safety_count: int) = function
        | Safety.Safe   -> safety_count + 1
        | Safety.Unsafe -> safety_count
        in
        model
        |> List.map get_safety
        |> List.fold_left fold_incr_if_safe 0

    let solution (input: string) : int = input |> parse |> solve
end

module B = struct

    module Report = struct
      type t = int list

      let parse (line: string) : t =
        line
        |> String.split_on_char ' '
        |> List.filter is_not_blank
        |> List.map int_of_string

      let diff_reduce (xs: t) : int list =
        let res = ref [] in
        for i = 0 to (List.length xs - 2) do
            let x = List.nth xs i in
            let y = List.nth xs (i+1) in
            let diff = x - y in
            res := !res @ [diff]
        done;
        !res
    end

    module Model = struct
        type t = Report.t list
    end

    module Safety = struct
        type t = Safe | Unsafe

        let to_string = function
        | Safe   -> "Safe"
        | Unsafe -> "Unsafe"
    end

    let parse (input: string) : Model.t =
        let lines: string list = input
            |> String.split_on_char '\n'
            |> List.filter is_not_blank
        in
        List.map Report.parse lines

    let is_same_polarity (report_diff: int list) : bool =
        let are_all_positive = List.for_all ((<) 0) in
        let are_all_negative = List.for_all ((>) 0) in
        are_all_positive report_diff || are_all_negative report_diff

    let get_safety (report: Report.t) : Safety.t =
        let report_length = List.length report in
        let diff: int list = Report.diff_reduce report in
        let in_range (x: int) =
            let x = abs x in
            1 <= x && x <= 3
        in
        let are_all_in_range: bool = List.for_all in_range diff in
        let same_polarity: bool = is_same_polarity diff in
        if same_polarity && are_all_in_range then Safety.Safe else begin
            (* iterate over report, remove an item, try next. *)
            let result = ref Safety.Unsafe in
            let indices: int list = List.of_seq (Seq.take report_length (Seq.ints 0)) in
            let report_assoc: (int * int) list = List.combine indices report in
            indices |> List.iter (fun i ->
                let (_, report) = List.split (List.remove_assoc i report_assoc) in
                let diff: int list = Report.diff_reduce report in
                let are_all_in_range: bool = List.for_all in_range diff in
                let same_polarity: bool = is_same_polarity diff in
                if same_polarity && are_all_in_range then
                    result := Safety.Safe
            );
            !result
        end

    let solve (model: Model.t) : int =
        let fold_incr_if_safe (safety_count: int) = function
        | Safety.Safe   -> safety_count + 1
        | Safety.Unsafe -> safety_count
        in
        let model_safety = List.map get_safety model in
        List.fold_left fold_incr_if_safe 0 model_safety

    let solution (input: string) : int = input |> parse |> solve
end

let () =
    let input: string = stdin_read_all () in

    print_string "Part A: ";
    printf "%d\n" (A.solution input);

    print_string "Part B: ";
    printf "%d\n" (B.solution input)
