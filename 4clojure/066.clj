(defn m-gcd [a b]
  (if (zero? b)
    a
    (m-gcd b (mod a b))))

(= (m-gcd 2 4) 2)

(= (m-gcd 10 5) 5)

(= (m-gcd 5 7) 1)

(= (m-gcd 1023 858) 33)
