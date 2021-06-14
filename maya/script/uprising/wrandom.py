import random

class StepRandomizer(object):
    def __init__(self, max_steps, power):
        sentinel = max_steps+1
        w = {}
        self.__max = 0.0
        self.__weights = []
        for value in range(sentinel):
            j = 1.0-(value / float(sentinel))
            weight = pow(j, power)
            self.__max += weight
            self.__weights.append((self.__max, value))
            if value > 0:
                self.__max += weight
                self.__weights.append((self.__max, -value))

    def random(self):
        r = random.random() * self.__max
        for ceil, value in self.__weights:
            if ceil > r:
                return value


# sr = StepRandomizer (4, 2)
# results = {}
# for i in range (10000):
#     res = sr.random()
#     if not res in results:
#         results[res] = 0
#     results [res] += 1

# print ('After 10000 rounds the distribution is:')
# print (results)
#
