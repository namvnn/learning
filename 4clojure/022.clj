(defn m-count [x] (if (empty? x)
                    0
                    (+ 1 (m-count (rest x)))))

(= (m-count '(1 2 3 3 1)) 5)

(= (m-count "Hello World") 11)

(= (m-count [[1 2] [3 4] [5 6]]) 3)

(= (m-count '(13)) 1)

(= (m-count '(:a :b :c)) 3)
