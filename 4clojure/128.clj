(defn m-recognize-playing-card [s]
  {:suit  ({\D :diamond
            \H :heart
            \S :spade
            \C :club}
           (first s))
   :rank (.indexOf [\2 \3 \4 \5 \6 \7 \8 \9 \T \J \Q \K \A] (second s))})

(= {:suit :diamond :rank 10} (m-recognize-playing-card "DQ"))

(= {:suit :heart :rank 3} (m-recognize-playing-card "H5"))

(= {:suit :club :rank 12} (m-recognize-playing-card "CA"))

(= (range 13) (map (comp :rank m-recognize-playing-card str)
                   '[S2 S3 S4 S5 S6 S7
                     S8 S9 ST SJ SQ SK SA]))
