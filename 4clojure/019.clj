(defn get-last [x] (first (reverse x)))

(= (get-last [1 2 3 4 5]) 5)

(= (get-last '(5 4 3)) 3)

(= (get-last ["b" "c" "d"]) "d")
