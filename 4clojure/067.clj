(defn m-prime [c]
  (letfn [(is-prime? [x]
                     (if (< x 2)
                       false
                       (loop [i 2]
                         (cond
                           (< x (* i i)) true
                           (= 0 (mod x i)) false
                           :else (recur (inc i))))))]
    (take c (filter is-prime? (range)))))

(= (m-prime 2) [2 3])

(= (m-prime 5) [2 3 5 7 11])

(= (last (m-prime 100)) 541)
