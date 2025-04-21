(**
 Advent of Code - Day 06 Solution
 *)
open Printf
open Matrix

module A = struct

    module Simulation = struct
        class t input_matrix initial_position = object (self)
            val grid: CharMatrix.t = input_matrix
            val mutable guard_pos: int * int = initial_position

            method guard_row = fst guard_pos
            method guard_col = snd guard_pos

            method inbounds (pos: int * int) : bool =
                let (row, col) = pos in
                   0 <= row && row < grid.n
                && 0 <= col && col < grid.m

            method is_guard_inbounds : bool =
                self#inbounds guard_pos

            method is_guard_out_of_bounds : bool =
                not self#is_guard_inbounds

            method is_over : bool =
                self#is_guard_out_of_bounds

            method guard_block : char =
                grid.items.(self#guard_row).(self#guard_col)

            method guard_visit =
                (* printf "visiting tile (%d, %d)\n" self#guard_row self#guard_col; *)
                grid.items.(self#guard_row).(self#guard_col) <- 'X'

            method guard_move_up =
                (* print_endline "moving guard up..."; *)
                let guard = self#guard_block in
                self#guard_visit;
                guard_pos <- (self#guard_row - 1, self#guard_col);
                if self#is_guard_inbounds then
                    grid.items.(self#guard_row).(self#guard_col) <- guard

            method guard_move_right =
                (* print_endline "moving guard right..."; *)
                let guard = self#guard_block in
                self#guard_visit;
                guard_pos <- (self#guard_row, self#guard_col + 1);
                if self#is_guard_inbounds then
                    grid.items.(self#guard_row).(self#guard_col) <- guard

            method guard_move_down =
                (* print_endline "moving guard down..."; *)
                let guard = self#guard_block in
                self#guard_visit;
                guard_pos <- (self#guard_row + 1, self#guard_col);
                if self#is_guard_inbounds then
                    grid.items.(self#guard_row).(self#guard_col) <- guard

            method guard_move_left =
                (* print_endline "moving guard left..."; *)
                let guard = self#guard_block in
                self#guard_visit;
                guard_pos <- (self#guard_row, self#guard_col - 1);
                if self#is_guard_inbounds then
                    grid.items.(self#guard_row).(self#guard_col) <- guard

            method pos_ahead : (int * int) =
                match self#guard_block with
                | '^' -> (self#guard_row - 1, self#guard_col    )
                | '>' -> (self#guard_row    , self#guard_col + 1)
                | 'v' -> (self#guard_row + 1, self#guard_col    )
                | '<' -> (self#guard_row    , self#guard_col - 1)
                | c -> 
                    let _ = c in
                    (* printf "bad guard tile at (%d, %d) : '%c'\n" self#guard_row self#guard_col c; *)
                    failwith "bad tile"

            method block_ahead : char option =
                let pos = self#pos_ahead in
                if self#inbounds pos then
                    let (row, col) = pos in
                    Some grid.items.(row).(col)
                else
                    None

            method guard_turn =
                (* print_endline "turning right..."; *)
                match self#guard_block with
                | '^' -> grid.items.(self#guard_row).(self#guard_col) <- '>'
                | '>' -> grid.items.(self#guard_row).(self#guard_col) <- 'v'
                | 'v' -> grid.items.(self#guard_row).(self#guard_col) <- '<'
                | '<' -> grid.items.(self#guard_row).(self#guard_col) <- '^'
                | _ -> failwith "bad tile"

            method turn_while_block_ahead =
                let quit = ref false in
                while not !quit do
                    match self#block_ahead with
                    | None ->
                        quit := true

                    | Some b ->
                        if b = '#' then
                            self#guard_turn
                        else
                            quit := true
                done

            method step =
                (* print_endline "stepping..."; *)
                self#turn_while_block_ahead;
                let guard = self#guard_block in
                match guard with
                | '^' -> self#guard_move_up
                | '>' -> self#guard_move_right
                | 'v' -> self#guard_move_down
                | '<' -> self#guard_move_left
                | _ -> failwith "bad tile"

            method run =
                while not self#is_over do
                    self#step
                done
                (* print_endline "Simulation is done." *)

            method count_visited_tiles : int =
                let count = ref 0 in
                for i = 0 to grid.n - 1 do
                    for j = 0 to grid.m - 1 do
                        if grid.items.(i).(j) = 'X' then
                            count := !count + 1
                    done
                done;
                !count
        end
    end

    let input_parse = CharMatrix.parse '.'

    let solution (input: string) : int =
        (* print_newline (); *)
        let mat = input_parse input in
        let guard_occurency = CharMatrix.find mat '^' in
        let guard_pos = guard_occurency.pos in
        (* printf "Guard Position: (%d, %d)\n" (fst guard_pos) (snd guard_pos); *)
        let simulation = new Simulation.t mat guard_pos in
        simulation#run;
        simulation#count_visited_tiles
end

module B = struct
    module Simulation = struct
        class t input_matrix initial_position = object (self)
            val grid: CharMatrix.t = input_matrix
            val mutable guard_pos: int * int = initial_position

            method guard_row = fst guard_pos
            method guard_col = snd guard_pos

            method inbounds (pos: int * int) : bool =
                let (row, col) = pos in
                   0 <= row && row < grid.n
                && 0 <= col && col < grid.m

            method is_guard_inbounds : bool =
                self#inbounds guard_pos

            method is_guard_out_of_bounds : bool =
                not self#is_guard_inbounds

            method is_over : bool =
                self#is_guard_out_of_bounds

            method guard_block : char =
                grid.items.(self#guard_row).(self#guard_col)

            method guard_visit =
                (* printf "visiting tile (%d, %d)\n" self#guard_row self#guard_col; *)
                grid.items.(self#guard_row).(self#guard_col) <- 'X'

            method guard_move_up =
                (* print_endline "moving guard up..."; *)
                let guard = self#guard_block in
                self#guard_visit;
                guard_pos <- (self#guard_row - 1, self#guard_col);
                if self#is_guard_inbounds then
                    grid.items.(self#guard_row).(self#guard_col) <- guard

            method guard_move_right =
                (* print_endline "moving guard right..."; *)
                let guard = self#guard_block in
                self#guard_visit;
                guard_pos <- (self#guard_row, self#guard_col + 1);
                if self#is_guard_inbounds then
                    grid.items.(self#guard_row).(self#guard_col) <- guard

            method guard_move_down =
                (* print_endline "moving guard down..."; *)
                let guard = self#guard_block in
                self#guard_visit;
                guard_pos <- (self#guard_row + 1, self#guard_col);
                if self#is_guard_inbounds then
                    grid.items.(self#guard_row).(self#guard_col) <- guard

            method guard_move_left =
                (* print_endline "moving guard left..."; *)
                let guard = self#guard_block in
                self#guard_visit;
                guard_pos <- (self#guard_row, self#guard_col - 1);
                if self#is_guard_inbounds then
                    grid.items.(self#guard_row).(self#guard_col) <- guard

            method pos_ahead : (int * int) =
                match self#guard_block with
                | '^' -> (self#guard_row - 1, self#guard_col    )
                | '>' -> (self#guard_row    , self#guard_col + 1)
                | 'v' -> (self#guard_row + 1, self#guard_col    )
                | '<' -> (self#guard_row    , self#guard_col - 1)
                | c -> 
                    (* let _ = c in *)
                    printf "bad guard tile at (%d, %d) : '%c'\n" self#guard_row self#guard_col c;
                    failwith "bad tile"

            method block_ahead : char option =
                let pos = self#pos_ahead in
                if self#inbounds pos then
                    let (row, col) = pos in
                    Some grid.items.(row).(col)
                else
                    None

            method guard_turn =
                (* print_endline "turning right..."; *)
                match self#guard_block with
                | '^' -> grid.items.(self#guard_row).(self#guard_col) <- '>'
                | '>' -> grid.items.(self#guard_row).(self#guard_col) <- 'v'
                | 'v' -> grid.items.(self#guard_row).(self#guard_col) <- '<'
                | '<' -> grid.items.(self#guard_row).(self#guard_col) <- '^'
                | _ -> failwith "bad tile"

            method turn_while_block_ahead =
                let quit = ref false in
                while not !quit do
                    match self#block_ahead with
                    | None ->
                        quit := true

                    | Some b ->
                        if b = '#' then
                            self#guard_turn
                        else
                            quit := true
                done

            method step =
                (* print_endline "stepping..."; *)
                self#turn_while_block_ahead;
                let guard = self#guard_block in
                match guard with
                | '^' -> self#guard_move_up
                | '>' -> self#guard_move_right
                | 'v' -> self#guard_move_down
                | '<' -> self#guard_move_left
                | _ -> failwith "bad tile"

            method run ?(step_limit=5000) () : bool =
                let step_count = ref 0 in
                while not self#is_over && !step_count < step_limit do
                    self#step;
                    step_count := !step_count + 1
                done;
                !step_count >= step_limit
                (* print_endline "Simulation is done." *)

            method count_visited_tiles : int =
                let count = ref 0 in
                for i = 0 to grid.n - 1 do
                    for j = 0 to grid.m - 1 do
                        if grid.items.(i).(j) = 'X' then
                            count := !count + 1
                    done
                done;
                !count

            method visited_tiles (guard_pos: int * int) : (int * int) list =
                let tiles = ref [] in
                for i = 0 to grid.n - 1 do
                    for j = 0 to grid.m - 1 do
                        if grid.items.(i).(j) = 'X' then begin
                            if i != (fst guard_pos) || j != (snd guard_pos) then
                                tiles := (i, j) :: !tiles
                        end
                    done
                done;
                !tiles
        end
    end

    let input_parse = CharMatrix.parse '.'

    let solution (input: string) : int =
        (* print_newline (); *)
        let mat = input_parse input in
        let guard_occurency = CharMatrix.find mat '^' in
        let guard_pos = guard_occurency.pos in
        printf "Guard Position: (%d, %d)\n" (fst guard_pos) (snd guard_pos);
        let simulation = new Simulation.t mat guard_pos in
        let _ = simulation#run () in
        let count_visited = simulation#count_visited_tiles in
        printf "Visited Tiles Count: %d\n" count_visited;
        let visited_tiles = simulation#visited_tiles guard_pos in
        printf "Len Visited Tiles: %d\n" (List.length visited_tiles);
        let loop_count = ref 0 in
        visited_tiles |> List.iter begin fun visited_tile ->
            let o_row = fst visited_tile
            and o_col = snd visited_tile in
            let mat_prime: CharMatrix.t = CharMatrix.dup mat in
            mat_prime.items.(o_row).(o_col) <- '#';
            mat_prime.items.(fst guard_pos).(snd guard_pos) <- '^';
            let simulation = new Simulation.t mat_prime guard_pos in
            let loop = simulation#run ~step_limit:15000 () in
            if loop then
                loop_count := !loop_count + 1
        end;
        !loop_count
end
