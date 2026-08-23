(defn m-fib [n]
  (if (= n 1)
    [1]
    (if (= n 2)
      [1 1]
      (reduce
       (fn [l val]
         (conj l (+ (last l) (first (rest (reverse l))))))
       '[1 1]
       (range (- n 2))))))

(= (m-fib 3) '(1 1 2))

(= (m-fib 6) '(1 1 2 3 5 8))

(= (m-fib 8) '(1 1 2 3 5 8 13 21))
