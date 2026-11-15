(defn m-dot-product [m1 m2]
  (apply + (map * m1 m2)))

(= 0 (m-dot-product [0 1 0] [1 0 0]))

(= 3 (m-dot-product [1 1 1] [1 1 1]))

(= 32 (m-dot-product [1 2 3] [4 5 6]))

(= 256 (m-dot-product [2 5 6] [100 10 1]))
