open Printf

module CharMatrix = struct
    type t = {
        n: int;
        m: int;
        items: char array array;
    }

    let println (mat: t) =
        for i = 0 to mat.n - 1 do
            for j = 0 to mat.m - 1 do
                printf "%c " mat.items.(i).(j)
            done;
            print_newline ()
        done

    let create (n: int) (m: int) : t = {
        n = n;
        m = m;
        items = Array.make_matrix n m '.';
    }
end

module A = struct
    let offsets = [
        [ (0,0); ( 0, 1); ( 0, 2); ( 0, 3) ];  (** E  *)
        [ (0,0); ( 1, 1); ( 2, 2); ( 3, 3) ];  (** ES *)
        [ (0,0); ( 1, 0); ( 2, 0); ( 3, 0) ];  (** S *)
        [ (0,0); ( 1,-1); ( 2,-2); ( 3,-3) ];  (** WS *)
        [ (0,0); ( 0,-1); ( 0,-2); ( 0,-3) ];  (** W  *)
        [ (0,0); (-1,-1); (-2,-2); (-3,-3) ];  (** WN *)
        [ (0,0); (-1, 0); (-2, 0); (-3, 0) ];  (** N *)
        [ (0,0); (-1, 1); (-2, 2); (-3, 3) ];  (** NE *)
    ]
    let%test _ = List.length offsets == 8

    type coord = int * int
    type coords = coord list

    let coord_add (c1: coord) (c2: coord) = (fst c1 + fst c2, snd c1 + snd c2)
    let coord_print (c: coord) = printf "(%d, %d)" (fst c) (snd c)
    let coord_out_of_bounds (n: int) (m: int) (c: coord) : bool = 
        let x: int = fst c
        and y: int = snd c in
        let underflow : bool = x < 0  || y < 0  in
        let overflow  : bool = x >= n || y >= m in
        underflow || overflow

    let coord_in_bounds (n: int) (m: int) (c: coord) : bool =
        not (coord_out_of_bounds n m c)

    let coords_println (cs: coords) =
        cs
        |> List.iter (fun c -> coord_print c; print_char ' ');
        print_newline ()

    let xmas_count (words: string list) : int =
        Utils.list_count (String.equal "XMAS") words

    let solution (input: string) : int =
        let lines: string list = input
            |> String.split_on_char '\n'
            |> List.filter Utils.is_not_blank
        in
        let n = List.length lines in
        let m = List.hd lines
            |> String.trim
            |> String.length
        in
        (* printf "Matrix: %dx%d\n" n m; *)
        let mat: CharMatrix.t = CharMatrix.create n m in
        lines
        |> List.iteri (fun row line ->
            let cs: char list = Utils.string_to_char_list line in
            cs |> List.iteri (fun col c ->
                mat.items.(row).(col) <- c));
        (* CharMatrix.println mat; *)
        let total = ref 0 in
        for i = 0 to mat.n - 1 do
            for j = 0 to mat.m - 1 do
                if mat.items.(i).(j) == 'X' then begin
                    let ij: coord = (i, j) in
                    let coords_map_add (cs: coords) : coords = List.map (coord_add ij) cs in
                    let positions: coords list = List.map coords_map_add offsets in
                    (* (if i == 0 && j == 0 then begin
                        printf "---\n";
                        List.iter coords_println positions
                    end); *)
                    let valid_positions: coords list = List.filter (fun cs -> List.for_all (coord_in_bounds mat.n mat.m) cs) positions in
                    (* (if i == 0 && j == 0 then begin
                        printf "---\n";
                        List.iter coords_println valid_positions
                    end); *)
                    let char_elems: char list list = List.map (fun cs -> List.map (fun (x, y) -> mat.items.(x).(y)) cs) valid_positions in
                    let words: string list = List.map Utils.char_list_to_string char_elems in
                    (* (if i == 0 && j == 0 then begin
                        printf "---\n";
                        List.iter print_endline words
                    end); *)
                    let count = xmas_count words in
                    total := !total + count
                end
            done;
        done;
        !total
end

module B = struct
    type coord = int * int
    type coords = coord list

    let coord_add (c1: coord) (c2: coord) = (fst c1 + fst c2, snd c1 + snd c2)
    let coord_print (c: coord) = printf "(%d, %d)" (fst c) (snd c)
    let coord_out_of_bounds (n: int) (m: int) (c: coord) : bool = 
        let x: int = fst c
        and y: int = snd c in
        let underflow : bool = x < 0  || y < 0  in
        let overflow  : bool = x >= n || y >= m in
        underflow || overflow

    let coord_in_bounds (n: int) (m: int) (c: coord) : bool =
        not (coord_out_of_bounds n m c)

    let coords_println (cs: coords) =
        cs
        |> List.iter (fun c -> coord_print c; print_char ' ');
        print_newline ()

    let string_is_mas (s: string) : bool =
        String.equal s "MAS" || String.equal s "SAM"

    let x_mas (words: string list) : bool =
        match words with
        | [s1; s2] -> string_is_mas s1 && string_is_mas s2
        | _ -> false

    let solution (input: string) : int =
        let lines: string list = input
            |> String.split_on_char '\n'
            |> List.filter Utils.is_not_blank
        in
        let n = List.length lines in
        let m = List.hd lines
            |> String.trim
            |> String.length
        in
        let mat: CharMatrix.t = CharMatrix.create n m in
        lines
        |> List.iteri (fun row line ->
            let cs: char list = Utils.string_to_char_list line in
            cs |> List.iteri (fun col c ->
                mat.items.(row).(col) <- c));
        let total = ref 0 in
        for i = 0 to mat.n - 1 do
            for j = 0 to mat.m - 1 do
                if mat.items.(i).(j) == 'A' then begin
                    let positions: coords list = [
                        [(i-1, j-1); (i,j); (i+1, j+1)];
                        [(i+1, j-1); (i,j); (i-1, j+1)];
                    ] in
                    let valid_positions: coords list = List.filter (fun cs -> List.for_all (coord_in_bounds mat.n mat.m) cs) positions in
                    let char_elems: char list list = List.map (fun cs -> List.map (fun (x, y) -> mat.items.(x).(y)) cs) valid_positions in
                    let words: string list = List.map Utils.char_list_to_string char_elems in
                    if List.length words > 0 then begin
                        (if List.length words != 2 then failwith (sprintf "assert failed. words len = %d" (List.length words)));
                        if x_mas words then begin
                            total := !total + 1;
                        end
                    end
                end
            done;
        done;
        !total
end
