(defn m-perfect-numbers [n]
  (->> (range 1 n)
       (filter #(= (mod n %) 0))
       (apply +)
       (= n)))

(= (m-perfect-numbers 6) true)

(= (m-perfect-numbers 7) false)

(= (m-perfect-numbers 496) true)

(= (m-perfect-numbers 500) false)

(= (m-perfect-numbers 8128) true)
