(defn m-symmetric-difference [s1 s2]
  (let [j (into s1 s2)
        b (set (filter #(and (contains? s1 %) (contains? s2 %)) j))]
    (into (set (filter #(not (contains? b %)) s1))
          (filter #(not (contains? b %)) s2))))

(= (m-symmetric-difference #{1 2 3 4 5 6} #{1 3 5 7}) #{2 4 6 7})

(= (m-symmetric-difference #{:a :b :c} #{}) #{:a :b :c})

(= (m-symmetric-difference #{} #{4 5 6}) #{4 5 6})

(= (m-symmetric-difference #{[1 2] [2 3]} #{[2 3] [3 4]}) #{[1 2] [3 4]})
