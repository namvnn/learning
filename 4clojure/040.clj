(defn m-interpose [sep coll]
  (pop (reduce
        (fn [l v] (conj l v sep))
        []
        coll)))

(= (m-interpose 0 [1 2 3]) [1 0 2 0 3])

(= (apply str (m-interpose ", " ["one" "two" "three"])) "one, two, three")

(= (m-interpose :z [:a :b :c :d]) [:a :z :b :z :c :z :d])
