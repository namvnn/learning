(defn m-rev [x] (reduce conj () x))

(= (m-rev [1 2 3 4 5]) [5 4 3 2 1])

(= (m-rev (sorted-set 5 7 2 7)) '(7 5 2))

(= (m-rev [[1 2] [3 4] [5 6]]) [[5 6] [3 4] [1 2]])
