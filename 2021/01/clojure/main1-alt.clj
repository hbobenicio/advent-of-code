(ns main1-alt
    (:require [clojure.string :as str])
    (:gen-class))

(def input (slurp *in*))

(defn parse-line [s]
  (Integer/parseInt s))

(def parse-lines
  (partial map parse-line))

(defn parse-input [s]
  (parse-lines (str/split-lines s)))

(defn solve [xs]
  (def window-size 2)
  (def xs-grouped-by-2
    (partition window-size 1 xs))
  (count
    (filter
      (fn [p] (< (first p) (second p)))
      xs-grouped-by-2)))

(defn -main [& args]
  (let [xs (parse-input input)]
    (-> xs
        (solve)
        (println))))

(-main)

