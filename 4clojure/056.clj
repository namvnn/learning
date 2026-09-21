(defn m-distinct [coll]
  (reduce
   (fn [acc val]
         (if (contains? (set acc) val)
           acc
           (conj acc val)))
   []
   coll))

(= (m-distinct [1 2 1 3 1 2 4]) [1 2 3 4])

(= (m-distinct [:a :a :b :b :c :c]) [:a :b :c])

(= (m-distinct '([2 4] [1 2] [1 3] [1 3])) '([2 4] [1 2] [1 3]))

(= (m-distinct (range 50)) (range 50))
