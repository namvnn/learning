(defn m-seq-pronunciations [coll]
  (let [n (mapcat
           #(vector (count %) (first %))
           (partition-by identity coll))]
    (lazy-seq (cons n (m-seq-pronunciations n)))))


(= [3 1 2 4] (first (m-seq-pronunciations [1 1 1 4 4])))

(= [1 1 1 3 2 1 3 2 1 1] (nth (m-seq-pronunciations [1]) 6))

(= 338 (count (nth (m-seq-pronunciations [3 2]) 15)))
