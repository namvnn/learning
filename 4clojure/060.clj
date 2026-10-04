(defn m-seq-reduce
  ([f coll]
   (m-seq-reduce f (first coll) (rest coll)))
  ([f val coll]
   (lazy-seq
    (if (empty? coll)
      [val]
      (cons val (m-seq-reduce f (f val (first coll)) (rest coll)))))))

(= (take 5 (m-seq-reduce + (range))) [0 1 3 6 10])

(= (m-seq-reduce conj [1] [2 3 4]) [[1] [1 2] [1 2 3] [1 2 3 4]])

(= (last (m-seq-reduce * 2 [3 4 5])) (reduce * 2 [3 4 5]) 120)
