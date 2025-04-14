rows = 30
cols = 40

with open("tron_board.mem", "w") as f:
    for r in range(rows):
        for c in range(cols):
            if r == 0 or r == rows - 1 or c == 0 or c == cols - 1:
                f.write("1\n")  # Border
            else:
                f.write("0\n")  # Empty space