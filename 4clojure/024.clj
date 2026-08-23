(defn m-sum [x] (reduce + x))

(= (m-sum [1 2 3]) 6)

(= (m-sum (list 0 -2 5 5)) 8)

(= (m-sum #{4 2 1}) 7)

(= (m-sum '(0 0 -1)) -1)

(= (m-sum '(1 10 3)) 14)
