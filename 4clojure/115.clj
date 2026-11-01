(defn m-balance? [n]
  (let [x (map (comp read-string str) (str n))
        c (count x)
        i (int (/ c 2))]
    (= (reduce + (take i x))
       (reduce + (take-last i x)))))

(= true (m-balance? 11))

(= true (m-balance? 121))

(= false (m-balance? 123))

(= true (m-balance? 0))

(= false (m-balance? 88099))

(= true (m-balance? 89098))

(= true (m-balance? 89089))

(= (take 20 (filter m-balance? (range)))
   [0 1 2 3 4 5 6 7 8 9 11 22 33 44 55 66 77 88 99 101])
