(defn m-happy-numbers [n]
  (let [sum-sqr (fn sum-s [n]
                  (apply + (map (comp #(* % %) read-string str) (str n))))
        limit 1000]
    (=
     (last (take limit (iterate #(sum-sqr %) n)))
     1)))

(= (m-happy-numbers 7) true)

(= (m-happy-numbers 986543210) true)

(= (m-happy-numbers 2) false)

(= (m-happy-numbers 3) false)
