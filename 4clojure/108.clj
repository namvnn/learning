(defn m-lazy-searching [& args]
  (let [fs (map first args)
        ss (sort-by first args)
        k (lazy-seq (cons (rest (first ss)) (rest ss)))]
    (if (apply = fs) (first fs) (apply m-lazy-searching k))))

(= 3 (m-lazy-searching [3 4 5]))

(= 4 (m-lazy-searching [1 2 3 4 5 6 7] [0.5 3/2 4 19]))

(= 64 (m-lazy-searching (map #(* % % %) (range))
                        (filter #(zero? (bit-and % (dec %))) (range))
                        (iterate inc 20)))
