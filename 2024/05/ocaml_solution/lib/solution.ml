(* open Printf *)

module A = struct
    type rule = int * int
    type update = int list

    let parse_rule (s: string) : rule =
        match String.split_on_char '|' s with
        | [x; y] -> (int_of_string x, int_of_string y)
        | _ -> failwith "bad rule"

    let%test _ = parse_rule "61|29" = (61, 29)

    let parse_update (s: string) : update =
        List.map int_of_string (String.split_on_char ',' s)

    let%test _ = parse_update "75,47,61,53,29" = [75; 47; 61; 53; 29]

    let input_parse (input: string) : rule list * update list =
        let blocks: string * string =  match String.split_on_char '=' input with  (* is this cheating? *)
            | [rules_input; updates_input] -> (rules_input, updates_input)
            | _ -> failwith "bad input"
        in
        let rules: rule list = fst blocks
            |> String.split_on_char '\n'
            |> List.filter Utils.is_not_blank
            |> List.map parse_rule
        in
        let updates: update list = snd blocks
            |> String.split_on_char '\n'
            |> List.filter Utils.is_not_blank
            |> List.map parse_update
        in
        (rules, updates)

    let check_rule (r: rule) (upd: update) : bool =
        match Utils.index_of (==) (fst r) upd with
        | None -> true
        | Some idx_a ->
            match Utils.index_of (==) (snd r) upd with
            | None -> true
            | Some idx_b ->
                idx_a < idx_b
    
    let solution (input: string) : int =
        let parsed_input: rule list * update list = input_parse input in
        let rules: rule list = fst parsed_input in
        let updates: update list = snd parsed_input in
        let filtered_updates: update list = updates |> List.filter (fun update ->
            List.for_all (fun r -> check_rule r update) rules
        ) in
        let middle_elems: int list = filtered_updates |> List.map (fun update ->
            let middle_index = List.length update / 2 in
            List.nth update middle_index) in
        List.fold_left (+) 0 middle_elems
end

module B = struct
    type rule = int * int
    type update = int list

    let parse_rule (s: string) : rule =
        match String.split_on_char '|' s with
        | [x; y] -> (int_of_string x, int_of_string y)
        | _ -> failwith "bad rule"

    let%test _ = parse_rule "61|29" = (61, 29)

    let string_of_rule (r: rule) : string = 
        let s1 = string_of_int (fst r) in
        let s2 = string_of_int (snd r) in
        s1 ^ "|" ^ s2

    let parse_update (s: string) : update =
        List.map int_of_string (String.split_on_char ',' s)

    let%test _ = parse_update "75,47,61,53,29" = [75; 47; 61; 53; 29]

    let update_println (upd: update) =
        print_string "[ ";
        upd |> List.iteri (fun i x -> (if i > 0 then print_string ", "); print_int x);
        print_endline "]"

    let input_parse (input: string) : rule list * update list =
        let blocks: string * string =  match String.split_on_char '=' input with  (* is this cheating? *)
            | [rules_input; updates_input] -> (rules_input, updates_input)
            | _ -> failwith "bad input"
        in
        let rules: rule list = fst blocks
            |> String.split_on_char '\n'
            |> List.filter Utils.is_not_blank
            |> List.map parse_rule
        in
        let updates: update list = snd blocks
            |> String.split_on_char '\n'
            |> List.filter Utils.is_not_blank
            |> List.map parse_update
        in
        (rules, updates)

    let check_rule (r: rule) (upd: update) : bool =
        match Utils.index_of (==) (fst r) upd with
        | None -> true
        | Some idx_a ->
            match Utils.index_of (==) (snd r) upd with
            | None -> true
            | Some idx_b ->
                idx_a < idx_b

    let solution (input: string) : int =
        let parsed_input: rule list * update list = input_parse input in
        let rules: rule list = fst parsed_input in
        let updates: update list = snd parsed_input in

        (* filters out correctly ordered updates *)
        let updates: update list = updates |> List.filter (fun update ->
            List.exists (fun r -> not (check_rule r update)) rules
        ) in

        (* map updates to sorted updates *)
        let updates: update list = updates |> List.map (fun upd -> 
            upd |> List.sort (fun (x: int) (y: int) : int ->
                let xy_rules: rule list = List.filter (fun r -> fst r = x && snd r = y) rules in
                if List.length xy_rules > 0 then -1 else 1
            )
        ) in

        let middle_elems: int list = updates |> List.map (fun update ->
            let middle_index = List.length update / 2 in
            List.nth update middle_index) in
        List.fold_left (+) 0 middle_elems
end
