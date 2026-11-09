(defn m-universal-computation-engine [[op & args]]
  (fn [m]
    (apply ({'/ / '* * '+ + '- -} op)
           (map #(cond
                   (seq? %) ((m-universal-computation-engine %) m)
                   (symbol? %) (m %)
                   :else %)
                args))))

(= 2 ((m-universal-computation-engine '(/ a b))
      '{b 8
        a 16}))

(= 8 ((m-universal-computation-engine '(+ a b 2))
      '{a 2
        b 4}))

(= [6 0 -4]
   (map (m-universal-computation-engine '(* (+ 2 a) (- 10 b)))
        '[{a 1
           b 8}
          {b 5
           a -2}
          {a 2
           b 11}]))

(= 1 ((m-universal-computation-engine '(/ (+ x 2) (* 3 (+ y 1))))
      '{x 4
        y 1}))
