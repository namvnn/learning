(defn m-interleave [x y]
  (let [c (min (count x) (count y))]
    (loop [i 0
           v []]
      (if (= i c) v
        (recur (inc i)
               (conj v (x i) (y i)))))))

(= (m-interleave [1 2 3] [:a :b :c]) '(1 :a 2 :b 3 :c))

(= (m-interleave [1 2] [3 4 5 6]) '(1 3 2 4))

(= (m-interleave [1 2 3 4] [5]) [1 5])

(= (m-interleave [30 20] [25 15]) [30 25 20 15])
