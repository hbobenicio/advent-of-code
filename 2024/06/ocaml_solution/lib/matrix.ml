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

    let create (n: int) (m: int) (value: char) : t = {
        n = n;
        m = m;
        items = Array.make_matrix n m value;
    }

    let dup (mat: t) : t =
        let dup_mat = create mat.n mat.m ' ' in
        for i = 0 to mat.n - 1 do
            for j = 0 to mat.m - 1 do
                dup_mat.items.(i).(j) <- mat.items.(i).(j)
            done
        done;
        dup_mat

    let parse (value: char) (input: string) : t =
        let lines: string list = input
            |> String.split_on_char '\n'
            |> List.filter Utils.is_not_blank
        in
        let n = List.length lines in
        let m = List.hd lines
            |> String.trim
            |> String.length
        in
        let mat = create n m value in
        lines
        |> List.iteri (fun row line ->
            let cs: char list = Utils.string_to_char_list line in
            cs |> List.iteri (fun col c ->
                mat.items.(row).(col) <- c));
        mat

    type occurence = {
        pos: int * int;
        elem: char;
    }

    let find_opt (mat: t) (value: char) : occurence option =
        let rec impl i j =
            if i >= mat.n then None else begin
                if j >= mat.m then impl (i+1) 0 else begin
                    if mat.items.(i).(j) = value then
                        Some { pos = (i, j); elem = value }
                    else
                        impl i (j+1)
                end
            end
        in
        impl 0 0

    let find (mat: t) (value: char) : occurence =
        Option.get (find_opt mat value)
end
