import sys
import os

def chicken_to_minichicken(code: str) -> str:
    res = []
    code = code.lower()
    for l in code.split("\n"):
        res.append(str(l.count("chicken")))
    return " ".join(res)

def minichicken_to_chicken(code: str) -> str:
    res = []
    for n in code.split():
        res.append(" ".join("chicken" for _ in range(int(n))))
    return "\n".join(res)

def main():

    # parsing the arguments

    if ( len(sys.argv) < 3 ):
        print("Usage:\n\tminichicken.py c2m/m2c filename\n")
        return
    
    command  = sys.argv[1]
    filename = sys.argv[2]
    filename_no_ext = os.path.splitext(filename)[0]

    if command == "c2m":
        if os.path.exists(filename):
            with open(filename, "r") as f:
                with open(filename_no_ext + ".minichicken", "w") as target_f:
                    target_f.write(chicken_to_minichicken(f.read()))
        else:
            print("File " + filename + " does not exist!\n")

    elif command == "m2c":
        if os.path.exists(filename):
            with open(filename, "r") as f:
                with open(filename_no_ext + ".chicken", "w") as target_f:
                    target_f.write(minichicken_to_chicken(f.read()))
        else:
            print("File " + filename + " does not exist!\n")

if __name__ == "__main__":
    main()
