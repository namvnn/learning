(defn m-to-binary [x]
  (loop [q x
         l '()]
    (if (< q 2)
      (conj l q)
      (recur (quot q 2) (conj l (mod q 2))))))

(defn m-to-decimal [s]
  (letfn [(pow [n m] (if (zero? m) 1 (* n (pow n (dec m)))))]
    (loop [v (vec (map (comp read-string str) s))
           x 0
           i 0]
      (if (empty? v)
        x
        (recur (pop v) (+ x (* (peek v) (int (pow 2 i)))) (inc i))))))

(= 0     (m-to-decimal "0"))

(= 7     (m-to-decimal "111"))

(= 8     (m-to-decimal "1000"))

(= 9     (m-to-decimal "1001"))

(= 255   (m-to-decimal "11111111"))

(= 1365  (m-to-decimal "10101010101"))

(= 65535 (m-to-decimal "1111111111111111"))
