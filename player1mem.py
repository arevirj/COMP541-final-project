import numpy as np

rows, cols = 30, 40
mem = np.full((rows, cols), 3)  # Blue background

# Spell "PLAYER 1 WIN"
text = [
    "PLAYER",
    "1",
    "WIN"
]

start_row = 10
for i, word in enumerate(text):
    start_col = (cols - len(word)) // 2
    for j, char in enumerate(word):
        mem[start_row + i * 2, start_col + j] = 0  # Black letters

with open("player1_win.mem", "w") as f:
    for row in mem:
        for val in row:
            f.write(f"{val}\n")
