(defn m-sequs-horribilis [n [f & r :as c]]
  (cond
    (coll? f) [(m-sequs-horribilis n f)]
    (nil? f) []
    (> f n) []
    :else (concat [f] (m-sequs-horribilis (- n f) r))))

(= (m-sequs-horribilis 10 [1 2 [3 [4 5] 6] 7]) '(1 2 (3 (4))))

(= (m-sequs-horribilis 30 [1 2 [3 [4 [5 [6 [7 8]] 9]] 10] 11]) '(1 2 (3 (4 (5 (6 (7)))))))

(= (m-sequs-horribilis 9 (range)) '(0 1 2 3))

(= (m-sequs-horribilis 1 [[[[[1]]]]]) '(((((1))))))

(= (m-sequs-horribilis 0 [1 2 [3 [4 5] 6] 7]) '())

(= (m-sequs-horribilis 0 [0 0 [0 [0]]]) '(0 0 (0 (0))))

(= (m-sequs-horribilis 1 [-10 [1 [2 3 [4 5 [6 7 [8]]]]]])
   '(-10 (1 (2 3 (4)))))
