(defn m-palindrome-detector [x]
  (= (reverse x) (reverse (reverse x))))

(false? (m-palindrome-detector '(1 2 3 4 5)))

(true? (m-palindrome-detector "racecar"))

(true? (m-palindrome-detector [:foo :bar :foo]))

(true? (m-palindrome-detector '(1 1 3 3 1 1)))

(false? (m-palindrome-detector '(:a :b :c)))
