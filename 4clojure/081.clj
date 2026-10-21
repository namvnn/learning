(defn m-set-intersection [x y]
  (->> x
       (filter #(contains? y %))
       (set)))

(= (m-set-intersection #{0 1 2 3} #{2 3 4 5}) #{2 3})

(= (m-set-intersection #{0 1 2} #{3 4 5}) #{})

(= (m-set-intersection #{:a :b :c :d} #{:c :e :a :f :d}) #{:a :c :d})
