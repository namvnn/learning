(defn m-prime [c]
  (defn m-is-prime? [n]
    (cond
      (or (= n 0) (= n 1)) false
      :else (= '(1) (filter #(= 0 (mod n %)) (range 1 n)))))
  (take c (filter m-is-prime? (range))))

(= (m-prime 2) [2 3])

(= (m-prime 5) [2 3 5 7 11])

(= (last (m-prime 100)) 541)
