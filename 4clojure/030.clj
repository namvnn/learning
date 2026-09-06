(defn m-compress [x]
  (reduce
   (fn [l v]
     (if (= (last l) v)
       l
       (conj l v)))
   [(first x)]
   (rest x)))

(= (apply str (m-compress "Leeeeeerrroyyy")) "Leroy")

(= (m-compress [1 1 2 3 3 2 2 3]) '(1 2 3 2 3))

(= (m-compress [[1 2] [1 2] [3 4] [1 2]]) '([1 2] [3 4] [1 2]))
