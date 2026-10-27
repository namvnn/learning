(defn m-power [n]
  (fn [base] (reduce * (repeat n base))))

(= 256 ((m-power 2) 16), ((m-power 8) 2))

(= [1 8 27 64] (map (m-power 3) [1 2 3 4]))

(= [1 2 4 8 16] (map #((m-power %) 2) [0 1 2 3 4]))
