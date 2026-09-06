(defn m-rep-seq [x n]
  (reduce
   (fn [l v] (apply conj l (repeat n v)))
   (vec (repeat n (first x)))
   (rest x)))

(= (m-rep-seq [1 2 3] 2) '(1 1 2 2 3 3))

(= (m-rep-seq [:a :b] 4) '(:a :a :a :a :b :b :b :b))

(= (m-rep-seq [4 5 6] 1) '(4 5 6))

(= (m-rep-seq [[1 2] [3 4]] 2) '([1 2] [1 2] [3 4] [3 4]))

(= (m-rep-seq [44 33] 2) [44 44 33 33])
