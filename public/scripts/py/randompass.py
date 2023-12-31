import random
import string

def generate_password():
    # Generate 2 random integers
    digits = ''.join(random.choice(string.digits) for _ in range(2))

    # Generate 2 random lowercase letters
    lowercase_letters = ''.join(random.choice(string.ascii_lowercase) for _ in range(2))

    # Generate 2 random uppercase letters
    uppercase_letters = ''.join(random.choice(string.ascii_uppercase) for _ in range(2))

    # Generate 2 random punctuation characters
    punctuation_chars = ''.join(random.choice(string.punctuation) for _ in range(2))

    # Concatenate the generated parts to form the final password
    password = digits + lowercase_letters + uppercase_letters + punctuation_chars

    # Shuffle the characters to make the password more random
    password_list = list(password)
    random.shuffle(password_list)
    shuffled_password = ''.join(password_list)

    return shuffled_password

# Example: Generate and print a password
password = generate_password()
print("""

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Random password generator</title>
</head>
<body><center>
    <header><h1>
      Random password generator
    </h1></header>
    <main>
        <h3>this is your random password</h3>
        <h1>{}</h1>
    </main></center>
</body>
</html>
""".format(password))
