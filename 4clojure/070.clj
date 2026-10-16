(defn m-word-sort [s]
  (sort-by clojure.string/lower-case (clojure.string/split s #"\W")))

(= (m-word-sort  "Have a nice day.")
   ["a" "day" "Have" "nice"])

(= (m-word-sort  "Clojure is a fun language!")
   ["a" "Clojure" "fun" "is" "language"])

(= (m-word-sort  "Fools fall for foolish follies.")
   ["fall" "follies" "foolish" "Fools" "for"])
