(defn m-last [x] (first (reverse x)))

(= (m-last [1 2 3 4 5]) 5)

(= (m-last '(5 4 3)) 3)

(= (m-last ["b" "c" "d"]) "d")
