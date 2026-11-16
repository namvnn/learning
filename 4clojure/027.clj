(defn m-palindrome? [x]
  (= (reverse x) (reverse (reverse x))))

(false? (m-palindrome? '(1 2 3 4 5)))

(true? (m-palindrome? "racecar"))

(true? (m-palindrome? [:foo :bar :foo]))

(true? (m-palindrome? '(1 1 3 3 1 1)))

(false? (m-palindrome? '(:a :b :c)))
