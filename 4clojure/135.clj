(defn m-calc-infix [& args]
  (reduce
   (fn [acc val] 
     (if (fn? val)
       (partial val acc)
       (acc val)))
   identity
   args))

(= 7  (m-calc-infix 2 + 5))

(= 42 (m-calc-infix 38 + 48 - 2 / 2))

(= 8  (m-calc-infix 10 / 2 - 1 * 2))

(= 72 (m-calc-infix 20 / 2 + 2 + 4 + 8 - 6 - 10 * 9))
