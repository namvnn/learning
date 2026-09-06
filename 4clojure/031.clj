(defn m-pack-seq [x]
  (reduce
   (fn [l v]
     (if (= (last (last l)) v)
       (conj (pop l) (conj (peek l) v))
       (conj l [v])))
   [[(first x)]]
   (rest x)))

(= (m-pack-seq [1 1 2 1 1 1 3 3]) '((1 1) (2) (1 1 1) (3 3)))

(= (m-pack-seq [:a :a :b :b :c]) '((:a :a) (:b :b) (:c)))

(= (m-pack-seq [[1 2] [1 2] [3 4]]) '(([1 2] [1 2]) ([3 4])))
