(defn m-type [coll]
  (let [c (conj (empty coll) [:x 1] [:x 1])]
    (cond
      (and (= (count c) 1) (= (:x c) 1)) :map
      (and (= (count c) 1)) :set
      (= (first (conj c :xx)) :xx) :list
      (= (last (conj c :xx)) :xx) :vector)))

(= :map (m-type {:a 1, :b 2}))

(= :list (m-type (range (rand-int 20))))

(= :vector (m-type [1 2 3 4 5 6]))

(= :set (m-type #{10 (rand-int 5)}))

(= [:map :set :vector :list] (map m-type [{} #{} [] ()]))
