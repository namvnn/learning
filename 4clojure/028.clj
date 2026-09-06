(defn m-flatten [x]
  (if (not (sequential? x)) (vector x)
    (apply concat (map m-flatten x))))

(= (m-flatten '((1 2) 3 [4 [5 6]])) '(1 2 3 4 5 6))

(= (m-flatten ["a" ["b"] "c"]) '("a" "b" "c"))

(= (m-flatten '((((:a))))) '(:a))
