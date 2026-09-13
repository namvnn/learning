(defn m-factorial [n]
  (if (= n 0)
    1
    (* n (m-factorial (dec n)))))

(= (m-factorial 1) 1)

(= (m-factorial 3) 6)

(= (m-factorial 5) 120)

(= (m-factorial 8) 40320)
