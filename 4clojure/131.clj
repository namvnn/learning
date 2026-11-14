(defn m-sum-some-set-subsets? [& sets]
  (letfn [(sums [[x & xs]]
               (reduce
                (fn [acc val] (into acc (cons val (map (partial + val) acc))))
                #{x} xs))]
    (not= #{} (reduce
               #(set (filter %1 %2))
               (map (comp sums seq) sets)))))

(= true (m-sum-some-set-subsets? #{-1 1 99}
                                #{-2 2 888}
                                #{-3 3 7777}))

(= false (m-sum-some-set-subsets? #{1}
                                 #{2}
                                 #{3}
                                 #{4}))

(= true  (m-sum-some-set-subsets? #{1}))

(= false (m-sum-some-set-subsets? #{1 -3 51 9}
                                 #{0}
                                 #{9 2 81 33}))

(= true  (m-sum-some-set-subsets? #{1 3 5}
                                 #{9 11 4}
                                 #{-3 12 3}
                                 #{-3 4 -2 10}))

(= false (m-sum-some-set-subsets? #{-1 -2 -3 -4 -5 -6}
                                 #{1 2 3 4 5 6 7 8 9}))

(= true  (m-sum-some-set-subsets? #{1 3 5 7}
                                 #{2 4 6 8}))

(= true  (m-sum-some-set-subsets? #{-1 3 -5 7 -9 11 -13 15}
                                 #{1 -3 5 -7 9 -11 13 -15}
                                 #{1 -1 2 -2 4 -4 8 -8}))

(= true  (m-sum-some-set-subsets? #{-10 9 -8 7 -6 5 -4 3 -2 1}
                                 #{10 -9 8 -7 6 -5 4 -3 2 -1}))
