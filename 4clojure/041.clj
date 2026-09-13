(defn m-drop-every-nth [coll n]
  (loop [i 1
         r []]
    (if (> i (count coll))
      r
      (recur (inc i)
             (if (= (mod i n) 0) r (conj r (nth coll (dec i))))))))

(= (m-drop-every-nth [1 2 3 4 5 6 7 8] 3) [1 2 4 5 7 8])

(= (m-drop-every-nth [:a :b :c :d :e :f] 2) [:a :c :e])

(= (m-drop-every-nth [1 2 3 4 5 6] 4) [1 2 3 5 6])
