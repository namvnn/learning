(defn m-global-take-while [n p c]
  (let [[x [y & z]] (split-with (complement p) c)]
    (if (= n 1)
      x
      (concat x [y] (m-global-take-while (dec n) p z)))))

(= [2 3 5 7 11 13]
   (m-global-take-while 4 #(= 2 (mod % 3))
                        [2 3 5 7 11 13 17 19 23]))

(= ["this" "is" "a" "sentence"]
   (m-global-take-while 3 #(some #{\i} %)
                        ["this" "is" "a" "sentence" "i" "wrote"]))

(= ["this" "is"]
   (m-global-take-while 1 #{"a"}
                        ["this" "is" "a" "sentence" "i" "wrote"]))
