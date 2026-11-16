(defn m-big-divide [n a b]
  (letfn [(calc [x]
            (let [y (quot (dec n) x)]
              (/ (*' x y (inc y)) 2)))]
    (-'
     (+' (calc a) (calc b))
     (calc (*' a b)))))

(= 0 (m-big-divide 3 17 11))

(= 23 (m-big-divide 10 3 5))

(= 233168 (m-big-divide 1000 3 5))

(= "2333333316666668" (str (m-big-divide 100000000 3 5)))

(= "110389610389889610389610"
   (str (m-big-divide (* 10000 10000 10000) 7 11)))

(= "1277732511922987429116"
   (str (m-big-divide (* 10000 10000 10000) 757 809)))

(= "4530161696788274281"
   (str (m-big-divide (* 10000 10000 1000) 1597 3571)))
