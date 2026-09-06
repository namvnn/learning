(defn m-range [start end]
  (loop [i start
         r []]
    (if (= i end)
      r
      (recur (inc i) (conj r i)))))

(= (m-range 1 4) '(1 2 3))

(= (m-range -2 2) '(-2 -1 0 1))

(= (m-range 5 8) '(5 6 7))
