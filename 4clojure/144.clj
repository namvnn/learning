(defn m-oscilrate [v & fns]
  (let [fl (apply list fns)]
    (lazy-seq (cons v
                    (apply m-oscilrate
                     ((peek fl) v)
                     (concat (pop fl) (list (peek fl))))))))

(= (take 3 (m-oscilrate 3.14 int double)) [3.14 3 3.0])

(= (take 5 (m-oscilrate 3 #(- % 3) #(+ 5 %))) [3 0 5 2 7])

(= (take 12 (m-oscilrate 0 inc dec inc dec inc)) [0 1 0 1 0 1 2 1 2 1 2 3])
