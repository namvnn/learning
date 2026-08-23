(defn m-penultimate [x] (first (rest (reverse x))))

(= (m-penultimate (list 1 2 3 4 5)) 4)

(= (m-penultimate ["a" "b" "c"]) "b")

(= (m-penultimate [[1 2] [3 4]]) [1 2])
