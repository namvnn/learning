(defn m-intervals [pred v coll]
  (if (empty? (rest coll))
    coll
    (if (pred (first coll) (second coll))
      (lazy-seq (cons (first coll) (cons v (m-intervals pred v (rest coll)))))
      (lazy-seq (cons (first coll) (m-intervals pred v (rest coll)))))))

(= '(1 :less 6 :less 7 4 3) (m-intervals < :less [1 6 7 4 3]))

(= '(2) (m-intervals > :more [2]))

(= [0 1 :x 2 :x 3 :x 4] (m-intervals #(and (pos? %) (< % %2)) :x (range 5)))

(empty? (m-intervals > :more ()))

(= [0 1 :same 1 2 3 :same 5 8 13 :same 21]
   (take 12 (->> [0 1]
                 (iterate (fn [[a b]] [b (+ a b)]))
                 (map first) ; fibonacci numbers
                 (m-intervals (fn [a b] ; both even or both odd
                                (= (mod a 2) (mod b 2)))
                              :same))))
