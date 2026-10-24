(defn m-roman-numeral [n]
  (letfn [(rn [r b] (str r (m-roman-numeral (- n b))))]
    (cond
      (>= n 1000) (rn "M" 1000)
      (>= n 900) (rn "CM" 900)
      (>= n 500) (rn "D" 500)
      (>= n 100) (rn "C" 100)
      (>= n 90) (rn "XC" 90)
      (>= n 40) (rn "XL" 40)
      (>= n 10) (rn "X" 10)
      (>= n 9) (rn "IX" 9)
      (>= n 5) (rn "V" 5)
      (>= n 4) (rn "IV" 4)
      (>= n 1) (rn "I" 1))))

(= "I" (m-roman-numeral 1))

(= "I" (m-roman-numeral 1))

(= "XXX" (m-roman-numeral 30))

(= "IV" (m-roman-numeral 4))

(= "CXL" (m-roman-numeral 140))

(= "DCCCXXVII" (m-roman-numeral 827))

(= "MMMCMXCIX" (m-roman-numeral 3999))

(= "XLVIII" (m-roman-numeral 48))
