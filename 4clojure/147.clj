(defn m-pascal-trapezoid [xs]
  (lazy-seq (cons xs
                  (m-pascal-trapezoid (flatten [(first xs)
                                                (map #(apply +' %) (partition 2 1 xs))
                                                (last xs)])))))

(= (second (m-pascal-trapezoid [2 3 2])) [2 5 5 2])

(= (take 5 (m-pascal-trapezoid [1])) [[1] [1 1] [1 2 1] [1 3 3 1] [1 4 6 4 1]])

(= (take 2 (m-pascal-trapezoid [3 1 2])) [[3 1 2] [3 4 3 2]])

(= (take 100 (m-pascal-trapezoid [2 4 2])) (rest (take 101 (m-pascal-trapezoid [2 2]))))
