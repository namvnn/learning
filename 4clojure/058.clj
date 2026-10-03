(defn m-compose [& fns]
  (fn [& args]
    (first
     (reduce
      (fn [a f] (vector (apply f a)))
      args
      (reverse fns)))))

(= [3 2 1] ((m-compose rest reverse) [1 2 3 4]))

(= 5 ((m-compose (partial + 3) second) [1 2 3 4]))

(= true ((m-compose zero? #(mod % 8) +) 3 5 7 9))

(= "HELLO" ((m-compose #(.toUpperCase %) #(apply str %) take) 5 "hello world"))
