(defn m-nth [s n]
  (if (zero? n)
    (first s)
    (m-nth (rest s) (dec n))))

(= (m-nth '(4 5 6 7) 2) 6)

(= (m-nth [:a :b :c] 0) :a)

(= (m-nth [1 2 3 4] 1) 2)

(= (m-nth '([1 2] [3 4] [5 6]) 2) [5 6])
