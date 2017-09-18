import math

data = [238180748,210624598,220036229,205198963,205556281,204390065]

sum = 0
for i in data:
    sum += i

average = float(sum) / len(data)
print ("average : ", average)
sum = 0
for i in data:
    sum += (i-average) ** 2

var = float(sum) / len(data)
print ("variance: ", var)
desv = math.sqrt(var)
print ("desvest: ", desv)
