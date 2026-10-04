(defn m-iter [f v]
  (lazy-seq (cons v (m-iter f (f v)))))

(= (take 5 (m-iter #(* 2 %) 1)) [1 2 4 8 16])

(= (take 100 (m-iter inc 0)) (take 100 (range)))

(= (take 9 (m-iter #(inc (mod % 3)) 1)) (take 9 (cycle [1 2 3])))
