(ns main1
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

(defn solve
    "Solve the logic of the problem"
    [[x & tail]]
    (if (nil? tail)
        0
        (if (< x (first tail))
            (+ 1 (solve tail))
            (solve tail))))

(defn -main [& args]
    (let [xs (parse-lines (read-lines))]
        (println (solve xs))))

(-main)
