(ns main2
    (:import [java.io BufferedReader])
    (:gen-class))

(defn parse-line
    "Parses a single line of input"
    [line]
    (Integer/parseInt line))

(defn parse-lines
    "Parses all lines of input. Just calls parse-line for all of them"
    [lines]
    (map parse-line lines))

(defn read-lines
    "Read lines from stdin as a lazy sequence of strings"
    []
    (line-seq (BufferedReader. *in*)))

(defn group-3
    [xs]
    (for [i (range 0 (- (count xs) 2))]
        [(nth xs i) (nth xs (+ i 1)) (nth xs (+ i 2))]))

(defn solve
    "Solve the logic of the problem"
    [xs]
    (group-3 xs)
    )

(defn -main [& args]
    (let [xs (parse-lines (read-lines))]
        (println (solve xs))))

(-main)
