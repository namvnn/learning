(defn m-rotate-seq [n coll]
  (if (= n 0)
    coll
    (if (< n 0)
      (m-rotate-seq (inc n)
                    (conj (apply list (pop (vec coll)))
                          (peek (vec coll))))
      (m-rotate-seq (dec n)
                    (conj (vec (pop (apply list coll)))
                          (peek (apply list coll)))))))

(= (m-rotate-seq 2 [1 2 3 4 5]) '(3 4 5 1 2))

(= (m-rotate-seq -2 [1 2 3 4 5]) '(4 5 1 2 3))

(= (m-rotate-seq 6 [1 2 3 4 5]) '(2 3 4 5 1))

(= (m-rotate-seq 1 '(:a :b :c)) '(:b :c :a))

(= (m-rotate-seq -4 '(:a :b :c)) '(:c :a :b))
