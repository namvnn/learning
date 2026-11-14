(defn m-digits-and-base [x base]
  (loop [q x
         l '()]
    (if (< q base)
      (conj l q)
      (recur (quot q base) (conj l (mod q base))))))

(= [1 2 3 4 5 0 1] (m-digits-and-base 1234501 10))

(= [0] (m-digits-and-base 0 11))

(= [1 0 0 1] (m-digits-and-base 9 2))

(= [1 0] (let [n (rand-int 100000)] (m-digits-and-base n n)))

(= [22 6 10 5 0 19 6 9 6 31] (m-digits-and-base js/Number.MAX_SAFE_INTEGER 42))

