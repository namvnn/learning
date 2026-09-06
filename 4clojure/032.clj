(defn m-dup-seq [x]
  (reduce
   (fn [l v] (conj l v v))
   [(first x) (first x)]
   (rest x)))

(= (m-dup-seq [1 2 3]) '(1 1 2 2 3 3))

(= (m-dup-seq [:a :a :b :b]) '(:a :a :a :a :b :b :b :b))

(= (m-dup-seq [[1 2] [3 4]]) '([1 2] [1 2] [3 4] [3 4]))

(= (m-dup-seq [44 33]) [44 44 33 33])
