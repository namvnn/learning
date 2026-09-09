(defn m-max [& nums]
  (reduce
   (fn [max num]
     (if (> num max) num max))
   nums))

(= (m-max 1 8 3 4) 8)

(= (m-max 30 20) 30)

(= (m-max 45 67 11) 67)
