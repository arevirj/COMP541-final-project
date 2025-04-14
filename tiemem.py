import numpy as np

rows, cols = 30, 40
mem = np.full((rows, cols), 4)  # Yellow background

text = ["TIE"]

start_row = 14
for i, word in enumerate(text):
    start_col = (cols - len(word)) // 2
    for j, char in enumerate(word):
        mem[start_row + i * 2, start_col + j] = 0  # Black letters

with open("tie.mem", "w") as f:
    for row in mem:
        for val in row:
            f.write(f"{val}\n")
