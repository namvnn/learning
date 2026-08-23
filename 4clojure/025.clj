(defn m-odd [x] (filter odd? x))

(= (m-odd #{1 2 3 4 5}) '(1 3 5))

(= (m-odd [4 2 1 6]) '(1))

(= (m-odd [2 2 4 6]) '())

(= (m-odd [1 1 1 3]) '(1 1 1 3))
