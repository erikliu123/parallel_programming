multiexe="akari_multi_allsolution"
single="akari_simple"
echo "THIS FIND ALL SOLUTIONS"
cat akari.h | grep MAX_ANS
g++ akari_thread.cpp  -o $multiexe -lpthread -std=c++11
g++ akari_single.cpp  -o $single  -std=c++11
echo "SINGlE THREAD"
./$single | grep time
echo "MULTI THREAD"
#./$multiexe  | grep -E '(time|seed)'
