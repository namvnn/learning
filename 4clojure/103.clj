(defn m-combination [n s]
  (cond
    (= n (count s)) #{s}
    (> n (count s)) #{}
    :else (loop [i 1
                 r (->> (for [x s] #{x}) (set))]
            (if (= i n)
              r
              (recur (inc i)
                     (->> (for [x s y r] (conj y x))
                          (filter #(= (count %) (inc i)))
                          (set)))))))

(= (m-combination 1 #{4 5 6}) #{#{4} #{5} #{6}})

(= (m-combination 10 #{4 5 6}) #{})

(= (m-combination 2 #{0 1 2}) #{#{0 1} #{0 2} #{1 2}})

(= (m-combination 3 #{0 1 2 3 4}) #{#{0 1 2} #{0 1 3} #{0 1 4} #{0 2 3} #{0 2 4}
                                    #{0 3 4} #{1 2 3} #{1 2 4} #{1 3 4} #{2 3 4}})

(= (m-combination 4 #{[1 2 3] :a "abc" "efg"}) #{#{[1 2 3] :a "abc" "efg"}})

(= (m-combination 2 #{[1 2 3] :a "abc" "efg"}) #{#{[1 2 3] :a} #{[1 2 3] "abc"} #{[1 2 3] "efg"}
                                                 #{:a "abc"} #{:a "efg"} #{"abc" "efg"}})
