(defn m-frequencies [coll]
  (reduce #(assoc %1 %2
                  (if (contains? %1 %2)
                    (inc (get %1 %2))
                    1))
          {}
          coll))

(= (m-frequencies [1 1 2 3 2 1 1]) {1 4, 2 2, 3 1})

(= (m-frequencies [:b :a :b :a :b]) {:a 2, :b 3})

(= (m-frequencies '([1 2] [1 3] [1 3])) {[1 2] 1, [1 3] 2})
