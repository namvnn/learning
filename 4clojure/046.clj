(defn m-flip-out [f]
  (fn [& args]
    (apply f (reverse args))))

(= 3 ((m-flip-out nth) 2 [1 2 3 4 5]))

(= true ((m-flip-out >) 7 8))

(= 4 ((m-flip-out quot) 2 8))

(= [1 2 3] ((m-flip-out take) [1 2 3 4 5] 3))
