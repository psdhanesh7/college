class Vector:
	def __init__(self, x1, x2, bias, target):
		self.x1 = x1
		self.x2 = x2
		self.b = bias
		self.target = target

def activationFunction(x, teta):
	if(x > teta):
		return 1
	elif(x >= -teta):
		return 0
	else:
		return -1 

# teta = 0.2
# alpha = 1

# inputVectors = [Vector(1, 1, 1, 1), Vector(1, 0, 1, 1), Vector(0, 1, 1, 1), Vector(0, 0, 1, -1)]

inputVectors = []

teta = float(input("Enter threshold value for activation funtion: "))
alpha = int(input("Enter the learning rate alpha: "))

bias = int(input("Enter 1 if bias is present: "))

n = int(input("Enter the number of input vectors: "))

for i in range(0, n):
	
	print("Enter input vector {0}".format(i + 1))
	x1 = int(input("x1: "))
	x2 = int(input("x2: "))
	target = int(input("target: "))

	inputVectors.append(Vector(x1, x2, bias, target))

w1 = 0
w2 = 0
b = 0

epoch = 1

while(True):

	print("Epoch ", epoch)
	print("x1\tx2\tb\tt\tyin\tY\tdw1\tdw2\tdb\t w1 \t w2 \t b")

	exit = True

	for i in range(0, n):
		currentVector = inputVectors[i];

		yin = currentVector.x1 * w1 + currentVector.x2 * w2 + currentVector.b * b
		yout = activationFunction(yin, teta)

		if(yout == currentVector.target):
			delta_w1 = 0
			delta_w2 = 0
			delta_b = 0
		else:
			exit = False
			delta_w1 = alpha * currentVector.target * currentVector.x1
			delta_w2 = alpha * currentVector.target * currentVector.x2
			delta_b = alpha * currentVector.target * currentVector.b

		w1 += delta_w1
		w2 += delta_w2
		b += delta_b

		print("{0}\t{1}\t{2}\t{3}\t{4}\t{5}\t{6}\t{7}\t{8}\t{9}\t{10}\t{11}".format(currentVector.x1, currentVector.x2, currentVector.b, currentVector.target, yin, yout, delta_w1, delta_w2, delta_b, w1, w2, b))

		

	print("----------------------------------------------------------------------------------------------")
	if(exit):
		break

	epoch += 1


