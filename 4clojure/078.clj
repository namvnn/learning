(defn m-trampoline [f & args]
  (if (fn? f) (m-trampoline (apply f args)) f))

(= (letfn [(triple [x] #(sub-two (* 3 x)))
           (sub-two [x] #(stop? (- x 2)))
           (stop? [x] (if (> x 50) x #(triple x)))]
     (m-trampoline triple 2))
   82)

(= (letfn [(my-even? [x] (if (zero? x) true #(my-odd? (dec x))))
           (my-odd? [x] (if (zero? x) false #(my-even? (dec x))))]
     (map (partial m-trampoline my-even?) (range 6)))
   [true false true false true false])
