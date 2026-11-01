(defn m-balanced-prime? [n]
  (letfn [(is-prime? [x]
                     (if (< x 2)
                       false
                       (loop [i 2]
                         (cond
                           (< x (* i i)) true
                           (= 0 (mod x i)) false
                           :else (recur (inc i))))))
          (find-prime [n f]
                      (let [next-n (f n)]
                        (cond
                          (< next-n 2) 0
                          (is-prime? next-n) next-n
                          :else (find-prime next-n f))))]
    (and (> n 2)
         (is-prime? n)
         (= (* 2 n)
            (+ (find-prime n inc)
               (find-prime n dec))))))

(= false (m-balanced-prime? 4))

(= true (m-balanced-prime? 563))

(= 1103 (nth (filter m-balanced-prime? (range)) 15))
