(defn m-lcm [& args]
  (letfn [(gcd [a b]
               (if (zero? b)
                 a
                 (gcd b (mod a b))))
          (lcm [a b]
               (/ (* a b) (gcd a b)))]
    (reduce lcm args)))

(== (m-lcm 2 3) 6)

(== (m-lcm 5 3 7) 105)

(== (m-lcm 1/3 2/5) 2)

(== (m-lcm 3/4 1/6) 3/2)

(== (m-lcm 7 5/7 2 3/5) 210)
