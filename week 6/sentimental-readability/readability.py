from cs50 import get_string

# Get text input
text = get_string("Text: ")

letters = 0
words = 0
sentences = 0

# Count letters, words, and sentences
for i, c in enumerate(text):
    # Count letters
    if c.isalpha():
        letters += 1

    # Count words
    if c.isspace() and i + 1 < len(text) and not text[i + 1].isspace():
        words += 1

    # Count sentences (. ! ?)
    if c in ".!?":
        sentences += 1

words += 1  # Last word

# Compute averages per 100 words
letters_avg = (letters / words) * 100
sentences_avg = (sentences / words) * 100

# Compute Coleman-Liau index
grade = round(0.0588 * letters_avg - 0.296 * sentences_avg - 15.8)

# Print grade
if grade < 1:
    print("Before Grade 1")
elif grade >= 16:
    print("Grade 16+")
else:
    print(f"Grade {grade}")
