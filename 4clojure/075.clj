(defn m-euler-totient [n]
  (defn m-gcd [a b]
    (if (zero? b)
      a
      (m-gcd b (mod a b))))
  (if (= n 1)
    1
    (->> (range 1 n)
         (filter #(= (m-gcd % n) 1))
         (count))))

(= (m-euler-totient 1) 1)

(= (m-euler-totient 10) (count '(1 3 7 9)) 4)

(= (m-euler-totient 40) 16)

(= (m-euler-totient 99) 60)
