(defn m-split-by-type [coll]
  (vals (group-by type coll)))

(= (set (m-split-by-type [1 :a 2 :b 3 :c])) #{[1 2 3] [:a :b :c]})

(= (set (m-split-by-type [:a "foo" "bar" :b])) #{[:a :b] ["foo" "bar"]})

(= (set (m-split-by-type [[1 2] :a [3 4] 5 6 :b])) #{[[1 2] [3 4]] [:a :b] [5 6]})
