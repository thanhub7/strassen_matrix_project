import serial
import time
import numpy as np


ser = serial.Serial('/dev/serial0', 9600, timeout=10)
time.sleep(2)




A = np.array([
	[1, 2, 3, 4],
	[5, 6, 7, 8],
	[9, 1, 2, 3],
	[4, 5, 6, 7]
])


B = np.array([
	[7, 6, 5, 4],
	[3, 2, 1, 0],
	[8, 7, 6, 5],
	[4, 3, 2, 1]
])





def split_matrix(M):
	return M[:2, :2], M[:2, 2:], M[2:, :2], M[2:, 2:]



A11, A12, A21, A22 = split_matrix(A)
B11, B12, B21, B22 = split_matrix(B)



M1_left = A12 - A22
M1_right = B21 + B22



def send_matrix_pair(tag, A, B):
	ser.write((tag + '\n').encode())
	for row in A:
		ser.write((' '.join(map(str, row)) + '\n').encode())
	for row in B:
		ser.write((' '.join(map(str, row)) + '\n').encode())

	print("Sent matrices for", tag)


def receive_result():
	header = ser.readline().decode().strip()
	print("Header received:", repr(header))
	if header != "RESULT":
		raise ValueError("Invalid reply")

	result = []
	for _ in range(2):
		line = ser.readline().decode().strip()
		row = list(map(int, line.split()))
		result.append(row)

	return np.array(result)


send_matrix_pair("M1", M1_left, M1_right)
print("Waiting for Arduino to reply...")
result = receive_result()
print("Received M1 = \n", result)

