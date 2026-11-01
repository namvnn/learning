(defn m-map [f coll]
  (if (empty? coll) '()
    (lazy-seq (cons (f (first coll)) (m-map f (rest coll))))))

(= [3 4 5 6 7]
   (m-map inc [2 3 4 5 6]))

(= (repeat 10 nil)
   (m-map (fn [_] nil) (range 10)))

(= [1000000 1000001]
   (->> (m-map inc (range))
        (drop (dec 1000000))
        (take 2)))

(= [1000000 1000001]
   (->> (m-map inc (range))
        (drop (dec 1000000))
        (take 2)))
