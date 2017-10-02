import math

def WeightedAverage(data):
    numerator = 0
    denominator = 0
    for v in data:
        numerator += v[0] * v[1]
        denominator += v[1]
    return numerator / denominator

def WeightedStandardDeviation(data):
    numerator = 0
    denominator = 0
    w_average = WeightedAverage(data)
    for v in data:
        numerator += v[1] * ((v[0] - w_average) ** 2);
        denominator += v[1]
    denominator = ((len(data) - 1) * denominator) / len(data)
    return math.sqrt(numerator / denominator)


# data = [238180748,210624598,220036229,205198963,205556281,204390065]
#
# sum = 0
# for i in data:
#     sum += i
#
# average = float(sum) / len(data)
# print ("average : ", average)
# sum = 0
# for i in data:
#     sum += (i-average) ** 2
#
# var = float(sum) / len(data)
# print ("variance: ", var)
# desv = math.sqrt(var)
# print ("desvest: ", desv)

data = [(256,1),(257,1),(256,1),(283,1),(211,1),(211,1),(277,1),(208,1)]
print(WeightedStandardDeviation(data))
