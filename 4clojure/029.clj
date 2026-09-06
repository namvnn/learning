(defn m-caps [x]
  (apply str (re-seq #"[A-Z]" x)))

(= (m-caps "HeLlO, WoRlD!") "HLOWRD")

(empty? (m-caps "nothing"))

(= (m-caps "$#A(*&987Zf") "AZ")
