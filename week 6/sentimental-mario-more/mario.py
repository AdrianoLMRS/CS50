from cs50 import get_int

height = get_int("Height: ")

while True:
    if height > 0 and height <= 8:

        for i in range(1, height + 1):
            hashes = i
            spaces = height - hashes

            print(" " * spaces, end="")
            print("#" * hashes, end="")
            print("  ", end="")
            print("#" * hashes)
        break

    else:
        height = get_int("Height: ")
