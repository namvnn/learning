(defn m-juxtaposition [& fns]
  (fn [& args]
    (reduce
     (fn [arr f] (conj arr (apply f args)))
     []
     fns)))

(= [21 6 1] ((m-juxtaposition + max min) 2 3 5 1 6 4))

(= ["HELLO" 5] ((m-juxtaposition #(.toUpperCase %) count) "hello"))

(= [2 6 4] ((m-juxtaposition :a :c :b) {:a 2, :b 4, :c 6, :d 8 :e 10}))
