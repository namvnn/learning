(defn get-penultimate [x] (first (rest (reverse x))))

(= (get-penultimate (list 1 2 3 4 5)) 4)

(= (get-penultimate ["a" "b" "c"]) "b")

(= (get-penultimate [[1 2] [3 4]]) [1 2])
